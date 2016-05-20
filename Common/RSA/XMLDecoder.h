#pragma once
#include "Includer.h"

class CXMLDecoder
{
public:
	CXMLDecoder()
		: m_pBlowfishKey(NULL)
		, m_nBlowfishKeySize(0)
	{
		m_oRSADecryptor.ImportPrivateKey(g_csPrivateKey);
	}
	virtual ~CXMLDecoder()
	{
		if (NULL!=m_pBlowfishKey)
			delete []m_pBlowfishKey;
	}

	BOOL SetBlowfishKey(IUnknown *punkKeyData)
	{
		RELEASEARRAYOBJECTS(m_pBlowfishKey);

		if (NULL != punkKeyData)
		{
			MediaCore::IAVSMediaData *pMediaData = NULL;
			punkKeyData->QueryInterface(MediaCore::IID_IAVSMediaData, (void**)&pMediaData);
			if  (NULL!=pMediaData)
			{
				LPBYTE pBuffer = NULL;
				pMediaData->get_Buffer(&pBuffer);
				long lDataSize = 0;
				pMediaData->get_DataSize(&lDataSize);
				if ((NULL != pBuffer) && (0 != lDataSize))
				{
					m_pBlowfishKey = NULL; 
					m_nBlowfishKeySize = 0;
					m_oRSADecryptor.Decrypt(pBuffer, lDataSize, m_pBlowfishKey, m_nBlowfishKeySize);
				}
				pMediaData->Release();
			}
		}
		return TRUE;
	}

	// ������������, ��� � ppData �������� ����� ��� dwDataLen ����, ����� ���������� ������ �������
	// � ppData ���������� ������ ����� ��� dwDataLen(���������� ��������)
	void DecryptXML(BYTE **ppData, DWORD *pdwDataLen)
	{
		if (NULL == m_pBlowfishKey)
			return;
		
		m_oBlowfish.Init(m_pBlowfishKey, (DWORD)m_nBlowfishKeySize);
		
		LPBYTE pszBase64	= (LPBYTE)*ppData;
		int nBase64Length	= (int)*pdwDataLen;

		LPBYTE pDecBuffer	= NULL;
		int nDecSize		= 0;

		if (!Decrypt(pszBase64, nBase64Length, pDecBuffer, nDecSize))
			return;
		delete []*ppData;
		*ppData = (BYTE*)pDecBuffer;
		*pdwDataLen = (DWORD)nDecSize;

		return;
	}

	CString DecryptXML(BSTR bstrXML)
	{
		if (NULL == m_pBlowfishKey)
			return CString(bstrXML);

		m_oBlowfish.Init(m_pBlowfishKey, (DWORD)m_nBlowfishKeySize);
		
		CStringA sXML = (CStringA)bstrXML;
		LPBYTE pszBase64	= (LPBYTE)sXML.GetBuffer();
		int nBase64Length	= (int)sXML.GetLength();

		LPBYTE pDecBuffer	= NULL;
		int nEncSize		= 0;

		if (!Decrypt(pszBase64, nBase64Length, pDecBuffer, nEncSize))
		{
			return _T("");
		}
		
		CString strRet = GetUnicodeString(pDecBuffer, nEncSize);
		delete []pDecBuffer;

		return strRet;
	}

	CStringW DecryptXMLW(BSTR bstrXML)
	{
		if (NULL == m_pBlowfishKey)
			return CStringW(bstrXML);

		return _DecryptXMLW((CStringA)bstrXML);
	}

protected:
	CStringW _DecryptXMLW(CStringA &sXML)
	{
		if (NULL == m_pBlowfishKey)
			return _T("");
		
		m_oBlowfish.Init(m_pBlowfishKey, (DWORD)m_nBlowfishKeySize);
		
		LPBYTE pszBase64	= (LPBYTE)sXML.GetBuffer();
		int nBase64Length	= (int)sXML.GetLength();

		LPBYTE pDecBuffer	= NULL;
		int nEncSize		= 0;

		if (!Decrypt(pszBase64, nBase64Length, pDecBuffer, nEncSize))
		{
			return _T("");
		}
		
		CStringW strRet((WCHAR*)pDecBuffer, nEncSize / sizeof(WCHAR));
		
		delete []pDecBuffer; 
		return strRet;
	}

	CString _DecryptXML(CStringA &sXML)
	{
		if (NULL == m_pBlowfishKey)
			return _T("");
		
		m_oBlowfish.Init(m_pBlowfishKey, (DWORD)m_nBlowfishKeySize);
		
		LPBYTE pszBase64	= (LPBYTE)sXML.GetBuffer();
		int nBase64Length	= (int)sXML.GetLength();

		LPBYTE pDecBuffer	= NULL;
		int nEncSize		= 0;

		if (!Decrypt(pszBase64, nBase64Length, pDecBuffer, nEncSize))
		{
			return _T("");
		}
		
		CString strRet((TCHAR*)pDecBuffer, nEncSize / sizeof(TCHAR));
		
		delete []pDecBuffer; 
		return strRet;
	}

protected:
	RSA::CRSADecryptor m_oRSADecryptor;

	LPBYTE m_pBlowfishKey;
	size_t m_nBlowfishKeySize;
	CBlowfish m_oBlowfish;

protected:
	
	BOOL Decrypt(BYTE* pszBase64, int nBase64Length, BYTE*& pDecBuffer, int& nEncSize)
	{
		pDecBuffer		= NULL;
		nEncSize		= 0;
		
		Base64Decode((LPCSTR)pszBase64, nBase64Length, NULL, &nEncSize);
		
		LPBYTE pEncBuffer = new BYTE[nEncSize];
		if (NULL == pEncBuffer)
			return FALSE;
		
		if (!Base64Decode((LPCSTR)pszBase64, nBase64Length, pEncBuffer, &nEncSize))
		{
			delete []pEncBuffer;
			return FALSE;
		}

		pDecBuffer = new BYTE[nEncSize + 1];
		if (NULL == pDecBuffer)
		{
			delete []pEncBuffer;
			return FALSE;
		}
		m_oBlowfish.Decrypt(pEncBuffer, (LPBYTE)pDecBuffer, nEncSize);
	
		pDecBuffer[nEncSize] = '\0';		
		delete [] pEncBuffer;

		return TRUE;
	}

	CString GetUnicodeString(BYTE* pBuffer, int nSize)
	{
		CStringA strFind(pBuffer);

		int nStart	= strFind.Find("<?xml");
		int nEnd	= strFind.Find("?>");

		if ((-1 == nStart) || (-1 == nEnd))
		{
			CString strRet((TCHAR*)pBuffer, nSize / sizeof(TCHAR));
			return strRet;
		}
		
		CString sXMLNode = (CString)strFind.Mid (nStart, nEnd - nStart + 2);	// strlen("?>") = 2
		sXMLNode.MakeLower();

		CHAR* pSrc = (CHAR*)(pBuffer + nEnd + 2);
		BOOL bIsUTF8 = (-1 != sXMLNode.Find(_T("utf-8")));
		if (bIsUTF8)
		{
			int nSize = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, NULL, 0);

			CStringW strW;
			MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, strW.GetBuffer(nSize + 1), nSize);
			strW.ReleaseBuffer();
			return strW;
		}

		CString strRet((TCHAR)pBuffer, nSize / sizeof(TCHAR));
		return strRet;
	}

};