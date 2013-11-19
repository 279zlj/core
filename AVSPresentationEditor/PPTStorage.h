#pragma once

#include "ShareMemArray.h"

namespace NSPresentationEditor
{
	// �������������� ��������
	#define ISID_DEFAULT			0x0000	// �� ���������
	#define ISID_PPTSTATUSSTORAGE	0x0001	// ������� ��� �������
	#define ISID_PPTINFOSTORAGE		0x0002	// ������� ��� ����������

	static long AVS_OFFICESTUDIO_FILE_UNKNOWN					= 0x0000;
	static long AVS_OFFICESTUDIO_FILE_PRESENTATION				= 0x0080;
	static long AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX			= 0x0081;
	static long AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT			= 0x0082;
	static long AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP			= 0x0083;


	namespace NSStrings
	{
		static CString ConvertCStringWToCString(CStringW& strW)
		{
			BSTR bstr = strW.AllocSysString();
			CString str(bstr);
			SysFreeString(bstr);

			return str;
		}

		static CStringW GetCStringWFromUTF8( BYTE* pBuffer, LONG64 lCount )
		{
			LONG64 lLenght = 0;
			CStringW strRes = L"";
			for (LONG lIndex = 0; lIndex < lCount; ++lIndex)
			{
				if (0x00 == (0x80 & pBuffer[lIndex]))
				{
					strRes += (WCHAR)pBuffer[lIndex];
					continue;
				}
				else if (0x00 == (0x20 & pBuffer[lIndex]))
				{
					WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x1F) << 6) + (pBuffer[lIndex + 1] & 0x3F));
					strRes += mem;

