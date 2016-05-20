#ifndef _FONT_H
#define _FONT_H

#include "GlyphBitmap.h"
#include "FontConsts.h"

struct TFontCacheTag;
class CFontFile;
class CFontPath;
class CGlyphString;

//-------------------------------------------------------------------------------------------------------------------------------
// CFont
//-------------------------------------------------------------------------------------------------------------------------------

class CFont 
{
public:

	CFont(CFontFile *pFontFile, BOOL bAntiAliasing, BOOL bUseKerning);

	// ������� ���� ��������� ���� ������������, ����� � ����������� ��������� ��� ��������� BBox.
	void InitCache();
	void ClearCache();

	virtual ~CFont();

	CFontFile *GetFontFile() 
	{ 
		return m_pFontFile; 
	}

	// ��������� ������������ �� FontFile � ��������.
	BOOL Matches(CFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix) 
	{
		// TO DO: �������� �������� �� FontFile
		return ( pFontFile == m_pFontFile && 
			     pFontMatrix[0] == m_arrdFontMatrix[0] && pFontMatrix[1] == m_arrdFontMatrix[1] && pFontMatrix[2] == m_arrdFontMatrix[2] && pFontMatrix[3] == m_arrdFontMatrix[3] &&
				 pTextMatrix[0] == m_arrdTextMatrix[0] && pTextMatrix[1] == m_arrdTextMatrix[1] && pTextMatrix[2] == m_arrdTextMatrix[2] && pTextMatrix[3] == m_arrdTextMatrix[3] );
	}
	BOOL Matches(CFontFile *pFontFile) 
	{
		return ( pFontFile == m_pFontFile );
	}

	// �������� ������ (��� bitmap) - ������� �������� ����� ��� � ����, ���� �� ����� ������� ����� Bitmap � ��������� ��� � ���.
	// �������� nFracX � nFracY, ������������ ��������� ���������� �������� �� [0, 1), ��� ����������� ����� sfontFraction = 1 << sfontFractionBits.
	// ����������� ������, �� ��������������, ������������ ���������� ������ ������� ������ �������� 0.
	virtual BOOL GetGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap);

	// ������� �� Glyph ������. �������� nFracX � nFracY ����� ��� �� �����, ��� � � GetGlyph.
	virtual BOOL MakeGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap) = 0;

	// ������� ���������� ������� �������� � ������, ������� � ����� (0;0)
	virtual BOOL GetString(CGlyphString *pString) = 0;

	// ������� ���������� ������� �������� � ������, ������� � ����� (0;0) � ����� ��������� ���� �������� ����
	virtual BOOL GetString2(CGlyphString *pString) = 0;

	// ������� ���������� CFontPath ��� ������� �������.
	virtual CFontPath *GetGlyphPath(int nCode) = 0;

	// ������� ������������ ��������, �������� �� �������� ������ ���������� ���������� ��� �������� ������
	virtual void SetStringGID(BOOL bGID) = 0;

	// ������� ���������� ��������, �������� �� �������� ������ ���������� ���������� ��� �������� ������
	virtual BOOL GetStringGID() = 0;

//----- ���������� ��������� ������ ----------------------------------------------------
	// ������� ���������� Ascent � �������� ������
	virtual short GetAscender() = 0;

	// ������� ���������� Descent � �������� ������
	virtual short GetDescender() = 0;

	// ������� ���������� ������ ����� 'M' � �������� ������
	virtual unsigned short GetUnitsPerEm() = 0;

	// ������� ���������� ���������� ����� ������� � �������� ������
	virtual short GetLineSpacing() = 0;

	// ������� ���������� �������� ��������� ������
	virtual char * GetFamilyName() = 0;

	// ������� ����������� ���������� �������, ���������� � �����
	virtual long GetFacesCount() = 0;

	// ������� ���������� ����� �������� ������ � �����
	virtual long GetFaceIndex() = 0;

	// ������� ���������� ���������� ������ � �����
	virtual long GetGlyphsCount() = 0;

	// ������� ���������� ����� ������ � ��������� ����� ("Regular", "Bold" � �.�.)
	virtual char *GetStyleName() = 0;

	// ������� ���������� ������� ����� ��� ������������� (������������ BaseLine)
	virtual short GetUnderlinePosition() = 0;

	// ������� ���������� ������� ����� �������������
	virtual short GetUnderlineThickness() = 0;

	// ������� ���������� ������������ ���������� �� ����������� ����� ���������� ������� ��������� ���� �������� ��������
	virtual short GetMaxAdvanceWidth() = 0;

	// ������� ���������� ������������ ���������� �� ��������� ����� ���������� ������� ��������� ���� �������� ��������
	virtual short GetMaxAdvanceHeight() = 0;

	// ������� ���������� ���������� �������������, � ������� ����� ���������� ����� ����� �� ������
	virtual void GetBBox(long *plMinX, long *plMinY, long *plMaxX, long *plMaxY) = 0;

	// ������� ���������� ����� ����� �� ��� �����
	virtual unsigned short GetNameIndex(char *sName) = 0;

