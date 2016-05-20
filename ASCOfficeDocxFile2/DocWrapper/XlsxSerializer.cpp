#include "XlsxSerializer.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/File.h"
#include "../../XlsxSerializerCom/Reader/BinaryWriter.h"
#include "../../XlsxSerializerCom/Writer/BinaryReader.h"
#include "../../ASCOfficePPTXFile/Editor/FontPicker.h"

namespace BinXlsxRW{
	int g_nCurFormatVersion = 0;

	CXlsxSerializer::CXlsxSerializer()
	{
		m_pExternalDrawingConverter = NULL;
	}
	CXlsxSerializer::~CXlsxSerializer()
	{
	}
	bool CXlsxSerializer::loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sMediaDir)
	{
		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetMediaDstPath(sMediaDir);

		//����� � ����������
		TCHAR tFolder[256];
		TCHAR tDrive[256];
		_tsplitpath( sSrcFileName, tDrive, tFolder, NULL, NULL );
		CString sFolder = CString(tFolder);
		CString sDrive = CString(tDrive);
		CString sFileInDir = sDrive + sFolder;

		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = sFileInDir.AllocSysString();
		oOfficeDrawingConverter.SetAdditionalParam(CString(L"SourceFileDir2"), var);
		RELEASESYSSTRING(var.bstrVal);

		BinXlsxRW::BinaryFileReader oBinaryFileReader;
		oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, &oOfficeDrawingConverter, sXMLOptions);
		return true;
	}
	bool CXlsxSerializer::saveToFile(CString& sDstFileName, CString& sSrcPath, CString& sXMLOptions)
	{
		COfficeFontPicker* pFontPicker = new COfficeFontPicker();
		pFontPicker->Init(m_sFontDir);
		CFontManager* pFontManager = pFontPicker->get_FontManager();
		DocWrapper::FontProcessor fp;
		fp.setFontManager(pFontManager);
		
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_sEmbeddedFontsDir.IsEmpty())
		{
			NSDirectory::CreateDirectory(string2std_string(m_sEmbeddedFontsDir));

			pFontPicker->SetEmbeddedFontsDirectory(m_sEmbeddedFontsDir);
			pEmbeddedFontsManager = pFontPicker->GetNativeCutter();

			//������� ���� �����
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager ����������� ��� �����
			//��� ����������
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
			pEmbeddedFontsManager->CheckString(CString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//���������� ��� ������ "#NULL!", "#DIV/0!"...
			pEmbeddedFontsManager->CheckString(CString(_T("#!/?")));
			//���������� ��� num �������� �� ��������� � id �� 0 �� 49
			pEmbeddedFontsManager->CheckString(CString(_T(".%E+-():")));
		}

		NSBinPptxRW::CDrawingConverter oOfficeDrawingConverter;
		oOfficeDrawingConverter.SetFontManager(pFontManager);
		oOfficeDrawingConverter.SetFontDir(m_sFontDir);
		oOfficeDrawingConverter.SetFontPicker(pFontPicker);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(fp);
		oBinaryFileWriter.Open(sSrcPath, sDstFileName, pEmbeddedFontsManager, &oOfficeDrawingConverter, sXMLOptions);

		RELEASEOBJECT(pFontPicker);
		return true;
	}
	bool CXlsxSerializer::loadChart(CString& sChartPath, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize)
	{
		bool bRes = false;
		OOX::Spreadsheet::CChartSpace oChart(sChartPath);
		if(NULL != m_pExternalDrawingConverter)
		{
			long nStartPos = oBufferedStream.GetPosition();
			BinXlsxRW::BinaryCommonWriter oBcw(oBufferedStream);

			CString sOldRelsPath = m_pExternalDrawingConverter->GetRelsPath();
			m_pExternalDrawingConverter->SetRelsPath(sChartPath);

			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(oBufferedStream, m_pExternalDrawingConverter);	
			oBinaryChartWriter.WriteCT_ChartSpace(oChart);

			m_pExternalDrawingConverter->SetRelsPath(sOldRelsPath);

			long nEndPos = oBufferedStream.GetPosition();
			lDataSize = nEndPos - nStartPos;
			bRes = true;
		}
		return bRes;
	}
	bool CXlsxSerializer::saveChart(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sFilepath, CString& sContentTypePath, CString** sContentTypeElement)
	{
		bool bRes = false;
		*sContentTypeElement = NULL;
		if(NULL != m_pExternalDrawingConverter)
		{
			m_pExternalDrawingConverter->SetDstContentRels();

			//�������� sThemePath �� bsFilename ����������� ��� ����� theme ��������� �� ������� ���� bsFilename
			CString sThemePath;
			CString sFilenameReverse = sFilepath;sFilenameReverse.MakeReverse();
			int nIndex = sFilenameReverse.Find('\\');
			nIndex = sFilenameReverse.Find('\\', nIndex + 1);
			if(-1 != nIndex)
				sThemePath = sFilepath.Left(sFilepath.GetLength() - nIndex) + _T("theme");

			//todo theme path
			BinXlsxRW::SaveParams oSaveParams(sThemePath);
			OOX::Spreadsheet::CChartSpace oChartSpace;
			BinXlsxRW::BinaryChartReader oBinaryChartReader(oBufferedStream, oSaveParams, m_pExternalDrawingConverter);
			oBinaryChartReader.ReadCT_ChartSpace(lLength, &oChartSpace.m_oChartSpace);

			if(oChartSpace.isValid())
			{
				TCHAR tDrive[256];
				TCHAR tFolder[256];
				TCHAR tFilename[256];
				TCHAR tExt[256];
				_tsplitpath( sFilepath, tDrive, tFolder, tFilename, tExt );
				CString sDrive(tDrive);
				CString sFolder(tFolder);
				CString sFilename(tFilename);
				CString sExt(tExt);

				CString sRelsDir = sDrive + sFolder;
				sRelsDir.Append(_T("_rels"));
				if( !NSDirectory::Exists(string2std_string(sRelsDir)) )
					OOX::CSystemUtility::CreateDirectories(sRelsDir);

				oChartSpace.write2(sFilepath);

				CString sRelsPath;
				sRelsPath.Format(_T("%s\\%s.rels"), sRelsDir, sFilename + sExt);

				m_pExternalDrawingConverter->SaveDstContentRels(sRelsPath);

				CString sContentType(sContentTypePath);
				sContentType.Append(sFilename);
				sContentType.Append(sExt);

				(*sContentTypeElement) = new CString();
				(*sContentTypeElement)->Format(_T("<Override PartName=\"%s\" ContentType=\"application/vnd.openxmlformats-officedocument.drawingml.chart+xml\"/>"), sContentType);
				(*sContentTypeElement)->Append(oSaveParams.sAdditionalContentTypes);
				
				bRes = true;
			}
		}
		return bRes;
	}
	void CXlsxSerializer::setFontDir(CString& sFontDir)
	{
		m_sFontDir = sFontDir;
	}
	void CXlsxSerializer::setEmbeddedFontsDir(CString& sEmbeddedFontsDir)
	{
		m_sEmbeddedFontsDir = sEmbeddedFontsDir;
	}
	void CXlsxSerializer::setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter)
	{
		m_pExternalDrawingConverter = pDrawingConverter;
	}
};