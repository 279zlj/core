/***************************************************************

	CShareMemArray 

		������ ������ ��� ������ � ����������� 
		Shared-�������. ����������� �������� �������
		�� ������ � ���������� � ������.

***************************************************************/

#pragma once
#pragma warning( disable : 4996 4244)	// No CRT-secure warning

#include <atlcoll.h>
#include "AVSUtils.h"	// CSyncAccess class definition

#define AVS_USER_NAME_LEN 1024

// ������ ��������� (������, ��� �������, �����)
enum TSMAStatus {SMAS_ERROR, SMAS_ALREADYEXISTS, SMAS_NEW};

// ���������
template <typename STOR_TYPE>
class CShareMemArray
{
protected:
	HANDLE		m_hAccessMutex;		// ������� ��� ����������� ������� � Shared-Memory
	HANDLE		m_hMapFile;			// ����� �� map �������
	STOR_TYPE	*m_pArray;			// ��������� �� ������
	LONG64		m_nSize;			// ������ �������
	CString		m_sMutexName;		// ��� ��������
	CString		m_sMapName;			// ��� �����
	TSMAStatus	m_sStatus;

protected:
	// ������ �� ������
	bool ReadFromSharedMem(LONG64 nIndex, STOR_TYPE &nValue)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return false;
		}

		__try
		{
			STOR_TYPE *pTable = (STOR_TYPE *) (((BYTE *) m_pArray) + sizeof(LONG64));	// sizeof(LONG64) - ������ �������
			nValue = pTable[nIndex];
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("CIndexerStorage::ReadFromSharedMem()\n");
			return false;
		}
		return true;
	}

	// ����� � ������
	bool WriteToSharedMem(LONG64 nIndex, STOR_TYPE aValue)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return false;
		}

		__try
		{
			STOR_TYPE *pTable = (STOR_TYPE *) (((BYTE *) m_pArray) + sizeof(LONG64));	// sizeof(LONG64) - ������ �������
			pTable[nIndex] = aValue;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("Error CIndexerStorage::WriteToSharedMem(#i)\n", nIndex);
			return false;
		}
		return true;
	}

	// �������� ������� �������� �� SharedMemory
	bool SaveTable_unsync(CAtlArray<STOR_TYPE> &aTable)
	{
		if ((NULL == m_pArray) || (NULL == m_hMapFile)) 
		{
			m_sStatus = SMAS_ERROR;
			return false;	// ������ �� ������
		}
	
		bool bRes = true;

		// ������ �� ������
		LONG64 nCopyCount = (m_nSize <= (LONG64) aTable.GetCount()) ? m_nSize : aTable.GetCount();
		
		// ��������� ������ �������
		Size_unsync(m_nSize);

		// �������� �� ������ � ������ (safe)
		for (LONG64 nIndex = 0; nIndex < nCopyCount; nIndex++)
		{
			bRes &= WriteToSharedMem (nIndex, aTable[nIndex]);
		}

		return bRes;
	}

	// ���������� ������� �������� �� SharedMemory
	bool LoadTable_unsync(CAtlArray<STOR_TYPE> &aTable)
	{
		if ((NULL == m_pArray) || (NULL == m_hMapFile)) 
		{
			m_sStatus = SMAS_ERROR;
			return false;	// ������ �� ������
		}
	
		aTable.RemoveAll();
		
		// ���������� ������
		m_nSize = Size_unsync();

		STOR_TYPE nValue;

		// �������� �� ������ � ������
		for (DWORD nIndex = 0; nIndex < m_nSize; nIndex++)
		{	
			if (ReadFromSharedMem(nIndex, nValue))
			{
				// ��������� � �������
				aTable.Add(nValue);
			}
		}
		return true;
	}

	// ������
	LONG64 Size_unsync()
	{
		LONG64 nValue = -1;

		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return nValue;
		}

		__try
		{
			LONG64 *pSize = (LONG64 *) m_pArray;
			nValue = *pSize;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("CIndexerStorage::Size_unsync()\n");
			return -1;
		}
		return nValue;
	}

	void Size_unsync(LONG64 aSize)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return;
		}

		__try
		{
			LONG64 *pSize = (LONG64*) m_pArray;	// sizeof(LONG64) - ������ �������
			*pSize = aSize;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("Error CIndexerStorage::Size_unsync(LONG64 aSize)\n");
		}
	}

