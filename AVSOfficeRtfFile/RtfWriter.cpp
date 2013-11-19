#include "stdafx.h"
#include "RtfWriter.h"
#include "RtfDocument.h"
#include "../Common/OfficeFileTemplate.h"

bool RtfWriter::Save()
{
	int nItemsCount = GetCount();
	SaveByItemStart();
	for( int i = 0; i < nItemsCount; i++ )
	{
		SaveByItem();
		short shCancel = c_shProgressContinue;
		OnProgress( g_cdMaxPercent * i / nItemsCount, &shCancel );
		if( c_shProgressCancel == shCancel )
			break;
	}
	return SaveByItemEnd();

}
bool RtfWriter::SaveByItemStart()
{
	//try{
	//	m_sTempFileResult = Utils::CreateTempFile( m_sTempFolder );
	//	BSTR bstrFileName = m_sTempFileResult.AllocSysString();
	//	m_oFileWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
	//	SysFreeString( bstrFileName );
	//	//������� �������� ���� ���� ����� ���������( ����� �������� �������� ������ � ���������� ����� )
	//	m_sTempFile = Utils::CreateTempFile( m_sTempFolder );
	//	BSTR bstrTempFileName = m_sTempFile.AllocSysString();
	//	m_oTempFileWriter = new NFileWriter::CBufferedFileWriter( bstrTempFileName );
	//	SysFreeString( bstrTempFileName );
	//}
	//catch(...)
	//{
	//	return false;
	//}
	//CStringA sRtf;
	//sRtf = CreateRtfStart();
	//m_oFileWriter->Write( (LPBYTE)(LPCSTR)sRtf, sRtf.GetLength() );
	m_bFirst = true;
	return true;
}
bool RtfWriter::SaveByItem()
{
	if( NULL == m_oCurTempFileWriter )
	{
		try
		{
			CString sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFiles.Add( sNewTempFile );

			RELEASEOBJECT( m_oCurTempFileWriter );
			BSTR bstrFileName = sNewTempFile.AllocSysString();
			m_oCurTempFileWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
			SysFreeString( bstrFileName );

			CString sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFilesSectPr.Add( sNewTempFileSect );

			RELEASEOBJECT( m_oCurTempFileSectWriter );
			BSTR bstrFileNameSect = sNewTempFileSect.AllocSysString();
			m_oCurTempFileSectWriter = new NFileWriter::CBufferedFileWriter( bstrFileNameSect );
			SysFreeString( bstrFileNameSect );
		}
		catch(...)
		{
			return false;
		}
	}
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = this;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	if( m_oDocument.GetCount() > 1 && m_oDocument[0]->GetCount() == 0 )
	{
		//����� ����� ������
		CStringA sRtfExt = "\\sect";
		m_oCurTempFileWriter->Write( (LPBYTE)(LPCSTR)sRtfExt, sRtfExt.GetLength() );
		//������������ ���������� �������� ����
		RELEASEOBJECT( m_oCurTempFileWriter )
		try
		{
			CString sNewTempFile = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFiles.Add( sNewTempFile );

			BSTR bstrFileName = sNewTempFile.AllocSysString();
			m_oCurTempFileWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
			SysFreeString( bstrFileName );
		}
		catch(...)
		{
			return false;
		}
		if( NULL != m_oCurTempFileSectWriter )
		{
			//����� �������� ������
			CString sRtf;
			if( true == m_bFirst )
			{
				//������ �������� ������ ����� ��� �������� ���������
				m_bFirst = false;
				oNewParam.nType = RENDER_TO_OOX_PARAM_FIRST_SECTION;
			}
			sRtf = m_oDocument[0]->m_oProperty.RenderToRtf(oNewParam);
			RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//���������� � ����
			RELEASEOBJECT( m_oCurTempFileSectWriter );
			//������� �����
			CString sNewTempFileSect = Utils::CreateTempFile( m_sTempFolder );
			m_aTempFilesSectPr.Add( sNewTempFileSect );

			BSTR bstrFileName = sNewTempFileSect.AllocSysString();
			m_oCurTempFileSectWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
			SysFreeString( bstrFileName );

			//m_aTempFilesSectPr.Add( sRtf );
			//RtfInternalEncoder::Decode( sRtf, *m_oFileWriter );
			//m_oFileWriter->Write( (LPBYTE)(LPCSTR)sRtf, sRtf.GetLength() );
		}
		//������� ������
		m_oDocument.RemoveItem( 0 );

		////�������� ��������� �� ��������� �����
		//HANDLE hFile = ::CreateFile(m_sTempFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
		//DWORD dwBytesRead = 1;
		//BYTE byteBuffer[ 4096 ];

		//while( 0 != dwBytesRead )
		//{
		//	::ReadFile(hFile,byteBuffer ,4096, &dwBytesRead, NULL);
		//	m_oFileWriter->Write( (LPBYTE)&byteBuffer, dwBytesRead );
		//}
		//CloseHandle( hFile );
		////������ ����� ������
		//CStringA sRtfExt = "\\sect";
		//m_oFileWriter->Write( (LPBYTE)(LPCSTR)sRtfExt, sRtfExt.GetLength() );

		////������� �������� ����
		//BSTR bstrFileName = m_sTempFile.AllocSysString();
		//m_oTempFileWriter = new NFileWriter::CBufferedFileWriter( bstrFileName );
		//SysFreeString( bstrFileName );
	}
	//����� ��������
	if( m_oDocument.GetCount() > 0 && m_oDocument[0]->GetCount() > 0 )
	{
		CString sRtf;
		sRtf = m_oDocument[0]->operator[](0)->RenderToRtf(oNewParam);
		if( TYPE_RTF_PARAGRAPH == m_oDocument[0]->operator[](0)->GetType() && !(m_oDocument[0]->GetCount() == 0 && m_oDocument.GetCount() > 1) )//��� ���������� ��������� ������ �� ����� \par
		{
			sRtf.Append( _T("\\par") );
			//oNewParam.nValue = RENDER_TO_RTF_PARAM_NO_PAR;
		}
		RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
		//m_oTempFileWriter->Write( (LPBYTE)(LPCSTR)sRtf, sRtf.GetLength() );

		//������� ������� ������� ������ ��� ��������
		m_oDocument[0]->RemoveItem( 0 );
	}
	return true;
}
bool RtfWriter::SaveByItemEnd()
{
	//������������ ���������� �������� ����
	RELEASEOBJECT( m_oCurTempFileWriter );

	CString sRtf;
	if( m_oDocument.GetCount() > 0 )
	{
		RenderParameter oNewParam;
		oNewParam.poDocument = &m_oDocument;
		oNewParam.poWriter = this;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		if( NULL != m_oCurTempFileSectWriter )
		{
			//����� ��������� ������
			if( true == m_bFirst )
			{
				//������ �������� ������ ����� ��� �������� ���������
				m_bFirst = false;
				oNewParam.nType = RENDER_TO_OOX_PARAM_FIRST_SECTION;
			}
			sRtf = m_oDocument[0]->m_oProperty.RenderToRtf(oNewParam);
			RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileSectWriter );
			//���������� � ����
			RELEASEOBJECT( m_oCurTempFileSectWriter );
		}
		//RtfInternalEncoder::Decode( sRtf, *m_oCurTempFileWriter );
		//m_oFileWriter->Write( (LPBYTE)(LPCSTR)sRtf, sRtf.GetLength() );

		//������� ������
		m_oDocument.RemoveItem( 0 );
	}

	////�������� ��������� �� ��������� �����
	//HANDLE hFile = ::CreateFile(m_sTempFile, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
	//DWORD dwBytesRead = 1;
	//BYTE byteBuffer[ 4096 ];

	//while( 0 != dwBytesRead )
	//{
	//	::ReadFile(hFile,byteBuffer ,4096, &dwBytesRead, NULL);
	//	m_oFileWriter->Write( (LPBYTE)&byteBuffer, dwBytesRead );
	//}
	//CloseHandle( hFile );

	////��������� ��������
	//sRtf = CreateRtfEnd();
	//RtfInternalEncoder::Decode( sRtf, *m_oFileWriter );
	////m_oFileWriter->Write( (LPBYTE)(LPCSTR)sRtf, sRtf.GetLength() );
	//RELEASEOBJECT( m_oFileWriter );
	//��������� �������� ����
	try
	{
		BSTR bstrTargetFilename = m_sFilename.AllocSysString();
		NFileWriter::CBufferedFileWriter oTargetFileWriter( bstrTargetFilename );
		SysFreeString( bstrTargetFilename );

		//����� ��������� ����� ��� ���������
		sRtf = CreateRtfStart();
		DWORD dwBytesWrite = 0;
		RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
		//WriteFile ( hTargetFile, sRtf, ( DWORD ) sRtf.GetLength(), &dwBytesWrite, NULL );

		//�������� ��������� �� ������� � ��������� �� �������� ������
		for( int i = 0 ; i < (int)m_aTempFiles.GetCount() && i < (int)m_aTempFilesSectPr.GetCount(); i++ )
		{
			//�������� ������
			HANDLE hFile = ::CreateFile( m_aTempFilesSectPr[i], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
			if( INVALID_HANDLE_VALUE != hFile )
			{
				DWORD dwBytesRead = 1;
				BYTE byteBuffer[ 4096 ];

				while( 0 != dwBytesRead )
				{
					::ReadFile( hFile, byteBuffer, 4096, &dwBytesRead, NULL);
					oTargetFileWriter.Write( byteBuffer,  dwBytesRead );
				}
				CloseHandle( hFile );
			}
			//���������
			hFile = ::CreateFile( m_aTempFiles[i], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
			if( INVALID_HANDLE_VALUE != hFile )
			{
				DWORD dwBytesRead = 1;
				BYTE byteBuffer[ 4096 ];

				while( 0 != dwBytesRead )
				{
					::ReadFile( hFile, byteBuffer, 4096, &dwBytesRead, NULL);
					oTargetFileWriter.Write( byteBuffer,  dwBytesRead );
				}
				CloseHandle( hFile );
			}
		}

		//��������� ��������
		sRtf = CreateRtfEnd();
		RtfInternalEncoder::Decode( sRtf, oTargetFileWriter );
		byte nEndFile = 0;
		oTargetFileWriter.Write( &nEndFile, 1);
	}
	catch(...)
	{}

	for( int i = 0; i < (int)m_aTempFiles.GetCount(); i++ )
		Utils::RemoveDirOrFile( m_aTempFiles[i] );
	m_aTempFiles.RemoveAll();
	for( int i = 0; i < (int)m_aTempFilesSectPr.GetCount(); i++ )
		Utils::RemoveDirOrFile( m_aTempFilesSectPr[i] );
	m_aTempFilesSectPr.RemoveAll();
	return true;
}
int RtfWriter::GetCount()
{
	int nCount = 0;
	for( int i = 0; i < m_oDocument.GetCount(); i++ )
		nCount += m_oDocument[i]->GetCount();
	return nCount;
}
CString RtfWriter::CreateRtfStart()
{
	RenderParameter oRenderParameter;
	oRenderParameter.poDocument = &m_oDocument;
	oRenderParameter.poWriter = this;
	oRenderParameter.nType = RENDER_TO_RTF_PARAM_UNKNOWN;

	CString sResult;
	sResult.Append( _T("{\\rtf1\\ulc1") );
	sResult.Append( m_oDocument.m_oProperty.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oFontTable.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oColorTable.RenderToRtf( oRenderParameter ) );
	//CString sDefCharProp = m_oDocument.m_oDefaultCharProp.RenderToRtf( oRenderParameter );
	//if( false == sDefCharProp.IsEmpty() )
	//	sResult.AppendFormat( _T("{\\*\\defchp %s}"), sDefCharProp);
	//CString sDefParProp = m_oDocument.m_oDefaultParagraphProp.RenderToRtf( oRenderParameter );
	//if( false == sDefParProp.IsEmpty() )
	//	sResult.AppendFormat( _T("{\\*\\defpap %s}"),sDefParProp );
	//sResult.Append( m_oDocument.m_oStyleTable.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oListTabel.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oListOverrideTabel.RenderToRtf( oRenderParameter ) );
	sResult.Append( m_oDocument.m_oInformation.RenderToRtf( oRenderParameter ) );
	sResult.Append( _T("\\fet2") );//0	Footnotes only or nothing at all (the default). 1 Endnotes only. 2	Both footnotes and endnotes
	CString sFootnote;
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		sFootnote = m_oDocument.m_oFootnoteSep->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\ftnsep %s}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		sFootnote = m_oDocument.m_oFootnoteCon->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\ftnsepc %s}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		sFootnote = m_oDocument.m_oEndnoteSep->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\aftnsep %s}"), sFootnote );
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		sFootnote = m_oDocument.m_oEndnoteCon->RenderToRtf( oRenderParameter );
		if( _T("") != sFootnote )
			sResult.AppendFormat( _T("{\\*\\aftnsepc %s}"), sFootnote );
	}

	sResult.Append(_T("\n\n"));
	return sResult;
}

CString RtfWriter::CreateRtfEnd( )
{
	return _T("\n}\n");
}
