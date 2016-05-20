#include "Common.h"

#include "../../raster/ImageFileFormatChecker.h"
#include "../../raster/BgraFrame.h"

namespace MetaFile
{	

	bool ReadImageCoreHeader(BYTE* pHeaderBuffer, unsigned long ulHeaderBufferLen, BYTE* pImageBuffer, unsigned long ulImageBufferLen, BYTE** ppDstBuffer, unsigned long* pulWidth, unsigned long* pulHeight)
	{
		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		unsigned short ushWidth;
		unsigned short ushHeight;
		unsigned short ushPlanes;
		unsigned short ushBitCount;

		oHeaderStream >> ushWidth;
		oHeaderStream >> ushHeight;
		oHeaderStream >> ushPlanes;
		oHeaderStream >> ushBitCount;

		if (0x0001 != ushPlanes)
			return false;

		return false;
	}
	bool ReadImageInfoHeader(BYTE* pHeaderBuffer, unsigned long ulHeaderBufferLen, BYTE* pImageBuffer, unsigned long ulImageBufferLen, BYTE** ppDstBuffer, unsigned long* pulWidth, unsigned long* pulHeight)
	{
		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		int nWidth;
		int nHeight;

		unsigned short ushPlanes;
		unsigned short ushBitCount;

		unsigned int unCompression;
		unsigned int unImageSize;
		unsigned int unXPelsPerMeter;
		unsigned int unYPelsPerMeter;
		unsigned int unColorUsed;
		unsigned int unColorImportant;

		oHeaderStream >> nWidth;
		oHeaderStream >> nHeight;
		oHeaderStream >> ushPlanes;
		oHeaderStream >> ushBitCount;
		oHeaderStream >> unCompression;
		oHeaderStream >> unImageSize;
		oHeaderStream >> unXPelsPerMeter;
		oHeaderStream >> unYPelsPerMeter;
		oHeaderStream >> unColorUsed;
		oHeaderStream >> unColorImportant;

		if (0x0001 != ushPlanes)
			return false;

		if (nHeight < 0x00000000 && (BI_RGB != unCompression))
			return false;

		if (nWidth < 0)
			return false;

		BYTE* pBgraBuffer = NULL;
		unsigned long ulWidth  = 0;
		unsigned long ulHeight = 0;

		BYTE* pBuffer = pImageBuffer;
		long  lBufLen = ulImageBufferLen;

		*ppDstBuffer = NULL;
		*pulWidth    = 0;
		*pulHeight   = 0;

		if (BI_BITCOUNT_0 == ushBitCount) // ������ ���������� ���� PNG, ���� JPEG
		{
			if (BI_JPEG != unCompression || BI_PNG != unCompression)
				return false;

			std::wstring wsTempFileName;
			FILE* pTempFile = NULL;
			if (!WmfOpenTempFile(&wsTempFileName, &pTempFile, L"wb", L".wmf0", NULL))
				return false;

			::fwrite(pBuffer, 1, unImageSize, pTempFile);
			::fclose(pTempFile);

			CBgraFrame oFrame;
			oFrame.OpenFile(wsTempFileName);

			// TODO: ��� ����� ����� ������� ������.
			::_wunlink(wsTempFileName.c_str());

			return false;
		}
		else if (BI_BITCOUNT_1 == ushBitCount)
		{
			// ���������� ��������, ������ ������� ������� �� 2-� ������
			TRgbQuad oColor1, oColor2;
			oHeaderStream >> oColor1 >> oColor2;

			// ��������� ���� ��������
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)		
				return false;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			int nWidthBytes = (nWidth + 7) / 8;
			int nAdditBytes = 4 - div_t(div(((nWidth + 7) / 8), 4)).rem;
			if (4 == nAdditBytes)
				nAdditBytes = 0;

			int nLastBitCount = div_t(div(nWidth, 8)).rem - 1;
			if (-1 == nLastBitCount)
				nLastBitCount = 7;

			nLastBitCount = (int)pow((double)2, (double)nLastBitCount);

			for (int nY = 0, nIndex = 0; nY < abs(nHeight); nY++)
			{
				for (int nX = 0; nX < nWidthBytes; nX++)
				{
					int nByte = *pBuffer; pBuffer++; lBufLen--;
					int nBitCount = 128;
					if (nX == nWidthBytes - 1)
						nBitCount = nLastBitCount;

					for (int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2)
					{
						int nBit = (nByte & nBitIndex);
						TRgbQuad* pColor = (nBit ? &oColor2 : &oColor1);
						pBgraBuffer[nIndex * 4 + 0] = pColor->b;
						pBgraBuffer[nIndex * 4 + 1] = pColor->g;
						pBgraBuffer[nIndex * 4 + 2] = pColor->r;
						pBgraBuffer[nIndex * 4 + 3] = 255;
						nIndex++;
					}
				}
				for (int nAddIndex = 0; nAddIndex < nAdditBytes; nAddIndex++)
				{
					int nByte = *pBuffer; pBuffer++; lBufLen--;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_2 == ushBitCount)
		{
			// TODO: ������� ������ �������, ��� ������ ����� ����� � ������ �����
			return false;
		}
		else if (BI_BITCOUNT_3 == ushBitCount)
		{
			unsigned short ushColorTableLen = 256;

			if (0 != unColorUsed)
				ushColorTableLen = (std::min)((unsigned short)256, (unsigned short)unColorUsed);

			TRgbQuad oColorTable[256];
			if (lBufLen < ushColorTableLen * 4)
				return false;

			// ��������� �������
			for (unsigned short ushIndex = 0; ushIndex < ushColorTableLen; ushIndex++)
			{
				oHeaderStream >> oColorTable[ushIndex];
			}

			// 1 ���� - 1 ������
			// ������ ������ ���� ������ 4.
			int nAdd = 0;
			while (0 != div_t(div(nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			if (lBufLen < (nWidth + nAdd) * nHeight)
				return false;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						BYTE nByte = *pBuffer; pBuffer++; lBufLen--;

						pBgraBuffer[nIndex + 0] = oColorTable[nByte].b;
						pBgraBuffer[nIndex + 1] = oColorTable[nByte].g;
						pBgraBuffer[nIndex + 2] = oColorTable[nByte].r;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						BYTE nByte = *pBuffer; pBuffer++; lBufLen--;

						pBgraBuffer[nIndex + 0] = oColorTable[nByte].b;
						pBgraBuffer[nIndex + 1] = oColorTable[nByte].g;
						pBgraBuffer[nIndex + 2] = oColorTable[nByte].r;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_4 == ushBitCount)
		{
			// ���������� ������� ������ (��� �� �����)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

			if (BI_RGB != unCompression)
				return false; // TODO: ������� ������ �������, ��� ������ ����� ���� � ������ �����

			// ��������� ���� ��������
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 2 ���� �� ��� ������ �����
			// (������ * 3) ������ ���� ������ 4.
			int nAdd = 0;
			while (0 != div_t(div(2 * nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						unsigned short ushValue = ((pBuffer[1] << 8) | pBuffer[0]) & 32767; pBuffer += 2; lBufLen -= 2;
						unsigned char unR = ushValue & 31;            // 000000000011111
						unsigned char unG = (ushValue & 992) >> 5;  // 000001111100000
						unsigned char unB = (ushValue & 31744) >> 10; // 111110000000000

						pBgraBuffer[nIndex + 0] = (unsigned char)(unR / 31.0 * 255);
						pBgraBuffer[nIndex + 1] = (unsigned char)(unG / 31.0 * 255);
						pBgraBuffer[nIndex + 2] = (unsigned char)(unB / 31.0 * 255);
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						unsigned short ushValue = ((pBuffer[1] << 8) | pBuffer[0]) & 32767; pBuffer += 2; lBufLen -= 2;
						unsigned char unR = ushValue & 31;            // 000000000011111
						unsigned char unG = (ushValue & 992)   >> 5;  // 000001111100000
						unsigned char unB = (ushValue & 31744) >> 10; // 111110000000000

						pBgraBuffer[nIndex + 0] = (unsigned char)(unR / 31.0 * 255);
						pBgraBuffer[nIndex + 1] = (unsigned char)(unG / 31.0 * 255);
						pBgraBuffer[nIndex + 2] = (unsigned char)(unB / 31.0 * 255);
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return false;
		}
		else if (BI_BITCOUNT_5 == ushBitCount)
		{
			// ���������� ������� ������ (��� �� �����)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

			if (BI_RGB != unCompression)
				return false; // TODO: ������� ������ �������, ��� ������ ����� ����� � ������ �����

			// ��������� ���� ��������
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 1 ���� �� ������ �����
			// (������ * 3) ������ ���� ������ 4.
			int nAdd = 0;
			while (0 != div_t(div(3 * nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			int nSize = nWidth * nHeight * 4;

			pBgraBuffer = new BYTE[nWidth * nHeight * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)nWidth;

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);

						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * (nWidth * nY + nX);
						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}
		else if (BI_BITCOUNT_6 == ushBitCount)
		{
			// ���������� ������� ������ (��� �� �����)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

			if (BI_RGB != unCompression)
				return false; // TO DO: ������� ������ �������, ��� ������ ����� ����� � ������ �����

			// ��������� ���� ��������
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if (lCalcLen != lBufLen)
				return false;

			// 1 ���� �� ������ �����
			// ������ ������ ���� ������ 4.
			int nAdd = 0;
			while (0 != div_t(div(nWidth + nAdd, 4)).rem)
			{
				nAdd++;
			}

			int nSize = nWidth * nHeight * 4;

			pBgraBuffer = new BYTE[(nWidth + nAdd) * nHeight * 4 * sizeof(BYTE)];
			if (NULL == pBgraBuffer)
				return false;

			ulHeight = (unsigned short)abs(nHeight);
			ulWidth  = (unsigned short)(nWidth + nAdd);

			if (nHeight < 0)
			{
				for (int nY = 0; nY < abs(nHeight); nY++)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // ���� ����� �������� �� ��������, ����� ��� ���������� ������ ����������
					}
					for (int nX = nWidth; nX < nWidth + nAdd; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						pBgraBuffer[nIndex + 0] = 255;
						pBgraBuffer[nIndex + 1] = 255;
						pBgraBuffer[nIndex + 2] = 255;
						pBgraBuffer[nIndex + 3] = 0;
					}
				}
			}
			else
			{
				for (int nY = abs(nHeight) - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);
						pBgraBuffer[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pBgraBuffer[nIndex + 3] = 255; pBuffer++; lBufLen--; // ���� ����� �������� �� ��������, ����� ��� ���������� ������ ����������
					}

					for (int nX = nWidth; nX < nWidth + nAdd; nX++)
					{
						int nIndex = 4 * ((nWidth + nAdd) * nY + nX);

						pBgraBuffer[nIndex + 0] = 255;
						pBgraBuffer[nIndex + 1] = 255;
						pBgraBuffer[nIndex + 2] = 255;
						pBgraBuffer[nIndex + 3] = 0;
					}

				}
			}

			*ppDstBuffer = pBgraBuffer;
			*pulWidth    = ulWidth;
			*pulHeight   = ulHeight;

			return true;
		}

		return false;
	}
	void ReadImage(BYTE* pHeaderBuffer, unsigned long ulHeaderBufferLen, BYTE* pImageBuffer, unsigned long ulImageBufferLen, BYTE** ppDstBuffer, unsigned long* pulWidth, unsigned long* pulHeight)
	{
		if (ulHeaderBufferLen <= 0 || NULL == pHeaderBuffer || NULL == pImageBuffer || ulImageBufferLen < 0)
			return;

		CDataStream oHeaderStream;
		oHeaderStream.SetStream(pHeaderBuffer, ulHeaderBufferLen);

		// ��������� ���������
		unsigned long ulHeaderSize;
		oHeaderStream >> ulHeaderSize;

		if (ulHeaderSize < 0x0000000C)
			return;
		else if (0x0000000C == ulHeaderSize) // BitmapCoreHeader
			ReadImageCoreHeader(pHeaderBuffer + 4, ulHeaderBufferLen - 4, pImageBuffer, ulImageBufferLen, ppDstBuffer, pulWidth, pulHeight);
		else // BitmapInfoHeader
			ReadImageInfoHeader(pHeaderBuffer + 4, ulHeaderBufferLen - 4, pImageBuffer, ulImageBufferLen, ppDstBuffer, pulWidth, pulHeight);
	}
}