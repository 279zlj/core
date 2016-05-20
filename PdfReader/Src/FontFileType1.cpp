#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "FontFileEncodings.h"
#include "FontFileType1.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CFontFileType1
	//------------------------------------------------------------------------

	CFontFileType1 *CFontFileType1::LoadFromBuffer(char *sBuffer, int nLen)
	{
		return new CFontFileType1(sBuffer, nLen, false);
	}

	CFontFileType1 *CFontFileType1::LoadFromFile(wchar_t *wsFileName)
	{
		char *sBuffer;
		int nLen = 0;

		if (!(sBuffer = CFontFileBase::ReadFile(wsFileName, &nLen)))
			return NULL;

		return new CFontFileType1(sBuffer, nLen, true);
	}

	CFontFileType1::CFontFileType1(char *sBuffer, int nLen, bool bFreeData) :
		CFontFileBase(sBuffer, nLen, bFreeData)
	{
		m_sName = NULL;
		m_arrEncoding = NULL;
		m_bParsed = false;
	}

	CFontFileType1::~CFontFileType1()
	{
		if (m_sName)
			MemUtilsFree(m_sName);

		if (m_arrEncoding && m_arrEncoding != c_arrsFontFileType1StandardEncoding)
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				MemUtilsFree(m_arrEncoding[nIndex]);
			}
			MemUtilsFree(m_arrEncoding);
		}
	}

	char *CFontFileType1::GetName()
	{
		if (!m_bParsed)
			Parse();

		return m_sName;
	}

	char **CFontFileType1::GetEncoding()
	{
		if (!m_bParsed)
			Parse();

		return m_arrEncoding;
	}

	void CFontFileType1::WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream)
	{
		char sBuffer[512];
		char *sLine, *sLine2, *sCurChar;

		// �������� ��� �� ������� /Encoding
		for (sLine = (char *)m_sFile; sLine && strncmp(sLine, "/Encoding", 9); sLine = GetNextLine(sLine));
		if (!sLine)
		{
			// �� ����� ���������, ����� �������� ������� ���� ����
			(*pOutputFunc)(pOutputStream, (char *)m_sFile, m_nLen);
			return;
		}
		(*pOutputFunc)(pOutputStream, (char *)m_sFile, sLine - (char *)m_sFile);

		// ����� ����� ���������
		(*pOutputFunc)(pOutputStream, "/Encoding 256 array\n", 20);
		(*pOutputFunc)(pOutputStream, "0 1 255 {1 index exch /.notdef put} for\n", 40);
		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			if (ppNewEncoding[nIndex])
			{
				sprintf(sBuffer, "dup %d /%s put\n", nIndex, ppNewEncoding[nIndex]);
				(*pOutputFunc)(pOutputStream, sBuffer, strlen(sBuffer));
			}
		}
		(*pOutputFunc)(pOutputStream, "readonly def\n", 13);

		if (!strncmp(sLine, "/Encoding StandardEncoding def", 30))
		{
			sLine = GetNextLine(sLine);
		}
		else
		{
			sCurChar = sLine + 10;
			sLine = NULL;
			for (; sCurChar < (char *)m_sFile + m_nLen; ++sCurChar)
			{
				if ((*sCurChar == ' ' || *sCurChar == '\t' || *sCurChar == '\x0a' || *sCurChar == '\x0d' || *sCurChar == '\x0c' || *sCurChar == '\0') && sCurChar + 4 <= (char *)m_sFile + m_nLen && !strncmp(sCurChar + 1, "def", 3))
				{
					sLine = sCurChar + 4;
					break;
				}
			}
		}

		// � ��������� ������ ��� ������ /Encoding, ������� ��������� ������� ������ ������
		if (sLine)
		{
			int nIndex;
			for (sLine2 = sLine, nIndex = 0; nIndex < 20 && sLine2 && strncmp(sLine2, "/Encoding", 9); sLine2 = GetNextLine(sLine2), ++nIndex);
			if (nIndex < 20 && sLine2)
			{
				(*pOutputFunc)(pOutputStream, sLine, sLine2 - sLine);
				if (!strncmp(sLine2, "/Encoding StandardEncoding def", 30))
				{
					sLine = GetNextLine(sLine2);
				}
				else
				{
					sCurChar = sLine2 + 10;
					sLine = NULL;
					for (; sCurChar < (char *)m_sFile + m_nLen; ++sCurChar)
					{
						if ((*sCurChar == ' ' || *sCurChar == '\t' || *sCurChar == '\x0a' || *sCurChar == '\x0d' || *sCurChar == '\x0c' || *sCurChar == '\0') && sCurChar + 4 <= (char *)m_sFile + m_nLen && !strncmp(sCurChar + 1, "def", 3))
						{
							sLine = sCurChar + 4;
							break;
						}
					}
				}
			}

			// �������� ��� ����� ���������
			if (sLine)
			{
				(*pOutputFunc)(pOutputStream, sLine, ((char *)m_sFile + m_nLen) - sLine);
			}
		}
	}

	char *CFontFileType1::GetNextLine(char *sLine)
	{
		while (sLine < (char *)m_sFile + m_nLen && *sLine != '\x0a' && *sLine != '\x0d')
			++sLine;

		if (sLine < (char *)m_sFile + m_nLen && *sLine == '\x0d')
			++sLine;

		if (sLine < (char *)m_sFile + m_nLen && *sLine == '\x0a')
			++sLine;

		if (sLine >= (char *)m_sFile + m_nLen)
			return NULL;

		return sLine;
	}

	void CFontFileType1::Parse()
	{
		char *sLine, *sLine1, *pCur, *pTemp;
		char sBuffer[256];
		int nCount, nCode;
		int nIndex = 0;

		for (nIndex = 1, sLine = (char *)m_sFile; nIndex <= 100 && sLine && (!m_sName || !m_arrEncoding); ++nIndex)
		{
			if (!m_sName && !strncmp(sLine, "/FontName", 9))
			{
				strncpy(sBuffer, sLine, 255);
				sBuffer[255] = '\0';
				if ((pCur = strchr(sBuffer + 9, '/')) && (pCur = strtok(pCur + 1, " \t\n\r")))
				{
					m_sName = CopyString(pCur);
				}
				sLine = GetNextLine(sLine);

			}
			else if (!m_arrEncoding && !strncmp(sLine, "/Encoding StandardEncoding def", 30))
			{
				m_arrEncoding = c_arrsFontFileType1StandardEncoding;
			}
			else if (!m_arrEncoding && !strncmp(sLine, "/Encoding 256 array", 19))
			{
				m_arrEncoding = (char **)MemUtilsMallocArray(256, sizeof(char *));
				int nJ = 0;
				for (nJ = 0; nJ < 256; ++nJ)
				{
					m_arrEncoding[nJ] = NULL;
				}
				for (nJ = 0, sLine = GetNextLine(sLine); nJ < 300 && sLine && (sLine1 = GetNextLine(sLine)); ++nJ, sLine = sLine1)
				{
					if ((nCount = sLine1 - sLine) > 255)
					{
						nCount = 255;
					}
					strncpy(sBuffer, sLine, nCount);
					sBuffer[nCount] = '\0';
					for (pCur = sBuffer; *pCur == ' ' || *pCur == '\t'; ++pCur);
					if (!strncmp(pCur, "dup", 3))
					{
						for (pCur += 3; *pCur == ' ' || *pCur == '\t'; ++pCur);
						for (pTemp = pCur; *pTemp >= '0' && *pTemp <= '9'; ++pTemp);
						if (*pTemp)
						{
							char nChar = *pTemp;
							*pTemp = '\0';
							nCode = atoi(pCur);
							*pTemp = nChar;
							if (nCode == 8 && *pTemp == '#')
							{
								nCode = 0;
								for (++pTemp; *pTemp >= '0' && *pTemp <= '7'; ++pTemp)
								{
									nCode = nCode * 8 + (*pTemp - '0');
								}
							}
							if (nCode < 256)
							{
								for (pCur = pTemp; *pCur == ' ' || *pCur == '\t'; ++pCur);
								if (*pCur == '/')
								{
									++pCur;
									for (pTemp = pCur; *pTemp && *pTemp != ' ' && *pTemp != '\t'; ++pTemp);
									*pTemp = '\0';
									m_arrEncoding[nCode] = CopyString(pCur);
								}
							}
						}
					}
					else
					{
						if (strtok(sBuffer, " \t") && (pCur = strtok(NULL, " \t\n\r")) && !strcmp(pCur, "def"))
						{
							break;
						}
					}
				}
			}
			else
			{
				sLine = GetNextLine(sLine);
			}
		}

		m_bParsed = true;
	}
}