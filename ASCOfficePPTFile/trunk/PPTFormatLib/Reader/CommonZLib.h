#pragma once 
// zlib - ������������ ������� � �������.
// ����� ��������� - ��������� � ������� ���� � �������

#include <zlib.h>

#pragma comment(lib, "zlib.lib")

namespace NSZLib
{
	static BOOL Decompress(const BYTE* pSrcBuffer, const ULONG& lSrcBufferLen,
								BYTE* pDstBuffer, ULONG& lDstBufferLen)
	{
		try
		{
			if (Z_OK == uncompress(pDstBuffer, &lDstBufferLen, pSrcBuffer, lSrcBufferLen))
			{
				return TRUE;
			}
		}
		catch(...)
		{
		}
		return FALSE;
	}
}