					lIndex += 1;
				}
				else if (0x00 == (0x10 & pBuffer[lIndex]))
				{
					WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x0F) << 12) + ((pBuffer[lIndex + 1] & 0x3F) << 6) + (pBuffer[lIndex + 2] & 0x3F));
					strRes += mem;

					lIndex += 2;
				}
			}
			return strRes;
		}
		
		static CString GetCStringFromUTF8( BYTE* pBuffer, LONG64 lCount )
		{
			CStringW strRes = GetCStringWFromUTF8(pBuffer, lCount);
			return ConvertCStringWToCString(strRes);
		}
	};

	// ������ ������� �������� PPT-�����
	enum PPTStatus {STIF_ERROR, STIF_AVAILABLE, STIF_CREATING, STIF_BROKEN};

	enum StringCoding 
	{	
		scASCII		= 0, 
		scUNICODE	= 1, 
		scUTF8		= 2
	};

	// ��������� ��� ����������� ����� � ������� ������� ���������
	struct SPPTStatusStorage
	{
		PPTStatus	m_sStatus;					// ������

		// ������ ��� ����������� ������������ � ��������� ����������
		LONG		m_nProgress;				// �������� ����������
		LONG64		m_lLength;					// ������ �������

		SPPTStatusStorage() 
		{
			Reset();
		}

		void Reset()
		{
			// ����� ������
			m_sStatus = STIF_ERROR;				// ������

			// ������ ��� ����������� ������������ � ��������� ����������
			m_nProgress		= 0;
			m_lLength		= 0;				// ������ �������
		}
	};

	struct SPPTInfoStorage
	{
		LONG		m_lCount;
		
		CString		m_strTempDirectory;
		CString		m_strXml;				// ��� ����� ��� �����������

		SPPTInfoStorage() 
		{
			Reset();
		}

		~SPPTInfoStorage()
		{
		}

		void Reset()
		{
			m_lCount			= 0;

			m_strTempDirectory	= _T("");
			m_strXml			= _T("");
		}

		CString SetXml(BYTE* pBuffer, LONG lBytesCount)
		{
			CString strXml((WCHAR*)pBuffer, lBytesCount / sizeof(WCHAR));
			return strXml;
		}
		void Save(BYTE* pBuffer)
		{
			if (NULL == pBuffer)
				return;

			BYTE* pData = pBuffer;
			
			*((LONG*)pData) = m_lCount;
			pData += sizeof(LONG);

			LONG lLenTemp = m_strTempDirectory.GetLength();

			*((LONG*)pData) = lLenTemp;
			pData += sizeof(LONG);

			size_t lSizeTemp = ((size_t)lLenTemp) * sizeof(WCHAR);
			memcpy(pData, m_strTempDirectory.GetBuffer(), lSizeTemp);
			pData += lSizeTemp;

			memcpy(pData, (BYTE*)m_strXml.GetBuffer(), m_strXml.GetLength() * sizeof(WCHAR));
		}

		LONG64 GetLength()
		{
			LONG64 lRes = 2 * sizeof(LONG);
			lRes += (m_strTempDirectory.GetLength() * sizeof(WCHAR));
			lRes += (m_strXml.GetLength() * sizeof(WCHAR));
			return lRes;
		}

		void Load(BYTE* pBuffer, LONG64 lLength)
		{
			BYTE* pData = pBuffer;

			m_lCount = *((LONG*)pData);
			pData += sizeof(LONG);

			LONG lLenTemp = *((LONG*)pData);
			pData += sizeof(LONG);

			CStringW strTemp((WCHAR*)pData, lLenTemp);
			m_strTempDirectory = strTemp;
			pData += lLenTemp * sizeof(WCHAR);

			LONG lXmlSize = (LONG)(lLength - (pData - pBuffer));
			CStringW strW((WCHAR*)pData, lXmlSize / sizeof(WCHAR));

			m_strXml = strW;
		}
	};

	// ����� ��������� ������� �������
	class CStatusStorage: public CShareMemArray<BYTE>
	{
	protected:

		bool ReadStruct_unsync(SPPTStatusStorage *aData)		// ��������� ��������� unsync
		{
			__try
			{
				SPPTStatusStorage *pTable = (SPPTStatusStorage *) ( (BYTE *)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - ������ �������
				memcpy(aData, pTable, sizeof(SPPTStatusStorage)); 
			}
			__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Failed to read from the view.
				ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
				return false;
			}
			return true;
		}

		bool WriteStruct_unsync(SPPTStatusStorage *aData)	// �������� ��������� unsync
		{
			__try
			{
				SPPTStatusStorage *pTable = (SPPTStatusStorage *) ( (BYTE *)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - ������ �������
				memcpy(pTable, aData, sizeof(SPPTStatusStorage)); 
			}
			__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Failed to read from the view.
				ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
				return false;
			}
			return true;
		}

	public:

		bool ReadStruct(SPPTStatusStorage *aData)
		{
			CSynchAccess oAccess = m_hAccessMutex;
			return ReadStruct_unsync(aData);
		}
		bool WriteStruct(SPPTStatusStorage *aData)
		{
			CSynchAccess oAccess = m_hAccessMutex;
			return WriteStruct_unsync(aData);
		}

	public:

		// �������� ������ ����������� (bForceControl - ���������� ����� ��������, ���� ���������)
		// ���������� *bForceControl == true - ���������� ������ ���� ��������
		bool GetStatus(bool *bForceControl, SPPTStatusStorage* aIS)
		{
			bool bTryToForceControl = bForceControl && (*bForceControl);

			if (NULL == aIS) return false;

			if (bForceControl) *bForceControl = false;	// ���� �� �� ����� ����� ��������

			// ������ ��� ��� �������� :)
			CSynchAccess oAccess = m_hAccessMutex;
			
			if (!ReadStruct_unsync(aIS)) return false;	// �� ���������� ��������� ������

			if ((STIF_BROKEN == aIS->m_sStatus) || (STIF_ERROR == aIS->m_sStatus))
			{
				// ������� �������� - �������, ��� ����� ����� �������� (���� ����� ����)
				if (bTryToForceControl)
				{
					// ���������� �������
					aIS->Reset();
					aIS->m_sStatus = STIF_CREATING;
					*bForceControl = WriteStruct_unsync(aIS);	// ����� � ������ � �������, ��� ����� ��������
				}
			}

			return true;
		}
		// ���������� ������, ��� ���������� ���������. ���������� ����� ��������������� ������� ���������
		bool SetBroken()
		{
			CSynchAccess oAccess = m_hAccessMutex;
			SPPTStatusStorage sStor;
			sStor.m_sStatus = STIF_BROKEN;
			// ����� � �-������
			return WriteStruct_unsync(&sStor);
		}
		// ��������� ��������� (������ ��������)
		void Progress(double aProgress)
		{
			CSynchAccess oAccess = m_hAccessMutex;
			
			SPPTStatusStorage sStor;
			ReadStruct_unsync(&sStor);
			sStor.m_nProgress = aProgress;
			
			WriteStruct_unsync(&sStor);
		}
		// ��������� ���������
		double Progress()
		{
			CSynchAccess oAccess = m_hAccessMutex;
			
			SPPTStatusStorage sStor;
			ReadStruct_unsync( &sStor );
			return sStor.m_nProgress;
		}
	private:
		// �������� ������ ������ � ���������
		bool Save(CAtlArray<BYTE> &aTable)
		{
			return false;
		}

		bool Load(CAtlArray<BYTE> &aTable)
		{
			return false;
		}


	public:
		CStatusStorage(CString &aFileName):
		  CShareMemArray(aFileName, sizeof(SPPTStatusStorage), ISID_PPTSTATUSSTORAGE)
		{
			//ATLTRACE2("CStatusStorage created;\n");
		};	
		~CStatusStorage()
		{
			//ATLTRACE2("CStatusStorage destroyed;\n");
		};
	};

	// ����� ��������� ������� �������
	class CInfoStorage: public CShareMemArray<BYTE>
	{
	protected:

		bool ReadStruct_unsync(SPPTInfoStorage *aData)		// ��������� ��������� unsync
		{
			__try
			{
				LONG64 lLength = Size();
				aData->Load((BYTE*)m_pArray + sizeof(LONG64), lLength);	// sizeof(LONG64) - ������ �������
			}
			__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Failed to read from the view.
				ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
				return false;
			}
			return true;
		}

		bool WriteStruct_unsync(SPPTInfoStorage *aData)	// �������� ��������� unsync
		{
			__try
			{
				LONG64 *pSize = (LONG64*)m_pArray;	
				*pSize = m_nSize;

				aData->Save((BYTE*)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - ������ �������
			}
			__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Failed to read from the view.
				ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
				return false;
			}
			return true;
		}

	public:

		bool ReadStruct(SPPTInfoStorage *aData)
		{
			CSynchAccess oAccess = m_hAccessMutex;
			return ReadStruct_unsync(aData);
		}
		bool WriteStruct(SPPTInfoStorage *aData)
		{
			CSynchAccess oAccess = m_hAccessMutex;
			return WriteStruct_unsync(aData);
		}
		bool WriteCount(SPPTInfoStorage *aData)
		{
			__try
			{
				BYTE* pData = (BYTE*)m_pArray + sizeof(LONG64);
				*((LONG*)pData) = aData->m_lCount;
			}
			__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				// Failed to read from the view.
				ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
				return false;
			}
			return true;
		}

	private:
		// �������� ������ ������ � ���������
		bool Save(CAtlArray<BYTE> &aTable)
		{
			return false;
		}

		bool Load(CAtlArray<BYTE> &aTable)
		{
			return false;
		}


	public:
		CInfoStorage(CString &aFileName, LONG64 lSize):
		  CShareMemArray(aFileName, lSize, ISID_PPTINFOSTORAGE)
		{
			//ATLTRACE2("CStatusStorage created;\n");
		};	
		~CInfoStorage()
		{
			//ATLTRACE2("CStatusStorage destroyed;\n");
		};
	};
}
