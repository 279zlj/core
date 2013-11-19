// AVSDocumentConverter.cpp : Implementation of CAVSOfficeUniversalConverter

#include "stdafx.h"
#include "AVSDocumentConverter.h"
#include "StringUtils.h"
#include "DocumentConverterManager.h"
#include "DecodingResources.h"
#include "AVSXMLMultiDocumentSource.h"
#include "AVSDocumentMenu.h"

#ifdef VALIDATE_INPUT_XML
#import <msxml6.dll>
#endif

// CAVSOfficeUniversalConverter
STDMETHODIMP CAVSOfficeUniversalConverter::Start(BSTR bstrXMLString)
{
	CTemporaryCS oCS(&m_csThread);
	m_bStop = false;
	CString strXml = m_oDecoder.DecryptXML( bstrXMLString );

	#ifdef VALIDATE_INPUT_XML
	BSTR bstrInputXml = strXml.AllocSysString();
	HRESULT hValidateRes = ValidateDocumentConverterXML( bstrInputXml );
	SysFreeString( bstrInputXml );
	if( S_OK != hValidateRes )
		return AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_FIRST;
	#endif

	//��� ��������� ������ ��������� ������������� ���������� �����
	if( DOCUMENTCONVERTER_STATE_IDLE != m_oConvertManager.GetStatus() )
		Stop();

	//���� �� ������ ���������� ������� ���������� �� ���������� �����������
	if( m_oConvertManager.GetTreadCount() <= 0 )
		SetThreadType(-1);

	INT32 nError = -1;
	//������ xml
	XmlUtils::CXmlReader oReader;
	if( TRUE == oReader.OpenFromXmlString( strXml ))
		if( TRUE == oReader.ReadRootNode( CString(_T("DocumentConverter")) ))
		{
			if( TRUE == oReader.ReadNodeList( CString(_T("Document")) ) )
			{
				nError = 0;
				m_lLastProcess = -1;
				//�������� ����������
				m_oConvertManager.Clear();
				//������ Event ��� ������ �������
				m_oConvertManager.SetEventSource( m_hNeedEvent );
				//������ �������� ����������
				m_oConvertManager.SetTempDirectory( m_sTempDirectory );
				//������ cache
				m_oConvertManager.SetCache( m_piFilesCache );
				//��������� ����������� ������
				nError =  m_oConvertManager.AddDocuments( oReader );
				if( 0 == nError )
					m_oConvertManager.Start();
			}
		}
	if( -1 == nError)
	{
		OnError( AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_XML_FORMAT, -1 );
		return AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_XML_FORMAT;
	}
	return S_OK;
}
STDMETHODIMP CAVSOfficeUniversalConverter::_OnEvent(void)
{
	//�������� ��� ��������� ���������
	m_oConvertManager.Lock();
	long nNewProcess = m_oConvertManager.GetProcess();
	CAtlArray< int > aFilesStarted;
	bool bFileStart = m_oConvertManager.GetFilesStarted( &aFilesStarted );
	CAtlArray< FileAndProcess > aFilesProcess;
	bool bFileProcess = m_oConvertManager.GetFilesProcess( &aFilesProcess ) ;
	CAtlArray< int > aFilesComplete;
	bool bFileComplete = m_oConvertManager.GetFilesComplete( &aFilesComplete );
	CAtlArray< CError > aErrors;
	bool bErrors = m_oConvertManager.GetErrors( &aErrors );
	//CAtlArray<CError> aErrorsNoCritical;
	//bool bErrorsNoCritical = m_oConvertManager.GetErrorsNoCritical( &aErrorsNoCritical );
	bool bComplete = m_oConvertManager.GetIsComplete();
	m_oConvertManager.unLock();

  if( false == m_bStop )
  {
	//��������� ������ ����������� ������
	if( true == bFileStart ) 
	{
		for(INT32 i=0; i < (INT32)aFilesStarted.GetCount(); i++)
		{
			OnDestinationFileStart( aFilesStarted[i] );
		}
	}

	//��������� �������� ������
	if( true == bFileProcess  )
	{
		for( INT32 i=0; i < (INT32)aFilesProcess.GetCount(); i++ )
		{
			FileAndProcess oCurItem = aFilesProcess[i];
			OnProcessFile( oCurItem.lProcess, oCurItem.nId );
		}
	}
	//��������� ��������� ����������� ������
	if( true == bFileComplete  )
	{
		for( INT32 i = 0; i < (INT32)aFilesComplete.GetCount(); i++)
		{
			OnDestinationFileComplete( aFilesComplete[i] );
		}
	}

	//������� �������� ������� �� ������������ �� ������� Event
	if( m_lLastProcess < nNewProcess  && nNewProcess < c_nMaxProgressPercent )
	{
		m_lLastProcess = nNewProcess;
		OnProcess( nNewProcess );
	}

	////��������� ������������� ������
	//if( true == bErrorsNoCritical  )
	//{
	//	for( INT32 i=0; i < (INT32)aErrorsNoCritical.GetCount(); i++)
	//	{
	//		CString sError = aErrorsNoCritical[i].ToString();
	//		BSTR bstrError = sError.AllocSysString();
	//		OnWarning(aErrorsNoCritical[i].GetID(), bstrError);
	//		SysFreeString( bstrError );
	//	}
	//}
	//��������� �� ��������� �� ������
	if( true == bErrors  )
	{
		bool bNeedStop = false;
		for( INT32 i = 0; i < (INT32)aErrors.GetCount(); i++)
		{
			OnError( aErrors[i].GetID(), aErrors[i].GetFileID() );
			SHORT nCancel; 
			BSTR bstrDescription = aErrors[i].GetDescription().AllocSysString();
			OnErrorWide( bstrDescription, &nCancel );
			SysFreeString( bstrDescription );
			if( 1 == nCancel )
			{
				Stop();
				return S_OK;
			}
		}
		Resume();
	}
  }
	if( true == bComplete )
	{
		//���� ��� ��������� ���������� OnComplete � ������������� ������
		m_oConvertManager.Clear();
		OnProcess( c_nMaxProgressPercent );
		OnComplete();
	}
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::Stop(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_bStop = true;
	if( DOCUMENTCONVERTER_STATE_IDLE != m_oConvertManager.GetStatus() )
	{
		Resume();
		if( FALSE == m_oConvertManager.Stop())
			return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::Suspend(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.Suspend();
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::Resume(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.Resume();
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::get_Status(LONG* pVal)
{
	CTemporaryCS oCS(&m_csThread);

	if( NULL == pVal )
		return S_FALSE;

	(*pVal) = m_oConvertManager.GetStatus();
	return S_OK;
}
STDMETHODIMP CAVSOfficeUniversalConverter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName; sParamName = ParamName;
	if (g_csBlowfishKeyParamName == sParamName)
	{		
		if (!m_oDecoder.SetBlowfishKey(ParamValue.punkVal))
			return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::GetEstimatedTime(DOUBLE* pVal)
{
	CTemporaryCS oCS(&m_csThread);

	if(NULL != pVal)
	{
		(*pVal) = m_oConvertManager.GetEstimatedTime();
		return S_OK;
	}
	else
		return S_FALSE;
}
STDMETHODIMP CAVSOfficeUniversalConverter::GetElapsedTime(DOUBLE* pVal)
{
	CTemporaryCS oCS(&m_csThread);
	if(NULL != pVal)
	{
		(*pVal) = m_oConvertManager.GetElapsedTime();
		return S_OK;
	}
	else
		return S_FALSE;
}

STDMETHODIMP CAVSOfficeUniversalConverter::SetThreadType(LONG NewVal)
{
	CTemporaryCS oCS(&m_csThread);
	SYSTEM_INFO siSysInfo;
	DWORD dwProcNum;
	INT32 nThreadCount;
	switch( NewVal )
	{
		case -1:
				GetSystemInfo( &siSysInfo ); 
				nThreadCount = 3 * siSysInfo.dwNumberOfProcessors / 2;
				break;
		case 0:
				nThreadCount = 1;
				break;
		case 1:
				nThreadCount = 2;
				GetSystemInfo( &siSysInfo ); 
				dwProcNum = 3 * siSysInfo.dwNumberOfProcessors / 2;
				if( (INT32)dwProcNum > nThreadCount)
					nThreadCount = dwProcNum;
				break;
		default:
				GetSystemInfo( &siSysInfo ); 
				nThreadCount = 3 * siSysInfo.dwNumberOfProcessors / 2;
				break;
	}
	m_oConvertManager.SetTreadCount( nThreadCount );
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::SetThreadPriority(LONG NewVal)
{
	CTemporaryCS oCS(&m_csThread);
	m_oConvertManager.SetThreadPriority( NewVal );
	return S_OK;
}
STDMETHODIMP CAVSOfficeUniversalConverter::get_OfficeFilesCache(IUnknown** ppunkVal)
{
	CTemporaryCS oCS(&m_csThread);
	if( NULL == ppunkVal )
		return S_FALSE;
	(*ppunkVal) = NULL;
	if( NULL == m_piFilesCache )
		return S_OK;

	return m_piFilesCache->QueryInterface( IID_IUnknown , (void**)ppunkVal );
}
STDMETHODIMP CAVSOfficeUniversalConverter::put_OfficeFilesCache(IUnknown* punkVal)
{
	CTemporaryCS oCS(&m_csThread);

	RELEASEINTERFACE(m_piFilesCache);
	if(NULL != punkVal)
	{
		IAVSOfficeFilesCache* piFileCache = NULL;
		HRESULT hRes = punkVal->QueryInterface( __uuidof(IAVSOfficeFilesCache), (void**)&piFileCache );
		if( SUCCEEDED(hRes) &&  NULL != piFileCache )
			m_piFilesCache = piFileCache;
		else
			return S_FALSE;
	}
	m_oConvertManager.SetCache( m_piFilesCache );
	return S_OK;
}

STDMETHODIMP CAVSOfficeUniversalConverter::get_TempDirectory( BSTR* pbstrDir )
{
	CTemporaryCS oCS(&m_csThread);
	if( NULL == pbstrDir )
		return S_FALSE;
	(*pbstrDir) = m_sTempDirectory.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSOfficeUniversalConverter::put_TempDirectory( BSTR bstrDir )
{
	CTemporaryCS oCS(&m_csThread);
	m_sTempDirectory = CString( bstrDir );
	return S_OK;
}
#ifdef VALIDATE_INPUT_XML
// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)
STDMETHODIMP CAVSOfficeUniversalConverter::ValidateDocumentConverterXML(BSTR bstrXMLString)
{
    // Declare and initialize variables
    MSXML2::IXMLDOMSchemaCollectionPtr   pXS;
	MSXML2::IXMLDOMDocument2Ptr			 pXSD;  
    MSXML2::IXMLDOMDocument2Ptr          pXD;
    MSXML2::IXMLDOMParseErrorPtr         pErr;
    _bstr_t bstrResult = L"";
    HRESULT hr = S_OK;
	CString strXSD;

    // Create a schema cache and add DocumentConverter.xsd to it.
    CHK_HR(pXS.CreateInstance(__uuidof(MSXML2::XMLSchemaCache60), NULL, CLSCTX_INPROC_SERVER));
	CHK_HR(pXSD.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER));
	BSTR textXML = readFileFromRresource(IDR_CRPT1);	
	if (pXSD->loadXML(textXML) == VARIANT_FALSE)
	{
		SysFreeString( textXML );
		return S_FALSE;
	}
	SysFreeString( textXML );
	CHK_HR(pXS->add("", pXSD.GetInterfacePtr()));

    // Create a DOMDocument and set its properties.
    CHK_HR(pXD.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER));

    // Assign the schema cache to the DOMDocument's schemas collection.
    pXD->schemas = pXS.GetInterfacePtr();

    // Load bstrFile into the DOM document.
    pXD->async = VARIANT_FALSE;
    pXD->validateOnParse = VARIANT_TRUE;
    pXD->resolveExternals = VARIANT_TRUE;

    if(pXD->loadXML(bstrXMLString) != VARIANT_TRUE)
    {
        pErr = pXD->parseError;

        bstrResult = _bstr_t(L"Validation failed on ") +
            _bstr_t(L"\n=====================") +
            _bstr_t(L"\nReason: ") + _bstr_t(pErr->Getreason()) +
            _bstr_t(L"\nSource: ") + _bstr_t(pErr->GetsrcText()) +
            _bstr_t(L"\nLine: ") + _bstr_t(pErr->Getline()) +
            _bstr_t(L"\n");

		hr = S_FALSE;
    }

CleanUp:
    return hr;
}
#endif
void CAVSOfficeUniversalConverter::StopAndWait(void)
{
	CTemporaryCS oCS(&m_csThread);
	m_bStop = true;
	if( DOCUMENTCONVERTER_STATE_IDLE != m_oConvertManager.GetStatus() )
	{
		Resume();
		m_oConvertManager.Stop( true );
	}
}
