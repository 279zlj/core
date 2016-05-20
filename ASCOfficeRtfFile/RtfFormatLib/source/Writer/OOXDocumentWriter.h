#pragma once

#include "../RtfDocument.h"
#include "OOXWriter.h"
#include "../../../../Common/FileWriter.h"

class OOXDocumentWriter
{
private: 
	OOXWriter& m_oWriter;
	RtfDocument& m_oDocument;
	NFileWriter::CBufferedFileWriter* m_oFileWriter;
	bool m_bFirst; //���� �������� ����� ������ ������ � ������
public: 
	OOXDocumentWriter( OOXWriter& oWriter,RtfDocument& oDocument ): m_oWriter(oWriter), m_oDocument(oDocument)
	{
		m_oFileWriter = NULL;
		m_bFirst = true;
	}
	~OOXDocumentWriter()
	{
		RELEASEOBJECT( m_oFileWriter );
	}
	CString CreateXmlStart();

	CString CreateXmlEnd( );

	bool SaveByItemStart( CString sFolder )
	{
		CString pathWord = sFolder + FILE_SEPARATOR_STR + _T("word");
		FileSystem::Directory::CreateDirectoryW(pathWord) ;
	
		try
		{
			CString sFilename = pathWord + FILE_SEPARATOR_STR + _T("document.xml");
			m_oFileWriter = new NFileWriter::CBufferedFileWriter( sFilename );
		}
		catch(...)
		{
			return false;
		}
		m_oWriter.m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"), _T("word/document.xml") );
		m_oWriter.m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"), _T("/word/document.xml") );

		CString sXml = CreateXmlStart( );
		CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
		m_oFileWriter->Write( (BYTE*)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
		return true;
	}
	bool SaveByItem()
	{
		if( true == m_bFirst )
			m_bFirst = false;
		else
		{
			RenderParameter oNewParam;
			oNewParam.poDocument = &m_oDocument;
			oNewParam.poWriter = &m_oWriter;
			oNewParam.poRels = &m_oWriter.m_oDocRels;
			oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

			if( m_oDocument.GetCount() > 1 && m_oDocument[1]->GetCount() > 0 )//���� ���-�� ���� � ��������� ������ ������ ���������� �����������
			{
				if( m_oDocument[0]->GetCount() > 0 )
				{
					CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
					//����� ��������� �������� ������
					CString sSectPr = m_oDocument[0]->m_oProperty.RenderToOOX(oNewParam);
					CString sFindStr = _T("</w:pPr>");
					int nIndexpPr = sXml.Find( sFindStr );
					if( -1 != nIndexpPr )
					{
						sXml.Insert( nIndexpPr, sSectPr );
					}
					else
					{
						sFindStr = _T("<w:p>");
						int nIndexP = sXml.Find( _T("<w:p>") );
						if( -1 != nIndexP )
							sXml.Insert( nIndexP + sFindStr.GetLength(), _T("<w:pPr>") + sSectPr + _T("</w:pPr>") );
					}
					//������� ������ ��������
					m_oDocument[0]->RemoveItem( 0 );
					CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
					m_oFileWriter->Write( (BYTE*)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
				}
				//������� ������
				m_oDocument.RemoveItem( 0 );
			}
			else if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//����� ��������
			{
				CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
				//������� ������ ��������
				m_oDocument[0]->RemoveItem( 0 );
				CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
				m_oFileWriter->Write( (BYTE*)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
			}
		}
		return true;
	}
	bool SaveByItemEnd()
	{
		RenderParameter oNewParam;
		oNewParam.poDocument = &m_oDocument;
		oNewParam.poWriter = &m_oWriter;
		oNewParam.poRels = &m_oWriter.m_oDocRels;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )//���������� ��������� ��������
		{
			CString sXml = m_oDocument[0]->operator[](0)->RenderToOOX(oNewParam);
			//������� ������ ��������
			m_oDocument[0]->RemoveItem( 0 );
			CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
			m_oFileWriter->Write( (BYTE*)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
		}
		CString sXml = CreateXmlEnd( );
		CStringA sXmlUTF = Convert::UnicodeToUtf8( sXml );
		m_oFileWriter->Write( (BYTE*)(LPCSTR)sXmlUTF, sXmlUTF.GetLength() );
		RELEASEOBJECT( m_oFileWriter );
		return true;
	}
	int GetCount()
	{
		int nCount = 0;
		for( int i = 0; i < m_oDocument.GetCount(); i++ )
			nCount += m_oDocument[i]->GetCount();
		return nCount;
	}
};