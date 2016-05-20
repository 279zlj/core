#ifndef _PDF_READER_MEMORY_UTILS_H
#define _PDF_READER_MEMORY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace PdfReader
{
	//------------------------------------------------------------------------
	// ���������� malloc, �� � ���������� ������.
	static void *MemUtilsMalloc(int nSize)
	{
		void *pResult;

		if (nSize <= 0)
			return NULL;

		if (!(pResult = malloc(nSize)))
		{
			// TO DO: ������ ������ ��������� ������
		}
		return pResult;
	}


	// ���� ��� � realloc, �� � ���������� ������. 
	// ���� <pData> NULL, ���������� ������� malloc ������ realloc.
	static void *MemUtilsRealloc(void *pData, int nSize)
	{
		void *pResult;

		if (nSize <= 0)
		{
			if (pData)
				free(pData);
			return NULL;
		}
		if (pData)
			pResult = realloc(pData, nSize);
		else
			pResult = malloc(nSize);
		if (!pResult)
		{
			// TO DO: ������ ������ ��������� ������
		}
		return pResult;
	}


	// ���� �����, ��� � MemUtilsMalloc and MemUtilsRelloc, ������t
	// ��������� ���������� ��������� � ������ ��������. � ����������
	// ���������� ������ �������� nObjectsCount * nObjectSize ����. 
	// ����� ���� ������������ ��������� ������ � �������� ����, �����
	// ��������� ������ �� �������� ������ ��� int.
	static void *MemUtilsMallocArray(int nObjectsCount, int nObjectSize)
	{
		if (0 == nObjectsCount)
			return NULL;

		int nSize = nObjectsCount * nObjectSize;
		if (nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= INT_MAX / nObjectSize)
		{
			// TO DO: ������ ������ ��������� ������
		}
		return MemUtilsMalloc(nSize);
	}

	// ���� ��� � free, �� ��������� � ���������� NULL-���������.
	static void MemUtilsFree(void *pData)
	{
		if (pData)
			free(pData);
	}

	static void *MemUtilsReallocArray(void *pData, int nObjectsCount, int nObjectSize)
	{
		if (0 == nObjectsCount)
		{
			if (pData)
				MemUtilsFree(pData);
			return NULL;
		}
		int nSize = nObjectsCount * nObjectSize;

		if (nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= INT_MAX / nObjectSize)
		{
			// TO DO: ������ ������ ��������� ������
		}
		return MemUtilsRealloc(pData, nSize);
	}

	// �������� ������ � �������� ���� ������.
	static char *CopyString(char *sString)
	{
		char *sResult = (char *)MemUtilsMalloc(strlen(sString) + 1);
		strcpy(sResult, sString);
		return sResult;
	}
}

#endif // _PDF_READER_MEMORY_UTILS_H
