#ifndef _SBITMAP_H
#define _SBITMAP_H

#include "STypes.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SBitmap
//-------------------------------------------------------------------------------------------------------------------------------

class SBitmap 
{
public:

	SBitmap(int nWidth, int nHeight, int nRowPad, SColorMode eMode, BOOL bAlpha, BOOL bTopDown = TRUE);

	~SBitmap();

	int GetWidth() 
	{ 
		return m_nWidth; 
	}
	int GetHeight() 
	{ 
		return m_nHeight; 
	}
	int GetStride() 
	{ 
		return m_nStride; 
	}
	int GetAlphaStride() 
	{ 
		return m_nWidth; 
	}
	SColorMode GetColorMode() 
	{ 
		return m_eMode; 
	}
	SColorPointer GetData() 
	{ 
		return m_pData; 
	}
	unsigned char *GetAlpha() 
	{ 
		return m_pAlpha; 
	}

	int WritePNMFile(wchar_t *wsFileName);
	int WriteBMPFile(wchar_t *wsFileName);
	int WriteJPGFile(wchar_t *wsFileName, wchar_t *wsTempFolder);

	void GetPixel(int nX, int nY, SColorPointer pPixel);
	unsigned char GetAlpha(int nX, int nY);

private:

	int            m_nWidth;    // ������ ��������
	int            m_nHeight;   // ������ ��������

	int            m_nStride;   // ���������� ���� ����� ������� ����� ������ � ������� ���������, 
	                            // ���� �������� �������������, ������ �������� ����������� (flip)
	SColorMode     m_eMode;     // Color mode
	SColorPointer  m_pData;     // ��������� �� ������ ������ ��������
	unsigned char *m_pAlpha;   	// ��������� �� ������ ������ ����� ������(�� ������� �� flip)

	friend class   SImage;
};

#endif /* _SBITMAP_H */
