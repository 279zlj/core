#pragma once
#include "Common.h"

namespace Writers
{
	class ContentWriter
	{
	public:		
		CStringWriter	m_oContent;
		CStringWriter	m_oSecPr;
	};
	class HdrFtrItem
	{
	public:
		HdrFtrItem(CString sDir)
		{
		}
		bool IsEmpty()
		{
			return m_sFilename.IsEmpty();
		}
		CString m_sFilename;
		ContentWriter Header;
		CString rId;
	};
	static CString g_string_hdr_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:hdr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\">");
	static CString g_string_hdr_End = _T("</w:hdr>");

	static CString g_string_ftr_Start = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:ftr xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\">");
	static CString g_string_ftr_End = _T("</w:ftr>");

	class HeaderFooterWriter 
	{
		CString	m_sDir;
		ContentTypesWriter& m_oContentTypesWriter;
	public:
		HdrFtrItem m_oHeaderFirst;
		HdrFtrItem m_oHeaderEven;
		HdrFtrItem m_oHeaderOdd;

		HdrFtrItem m_oFooterFirst;
		HdrFtrItem m_oFooterEven;
		HdrFtrItem m_oFooterOdd;
	public:
		HeaderFooterWriter(CString sDir, ContentTypesWriter& oContentTypesWriter):m_sDir(sDir),m_oContentTypesWriter(oContentTypesWriter),
			m_oHeaderFirst(sDir),m_oHeaderEven(sDir),m_oHeaderOdd(sDir),m_oFooterFirst(sDir),m_oFooterEven(sDir),m_oFooterOdd(sDir)
		{
		}
		void Write()
		{
			if(false == m_oHeaderFirst.IsEmpty())
			{
				WriteItem(_T("header"), m_oHeaderFirst.m_sFilename, m_oHeaderFirst.Header, true);
				//m_oHeaderFirst.RelsWriter.Write(sFilename + _T(".rels"));
			}
			if(false == m_oHeaderEven.IsEmpty())
			{
				WriteItem(_T("header"), m_oHeaderEven.m_sFilename, m_oHeaderEven.Header, true);
				//m_oHeaderEven.RelsWriter.Write(sFilename + _T(".rels"));
			}
			if(false == m_oHeaderOdd.IsEmpty())
			{
				WriteItem(_T("header"), m_oHeaderOdd.m_sFilename, m_oHeaderOdd.Header, true);
				//m_oHeaderOdd.RelsWriter.Write(sFilename + _T(".rels"));
			}

			if(false == m_oFooterFirst.IsEmpty())
			{
				WriteItem(_T("footer"), m_oFooterFirst.m_sFilename, m_oFooterFirst.Header, false);
				//m_oFooterFirst.RelsWriter.Write(sFilename + _T(".rels"));
			}
			if(false == m_oFooterEven.IsEmpty())
			{
				WriteItem(_T("footer"), m_oFooterEven.m_sFilename, m_oFooterEven.Header, false);
				//m_oFooterEven.RelsWriter.Write(sFilename + _T(".rels"));
			}
			if(false == m_oFooterOdd.IsEmpty())
			{
				WriteItem(_T("footer"), m_oFooterOdd.m_sFilename, m_oFooterOdd.Header, false);
				//m_oFooterOdd.RelsWriter.Write(sFilename + _T(".rels"));
			}
		}
		void WriteItem(CString sHeader, CString& sFilename, ContentWriter& m_oWriter, bool bHeader)
		{
			CFile oFile;
			oFile.CreateFile(m_sDir + _T("\\word\\" + sFilename));
			if(bHeader)
				oFile.WriteStringUTF8(g_string_hdr_Start);
			else
				oFile.WriteStringUTF8(g_string_ftr_Start);
			oFile.WriteStringUTF8(m_oWriter.m_oContent.GetData());
			if(bHeader)
				oFile.WriteStringUTF8(g_string_hdr_End);
			else
				oFile.WriteStringUTF8(g_string_ftr_End);
			oFile.CloseFile();

			//Content_Types
			m_oContentTypesWriter.AddOverride(_T("/word/") + sFilename, _T("application/vnd.openxmlformats-officedocument.wordprocessingml.")+sHeader+_T("+xml"));

			//Rels
			//return m_oDocumentRelsWriter.AddRels(_T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/") + sHeader, sFilename);
		}
	};
}