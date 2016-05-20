#pragma once

namespace NFileWriter
{
	//------------------------------------------------------------------------------------------------------
	// CBufferedFileWriter
	//------------------------------------------------------------------------------------------------------
	// ����������� �����, ������� �������� � ���� ���������� �������
	//------------------------------------------------------------------------------------------------------

	class CFileWriter
	{
	public :

		// ��������� ������ �� ������
		virtual VOID Write ( LPBYTE lpData, LONG64 lDataLength ) = 0;
		// ���� ( ���������� ��� ������ )
		virtual VOID Flush () = 0;
		// ��� �� ��������� ������� �� �����
		virtual VOID Seek ( LONG64 lPosition, DWORD dwFrom = FILE_CURRENT ) = 0;
		// ������� ������� 
		virtual VOID GetPosition(ULONGLONG& nPos) = 0;
		// ������ ����������� �����
		virtual VOID GetSize(ULONGLONG& nLen) = 0;

	public :

		// �����������
		CFileWriter ()
		{
		}
		// ����������� ����������
		virtual ~CFileWriter ()
		{
		}
	};

	//------------------------------------------------------------------------------------------------------
	// CBufferedFileWriter
	//------------------------------------------------------------------------------------------------------
	// �����, ������� ���������� ������� ����� �� ����
	//------------------------------------------------------------------------------------------------------

	class CBufferedFileWriter : public CFileWriter
	{
	private :

		HANDLE m_hFile;									// ����� �� ����, � ������� ���� ������

		LONG64 m_lBufferSize;							// ������ �������

		LPBYTE m_lpBuffer;								// ��� ������

		LONG64 m_lWritePointer;							// ������� ��� ������ ������ � ������

	public :

