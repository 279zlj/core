#pragma once

#include "./DocxFormat/Source/SystemUtility/File.h"

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
		virtual void Write ( BYTE* lpData, LONG64 lDataLength ) = 0;
		// ���� ( ���������� ��� ������ )
		virtual void Flush () = 0;
		// ��� �� ��������� ������� �� �����
                virtual void Seek ( LONG64 lPosition, DWORD dwFrom = 0 ) = 0;
		// ������� ������� 
                virtual void GetPosition(ULONG64& nPos) = 0;
		// ������ ����������� �����
                virtual void GetSize(ULONG64& nLen) = 0;

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

		CFile m_oFile;
		//HANDLE m_hFile;									// ����� �� ����, � ������� ���� ������

		LONG64 m_lBufferSize;							// ������ �������

		BYTE* m_lpBuffer;								// ��� ������

		LONG64 m_lWritePointer;							// ������� ��� ������ ������ � ������

	public :

		// ����������� ( �� ����� �����, ��� ������� ��������� ���� )
		CBufferedFileWriter ( CString & sFileName, LONG64 lBufferSize = 10 * 1024 * 1024 ) : CFileWriter ()
		{
			if (m_oFile.CreateFile(sFileName) != S_OK)
			// ���� �� ���������� ������� ����
			{
				// ������ ����������
				throw 1;
			}

			// �������� ������
			m_lBufferSize		= lBufferSize;

			// �������� ����������
			m_lWritePointer		= 0;

			// �������� �����.
			m_lpBuffer = ( BYTE* ) new BYTE[ m_lBufferSize ];

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
			Flush();
			m_oFile.CloseFile();

			// ������� ������
			RELEASEARRAYOBJECTS(m_lpBuffer);
		}
		// ��������� ������ �� ������
		virtual void Write ( BYTE* lpData, LONG64 lDataLength )
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
                                        if ( false == WriteBuffer ( m_lBufferSize ) )
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
		virtual void Flush ()
		{
			// ����� ��� ������, ������� ����
			if ( 0 < m_lWritePointer )
			{
				// ���� ������ ������, �� ���������� ����������
                                if ( false == WriteBuffer ( m_lWritePointer ) )
					throw 1;
			}
		}
		// ��� �� ��������� ������� �� �����
		virtual void Seek ( LONG64 lPosition, DWORD dwFrom/* = FILE_CURRENT */)
		{

			// ������� ������� ��� ������
			Flush ();

			if (dwFrom == 1/*FILE_CURRENT*/)
			{
				dwFrom += m_oFile.GetPosition();
			}

			if (m_oFile.SetPosition(lPosition) != S_OK)
				throw 1;

		}
		// ������� ������� 
                virtual void GetPosition(ULONG64& nPos)
		{
			nPos = m_oFile.GetPosition() + m_lWritePointer;

		}
		// ������ ����������� �����
                virtual void GetSize(ULONG64& nLen)
		{
			nLen = m_oFile.GetFileSize() + m_lWritePointer;	

		}
	private :

		// ���� ������ �� ����
                bool WriteBuffer ( LONG64 lSize )
		{
			// ����� �� ����
			if (m_oFile.WriteFile(m_lpBuffer, ( DWORD ) lSize) != S_OK)
			{
				// ���� ���-�� ��������� �� ���, ���������� FALSE
                                return false;
			}
			// ���������� � 0 ��������� �� ������� ������ � �������
			m_lWritePointer = 0;
                        return true;
		}
	};
}
