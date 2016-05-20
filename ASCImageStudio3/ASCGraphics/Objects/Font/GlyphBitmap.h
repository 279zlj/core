#ifndef _GLYPH_BITMAP_H
#define _GLYPH_BITMAP_H

//-------------------------------------------------------------------------------------------------------------------------------
// TGlyphBitmap
//-------------------------------------------------------------------------------------------------------------------------------

struct TGlyphBitmap 
{
	int            nX;        // ����� �� X ��������� ����� ��� ��������� �������
	int            nY;        // ����� �� Y ��������� ����� ��� ��������� �������
	int            nWidth;    // ������ �������
	int            nHeight;   // ������ �������
	BOOL           bAA;       // Anti-aliased: True ��������, ��� Bitmap 8-������(�.�. � ������); False - Bitmap 1-������
	unsigned char *pData;     // Bitmap data(�������� � ��������)
	BOOL           bFreeData; // True, ���� ������ � pData ����� ����������

	TGlyphBitmap()
	{
		nX			= 0;
		nY			= 0;
		nWidth		= 0;
		nHeight		= 0;
		bAA			= FALSE;
		pData		= NULL;
		bFreeData	= NULL;
	}
};

#endif /* _GLYPH_BITMAP_H */