		// ����������� ( �� ����� �����, ��� ������� ��������� ���� )
		CBufferedFileWriter ( TCHAR *lpszFile, LONG64 lBufferSize = 10 * 1024 * 1024 ) : CFileWriter ()
		{
			// ������� ����� �� ����, � ������� ����� �������������� ������, � �������������� �����, ������� ������� �������
			m_hFile = CreateFile (lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			// ���� �� ���������� ������� ����
			if ( INVALID_HANDLE_VALUE == m_hFile )
			{
				// ������ ����������
				throw 1;
			}

			// �������� ������
			m_lBufferSize		= lBufferSize;

			// �������� ����������
			m_lWritePointer		= 0;

			// �������� �����.
			m_lpBuffer = ( LPBYTE ) ::HeapAlloc ( GetProcessHeap (), HEAP_ZERO_MEMORY, (size_t) m_lBufferSize );

			// ���� ������ �� ���������, ���������� Exception
			if ( !m_lpBuffer )
			{
				// ������ ����������
				throw 1;
			}

			// ��� �������!!!
		}

		// ����������
		virtual ~CBufferedFileWriter ()
		{
			// ���� ����� ������� ����, ��������� ���
			if ( m_hFile && m_hFile != INVALID_HANDLE_VALUE )
			{
				// ���������� ������� ��� ������ ����������, � ����� �������
				try
				{
					// ��� �� ������, ���� ������ ������ �� �����, �� ����-�� ������� �����
					Flush ();
				}
				catch ( ... )
				{
				}
				CloseHandle ( m_hFile );
				m_hFile = NULL;
			}

			// ������� ������
			if ( m_lpBuffer )
			{
				::HeapFree ( GetProcessHeap (), 0, m_lpBuffer );
				m_lpBuffer = NULL;
			}
		}
		// ��������� ������ �� ������
		virtual VOID Write ( LPBYTE lpData, LONG64 lDataLength )
		{
			// ���� ������ ������ �� ��������
			while ( 0 < lDataLength )
			{
				// ������ ��������� ������ � �������
				LONG64 lBufferFreeLength = 0;

				// ����, ���� �� �������� ��������� ����� ��� ������
				while ( 0 >= ( lBufferFreeLength = m_lBufferSize - m_lWritePointer ) )
				{
					// ����� ������, ���� �� ����������, �� ���������� ����������
					if ( FALSE == WriteBuffer ( m_lBufferSize ) )
						throw 1;
				}

				// ��������� ���������� �� �����, ����� �������� ���� ��������� ����� � ������
				if ( lBufferFreeLength > lDataLength )
					lBufferFreeLength = lDataLength;

				// �������� ������ � ������
				memcpy ( m_lpBuffer + m_lWritePointer, lpData, (size_t) lBufferFreeLength);

				// ��������� ������ ������ ����� �� �������, ������� �������� � ������
				lDataLength -= lBufferFreeLength;
				// �������� ��������� �� ��������� ������
				lpData = lpData + lBufferFreeLength;
				// �������� ������� ��� ������ ������ � ������
				m_lWritePointer += lBufferFreeLength;
			}
		}
		// ���� ( ���������� ��� ������ )
		virtual VOID Flush ()
		{
			// ����� ��� ������, ������� ����
			if ( 0 < m_lWritePointer )
			{
				// ���� ������ ������, �� ���������� ����������
				if ( FALSE == WriteBuffer ( m_lWritePointer ) )
					throw 1;
			}
		}
		// ��� �� ��������� ������� �� �����
		virtual VOID Seek ( LONG64 lPosition, DWORD dwFrom = FILE_CURRENT )
		{

			// ������� ������� ��� ������
			Flush ();

			// ������ ��� �� �����
			LARGE_INTEGER liOffset;
			liOffset.QuadPart = lPosition;
			DWORD nNewPos = ::SetFilePointer(m_hFile, liOffset.LowPart, &liOffset.HighPart, dwFrom);
			
			if (nNewPos == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}

		}
		// ������� ������� 
		virtual VOID GetPosition(ULONGLONG& nPos)
		{
			nPos=0;
			if(m_hFile == NULL)return;

			LARGE_INTEGER liOffset;
			liOffset.QuadPart = 0;
			liOffset.LowPart = ::SetFilePointer(m_hFile, 0, &liOffset.HighPart, FILE_CURRENT);
			if (liOffset.LowPart == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}
			nPos = liOffset.QuadPart + m_lWritePointer;
		}
		// ������ ����������� �����
		virtual void GetSize(ULONGLONG& nLen) 
		{
			nLen=0;
			if(m_hFile == NULL)return;
			
			LARGE_INTEGER liOffset;
			ULARGE_INTEGER liFileSize;
			
			liOffset.QuadPart = 0;
			liOffset.LowPart = ::SetFilePointer(m_hFile, 0, &liOffset.HighPart, FILE_CURRENT);

			liFileSize.LowPart = ::GetFileSize(m_hFile, &liFileSize.HighPart);
			
			if (liFileSize.LowPart == INVALID_FILE_SIZE || liOffset.LowPart == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}

			nLen = liFileSize.QuadPart;

			if (liFileSize.QuadPart==liOffset.QuadPart) //��������� � ����� ����� �� ���� ��������� �����
			{
				nLen += m_lWritePointer;
			}
		}
	private :

		// ���� ������ �� ����
		BOOL WriteBuffer ( LONG64 lSize )
		{
			// ��������
			if ( INVALID_HANDLE_VALUE == m_hFile || 0 == lSize )
				return FALSE;

			// ������� ���� ���������� �� ����
			DWORD dwBytesWrite = 0;

			// ����� �� ����
			if ( !WriteFile ( m_hFile, m_lpBuffer, ( DWORD ) lSize, &dwBytesWrite, NULL ) )
			{
				// ���� ���-�� ��������� �� ���, ���������� FALSE
				return FALSE;
			}

			// ��������, ���� ���������� ������, �� ��� ������
			if ( ( LONG64 ) dwBytesWrite != lSize )
				return FALSE;

			// ���������� � 0 ��������� �� ������� ������ � �������
			m_lWritePointer = 0;
			return TRUE;
		}
	};
}