//--------------------------------------------------------------------------------------

	// ������� ���������� true, ���� ������ � ��������� ��������� lUnicode ������������ � ������(� �����-������ CMap)
	virtual bool IsCharAvailable(long lUnicode) = 0;

	// ������������� Dpi
	virtual void SetSizeAndDpi(float fSize, unsigned int unHorDpi, unsigned int unVerDpi) = 0;

	// ������� ���������� ��� ������
	virtual const char *GetFontFormat() = 0;

	// ������� ��������� ������������ �� ������ Unicode Range � ������ ��� CodeRange(���� �������� un4ByteIndex ����� 4 ��� 5)
	virtual int IsUnicodeRangeAvailable(unsigned long ulBit, unsigned int un4ByteIndex) = 0;

	// ������� ���������� ����� ���� �� ������ �����
	virtual unsigned long GetCodeByGID(unsigned short ushGID) = 0;

	// ������� ���������� panose (� ������� OS/2)
	virtual void GetPanose(char **ppPanose) = 0;

	// ������� ��������� �������� �� ������ ����� ������������
	virtual bool IsFixedWidth() = 0;
//--------------------------------------------------------------------------------------

	// ���������� ������� �������������� � ���������
	virtual void ResetFontMatrix() = 0;

	// ���������� ���������� �������
	virtual void ResetTextMatrix() = 0;

	// ��������� �������������� (� ������� ������)
	virtual void ApplyTransform(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

	// ������������� ������� ������
	virtual void SetFontMatrix(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

	// ������������� ���������� ������� (����� ������� ��� ����� ������)
	virtual void SetTextMatrix(float fA, float fB, float fC, float fD, float fE, float fF) = 0;

//--------------------------------------------------------------------------------------
// ��������� �� ������ ����� ������� � ������ �����, ������� ������ �������� ����� 
// ������ �������.
//--------------------------------------------------------------------------------------

	// �������������, ����� �� ������ ����� ������(��������������)
	void SetBold(BOOL bBold)
	{
		m_bNeedDoBold = bBold;
	};

	// ���������, �������� �� ����� ������
	BOOL GetBold()
	{
		return m_bNeedDoBold;
	}

	// �������������, ����� �� ������ ����� ���������(��������������)
	void SetItalic(BOOL bItalic)
	{
		m_bNeedDoItalic = bItalic;
		ResetFontMatrix();
	}

	// ���������, �������� �� ����� ���������
	BOOL GetItalic()
	{
		return m_bNeedDoItalic;
	}
//--------------------------------------------------------------------------------------

	// ������������� ����������� �����
	void SetDefaultFont(CFont *pDefFont)
	{
		m_pDefaultFont = pDefFont;
	}


	// ������� ���������� ����������� �����
	CFont *GetDefaultFont()
	{
		return m_pDefaultFont;
	}

	void SetUseDefaultFont(BOOL bUse)
	{
		m_bUseDefaultFont = bUse;
	}

	BOOL GetUseDefaultFont()
	{
		return m_bUseDefaultFont;
	}


	// ������� ���������� FontMatrix
	double *GetMatrix() 
	{ 
		return m_arrdFontMatrix; 
	}


	// ������� ���������� BBox ��� Glyph.
	void GetBBox(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
	{ 
		*pnMinX = m_nMinX; 
		*pnMinY = m_nMinY; 
		*pnMaxX = m_nMaxX; 
		*pnMaxY = m_nMaxY; 
	}

	// ������������� ������������ ��������
	void SetCharSpacing(float fCharSpacing)
	{
		m_fCharSpacing = fCharSpacing;
	}

	// �������� ������������� ��������
	float GetCharSpacing()
	{
		return m_fCharSpacing;
	}
//--------------------------------------------------------------------------------------

	float GetFontSize()
	{
		return m_fSize;
	}
	unsigned int GetVerDpi()
	{
		return m_unVerDpi;
	}

	unsigned int GetHorDpi()
	{
		return m_unHorDpi;
	}

protected:

	CFontFile     *m_pFontFile;
	double         m_arrdFontMatrix[6]; // FontMatrix (Text space -> Device space)
	double         m_arrdTextMatrix[6]; // TextMatrix (Text space -> User space)
	BOOL           m_bAntiAliasing;     // Anti-aliasing
	BOOL           m_bUseKerning;       // ������������ � �������� �� Kerning

	float          m_fSize;             // ������ ������
	unsigned int   m_unHorDpi;          // �������������� ����������
	unsigned int   m_unVerDpi;          // ������������ ����������

	BOOL           m_bNeedDoItalic;     // ������ ����� ����� ������� ��������� | ����� ��������� � 2-� �������, ���� �� ������ ����� �� �����,
	BOOL           m_bNeedDoBold;       // ������ ����� ����� ������� ������    | ���� ����� ������������� ������� ��� ������ � ������ ������.

	float          m_fCharSpacing;      // ������������� ��������

	int            m_nMinX;        // 
	int            m_nMinY;        // Glyph BBox
	int            m_nMaxX;        //
	int            m_nMaxY;        //

	unsigned char *m_pCache;       // Glyph bitmap cache
	TFontCacheTag *m_pCacheTags;   // Cache Tags

	int            m_nGlyphWidth;  // ������ Glyph bitmaps   
	int            m_nGlyphHeight; // ������ Glyph bitmaps
	int            m_nGlyphSize;   // ������ Glyph bitmaps � ������

	int            m_nCacheSets;   // ���������� Sets � ����
	int            m_nCacheAssoc;  // ���������� Glyphs � Set

	CFont         *m_pDefaultFont;    // ����������� �����, ������� ������������, ���� � �������� �����-�� ������� �� ��������
	BOOL           m_bUseDefaultFont; // ���������� �� ����������� �����? ���� ���, ����� ����� ��� ������� ������ ����������� ������
};

#endif /* _FONT_H */