public:
	// aSize - ������ ������� ���������� (���-�� ����������), 
	// Id - �������������� �������������, ���� ����� ���� ��������� ����� ���������� ��� ������ �����
	CShareMemArray(CString &aFileName, LONG64 aSize, DWORD aId = ISID_DEFAULT):
	  m_hMapFile(NULL), m_nSize(aSize), m_pArray(NULL), m_sStatus(SMAS_ERROR)
	{
		// "�����������" ��� �����
		TCHAR aDrive[_MAX_DRIVE];
		TCHAR aDir[_MAX_DIR];
		TCHAR aFName[_MAX_FNAME];
		TCHAR aExt[_MAX_EXT];

		_tsplitpath (aFileName.GetBuffer(), aDrive, aDir, aFName, aExt);
		//_wsplitpath_s (aFileName.GetBuffer(), aDrive, _MAX_DRIVE, aDir, _MAX_DIR, aFName, _MAX_FNAME, aExt, _MAX_EXT);

		// ������������� ��������� ���������� ������� ��� ����� ����, ����� �������� � ��� �����
		DWORD dwPathID = 0;
		TCHAR tcPathIDItem = 0;

		// ������������ ����������� ����� ��� ����
		for (int i = 0; i < (int) _tcslen(aDir); i++) 
		{
			tcPathIDItem ^= aDir[i];
			dwPathID ^= dwPathID << 1;
			dwPathID += (DWORD) tcPathIDItem;
		}

		// ������������ ����������� ����� ��� ����������
		DWORD dwExtID = 0;
		TCHAR tcExtIDItem = 0;		
		for (int i = 0; i < (int) _tcslen(aExt); i++) 
		{
			tcExtIDItem ^= aExt[i];
			 dwExtID ^=  dwExtID << 1;
			 dwExtID += (DWORD) tcExtIDItem;
		}
		
		// ��������� ��� ������� � ����
		// ���� ��� �� �������� ���� ����� ��� ������������ � ������ ��������� ���� ����� ����������.
		// ���� � ���������� ��������.
		// � ������ ����� �������� ������ ������ "Global\"
		// ����������: "Global\" �� �������� �� Win7, ���� ����� �������� �� ��� �������
		m_sMutexName.Format(_T("Local\\avs_mutex%u_%s_%06x_%06I64x_%06x"), aId, aFName, dwPathID, aSize, dwExtID);
		m_sMapName.Format(_T("Local\\avs_storage%u_%s_%06x_%06I64x_%06x"), aId, aFName, dwPathID, aSize, dwExtID);

		// ��������� ��� �����
		TCHAR pBufferUserName[AVS_USER_NAME_LEN];
		DWORD dwBufferUserNameLen = AVS_USER_NAME_LEN;
		GetUserName(pBufferUserName, &dwBufferUserNameLen);
		
		CString strUserName(pBufferUserName, dwBufferUserNameLen);
		m_sMutexName	+= strUserName;
		m_sMapName		+= strUserName;

		// ������� �������
		m_hAccessMutex = CreateMutex(NULL, FALSE, m_sMutexName.GetBuffer());

		// ����� ��� ������ "��� ��������" :)
		CSynchAccess oAccess = m_hAccessMutex;
		
		// ������� ���
		ATLTRACE2("CShareMemArray()::CShareMemArray(): m_nSize = %d\n", m_nSize);

		ULARGE_INTEGER nMappingSize;
		nMappingSize.QuadPart = m_nSize * sizeof(STOR_TYPE) + sizeof(LONG64);

		m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, nMappingSize.HighPart, nMappingSize.LowPart, m_sMapName.GetBuffer());
		if (NULL == m_hMapFile)
		{
			// ������
			ATLTRACE2("CShareMemArray::CShareMemArray():CreateFileMapping() FAILS (0x%x)!\n", GetLastError());
			m_sStatus = SMAS_ERROR;
		}
		else 
		{
			// ��� ��!
			m_sStatus = (GetLastError() == ERROR_ALREADY_EXISTS) ? SMAS_ALREADYEXISTS : SMAS_NEW;
			ATLTRACE2 (SMAS_ALREADYEXISTS == m_sStatus ? "CShareMemArray: open existing!\n" : "CShareMemArray: create new!\n");

			// ������� View of file
			m_pArray = (STOR_TYPE *) MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, (SIZE_T) nMappingSize.QuadPart);
			if (NULL == m_pArray)
			{
				// ������
				ATLTRACE2("CShareMemArray::CShareMemArray():MapViewOfFile() FAILS (0x%x)!\n", GetLastError());
				m_sStatus = SMAS_ERROR;
			}
		}
	}
	
	virtual ~CShareMemArray()
	{
		if (m_pArray) UnmapViewOfFile(m_pArray);			// ������� view
		if (NULL != m_hMapFile) CloseHandle(m_hMapFile);	// ������� ���
		if (NULL != m_hAccessMutex) CloseHandle(m_hAccessMutex);	// ������� �������
	}

public:
	
	// ��������� ������� � �����-������
	bool Save(CAtlArray<STOR_TYPE> &aTable)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return SaveTable_unsync(aTable);
	}

	// ��������� �������
	bool Load(CAtlArray<STOR_TYPE> &aTable)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return LoadTable_unsync(aTable);
	}

	// ������ ���������
	LONG64 Size()
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return Size_unsync();
	}

	/*void Size(LONG64 aSize)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		Size_unsync(aSize);
	}*/

	// ��������� �������
	TSMAStatus Status(void) const 
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return m_sStatus;
	}
};

