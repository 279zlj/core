﻿//#include "stdafx.h"

#include "./SmartArt.h"
#include "../Slide.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"

#include "../../ASCOfficeDrawingConverter.h"
#include "../../../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"

namespace PPTX
{
	namespace Logic
	{
		void SmartArt::LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			CString strDataPath = _T("");

			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

			if(id_data.IsInit())
			{
				if(parentFileIs<Slide>())
					strDataPath = parentFileAs<Slide>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideLayout>())
					strDataPath = parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideMaster>())
					strDataPath = parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<Theme>())
					strDataPath = parentFileAs<Theme>().GetMediaFullPathNameFromRId(*id_data);
				else if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*id_data);
					if (p.is_init())
						strDataPath = p->filename().m_strFilename;
				}
			}

			if (_T("") == strDataPath)
				return;

			nullable<PPTX::RId> id_drawing;

			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlFile2(strDataPath) == false) return;

			XmlUtils::CXmlNode oNode2 = oNode.ReadNode(_T("dgm:extLst"));
			if (oNode2.IsValid())
			{
				XmlUtils::CXmlNode oNode3 = oNode2.ReadNode(_T("a:ext"));
				if (oNode3.IsValid())
				{
					//https://msdn.microsoft.com/library/documentformat.openxml.office.drawing.datamodelextensionblock.aspx
					XmlUtils::CXmlNode oNode4 = oNode3.ReadNode(_T("dsp:dataModelExt"));
					if (oNode4.IsValid())
					{
						oNode4.ReadAttributeBase(L"relId", id_drawing);
					}
				}
			}

			if (id_drawing.is_init())
			{
				CString strDWPath = _T("");
				if(parentFileIs<Slide>())
					strDWPath = parentFileAs<Slide>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<SlideLayout>())
					strDWPath = parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<SlideMaster>())
					strDWPath = parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*id_drawing);
				else if(parentFileIs<Theme>())
					strDWPath = parentFileAs<Theme>().GetMediaFullPathNameFromRId(*id_drawing);
				else if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*id_drawing);
					if (p.is_init())
						strDWPath = p->filename().m_strFilename;
				}


				if (_T("") != strDWPath)
				{
					XmlUtils::CXmlNode oNodeDW;
					if (oNodeDW.FromXmlFile2(strDWPath))
					{
						XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(_T("spTree"));

						if (oNodeS.IsValid())
						{
							m_diag = oNodeS;

							CCommonRels* pRels = new CCommonRels();
							OOX::CPath filename = strDWPath;
							pRels->_read(filename);

							m_oCommonRels = pRels;

							return;
						}
					}
				}
			}
			//Monetizing_Innovation.pptx (слайд 13) - diagrams/data1.xml не ссылается на diagrams/drawing1.xml
			//пробуем по тому же пути с номером data.xml				
			// easy4cargo1.pptx - слайд 2 - в диаграмме Smart вместо ссылки на drawing.xml ссылка на стороннюю картинку


			OOX::CPath pathDiagramData = strDataPath;

			int a1 = pathDiagramData.GetFilename().Find(_T("."));
			CString strId = pathDiagramData.GetFilename().Mid(4,pathDiagramData.GetFilename().GetLength() - 8);
			OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + _T("drawing") + strId + _T(".xml");

			XmlUtils::CXmlNode oNodeDW;
			if (oNodeDW.FromXmlFile2(pathDiagramDrawing.GetPath()))//todooo ... сделать отдельно .. повтор
			{
				XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(_T("spTree"));

				if (oNodeS.IsValid())
				{
					m_diag = oNodeS;

					CCommonRels* pRels = new CCommonRels();
					pRels->_read(pathDiagramDrawing);

					m_oCommonRels = pRels;

					return;
				}
			}

		}

		void ChartRec::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			FileContainer* pRels = NULL;
			if (pWriter->m_pCommonRels->is_init())
				pRels = pWriter->m_pCommonRels->operator ->();

			CString strDataPath = _T("");
			if(id_data.IsInit())
			{
				if(parentFileIs<Slide>())
					strDataPath = parentFileAs<Slide>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideLayout>())
					strDataPath = parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<SlideMaster>())
					strDataPath = parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(*id_data);
				else if(parentFileIs<Theme>())
					strDataPath = parentFileAs<Theme>().GetMediaFullPathNameFromRId(*id_data);
				else if (pRels != NULL)
				{
					smart_ptr<PPTX::Image> p = pRels->image(*id_data);
					if (p.is_init())
						strDataPath = p->filename().m_strFilename;
				}
			}

			if (_T("") == strDataPath)
				return;

			BinXlsxRW::CXlsxSerializer oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter oDrawingConverter;
			NSBinPptxRW::CBinaryFileWriter* pOldWriter = oDrawingConverter.m_pBinaryWriter;
			oDrawingConverter.m_pBinaryWriter = pWriter;
			NSCommon::smart_ptr<PPTX::CCommonRels> pOldRels = *oDrawingConverter.m_pBinaryWriter->m_pCommonRels;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

			long lDataSize = 0;
			oXlsxSerializer.loadChart(strDataPath, *pWriter, lDataSize);
			*oDrawingConverter.m_pBinaryWriter->m_pCommonRels = pOldRels;
			oDrawingConverter.m_pBinaryWriter = pOldWriter;
		}

		void ChartRec::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!id_data.is_init() || NULL == m_bData)
				return;
			
			CString strData = _T("<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"") + id_data->ToString() + _T("\"/>");

			pWriter->WriteString(strData);
		}

		void ChartRec::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			ULONG lLen = pReader->GetLong();
			m_bData = true;

			m_lChartNumber = pReader->m_lChartNumber;
			pReader->m_lChartNumber++;
			int lId = pReader->m_pRels->WriteChart(m_lChartNumber, pReader->m_lDocumentType);

			BinXlsxRW::CXlsxSerializer oXlsxSerializer;
			NSBinPptxRW::CDrawingConverter oDrawingConverter;

			NSBinPptxRW::CImageManager2* pOldImageManager = oDrawingConverter.m_pImageManager;
			oDrawingConverter.m_pImageManager = pReader->m_pRels->m_pManager;
			NSBinPptxRW::CBinaryFileReader* pOldReader = oDrawingConverter.m_pReader;
			oDrawingConverter.m_pReader = pReader;

			oXlsxSerializer.setDrawingConverter(&oDrawingConverter);

			CString strDstChart = pReader->m_pRels->m_pManager->GetDstMedia();
			int nPos = strDstChart.ReverseFind(TCHAR('m'));
			if (-1 != nPos)
				strDstChart = strDstChart.Mid(0, nPos);

			strDstChart += _T("charts");
			if (1 == m_lChartNumber)
			{
                FileSystem::Directory::CreateDirectory(strDstChart);
			}
			CString strChart = _T("");
			strChart.Format(_T("chart%d.xml"), m_lChartNumber);

            strChart = strDstChart + FILE_SEPARATOR_STR + strChart;

			CString* sContentTypes = NULL;

            CString strWordChartFolder  = _T("/word/charts/");
            CString strXlChartFolder    = _T("/xl/charts/");
            CString strPptChartFolder   = _T("/ppt/charts/");

            if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_DOCX)
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strWordChartFolder, &sContentTypes, m_lChartNumber);
			else if (pReader->m_lDocumentType == XMLWRITER_DOC_TYPE_XLSX)
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strXlChartFolder, &sContentTypes, m_lChartNumber);
			else
                oXlsxSerializer.saveChart(*pReader, lLen, strChart, strPptChartFolder, &sContentTypes, m_lChartNumber);

			pReader->m_strContentTypes += (*sContentTypes);
			RELEASEOBJECT(sContentTypes);

			oDrawingConverter.m_pReader = pOldReader;
			oDrawingConverter.m_pImageManager = pOldImageManager;

			id_data = new PPTX::RId((size_t)lId);
		}
	} // namespace Logic
} // namespace PPTX
