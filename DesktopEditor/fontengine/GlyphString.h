#ifndef _BUILD_GLYPH_STRING_H
#define _BUILD_GLYPH_STRING_H

#include "../common/Types.h"
#include "../common/Array.h"

#include <math.h>
#include <string>

//-------------------------------------------------------------------------------------------------------------------------------
// TGlyphBitmap
//-------------------------------------------------------------------------------------------------------------------------------
class TGlyphBitmap 
{
public:
	int     nX;        // ����� �� X ��������� ����� ��� ��������� �������
	int     nY;        // ����� �� Y ��������� ����� ��� ��������� �������
	int     nWidth;    // ������ �������
	int     nHeight;   // ������ �������
	BOOL    bAA;       // Anti-aliased: True ��������, ��� Bitmap 8-������(�.�. � ������); False - Bitmap 1-������
	BYTE*	pData;     // Bitmap data(�������� � ��������)
	BOOL    bFreeData; // True, ���� ������ � pData ����� ����������

	TGlyphBitmap();
	~TGlyphBitmap();
};

enum EGlyphState
{
	glyphstateNormal = 0,  // ������ ����������� � ������ ������
	glyphstateDeafault,    // ������ ����������� � ���������� ������
	glyphstateMiss         // ������ �� �����������
};

struct TBBox
{ 
	float fMinX;
	float fMaxX;
	float fMinY;
	float fMaxY;
};

struct TMetrics
{
	float fWidth;
	float fHeight;

	float fHoriBearingX;
	float fHoriBearingY;
	float fHoriAdvance;

	float fVertBearingX;
	float fVertBearingY;
	float fVertAdvance;
};

class TGlyph
{
public:
	long         lUnicode; // ������
	float        fX;       // ������� �����
	float        fY;       // �� BaseLine

	float        fLeft;    //
	float        fTop;     // BBox
	float        fRight;   //
	float        fBottom;  //

	TMetrics		oMetrics;

	EGlyphState		eState;

	bool			bBitmap;
	TGlyphBitmap	oBitmap;

public:
	TGlyph();
	~TGlyph();
};


namespace FontConstants
{
	//---------------------------------------------------------------------------------------------------
	// ��������� ��������� � CharMap: Platform, Encoding ID
	//---------------------------------------------------------------------------------------------------

	const long c_lUniPlatform = 0;
	const long c_lMacPlatform = 1;
	const long c_lWinPlatform = 3;

	const long c_lWinEncSymbol   = 0;
	const long c_lWinEncUSC2     = 1;
	const long c_lWinEncShiftJIS = 2;
	const long c_lWinEncPRC      = 3;
	const long c_lWinEncBig5     = 4;
	const long c_lWinEncWansung  = 5;
	const long c_lWinEncJohab    = 6;
	const long c_lWinEncUCS4     = 10;

	//--------------------------------------------------------------------------
	// FontStyle: face types and common styles
	//--------------------------------------------------------------------------

	enum FontStyle
	{
		FontStyleRegular    = 0,
		FontStyleBold       = 1,
		FontStyleItalic     = 2,
		FontStyleBoldItalic = 3,
		FontStyleUnderline  = 4,
		FontStyleStrikeout  = 8
	};

	//--------------------------------------------------------------------------
	// Unit constants
	//--------------------------------------------------------------------------

	enum Unit
	{
		UnitWorld,      // 0 -- World coordinate (non-physical unit)
		UnitDisplay,    // 1 -- Variable -- for PageTransform only
		UnitPixel,      // 2 -- Each unit is one device pixel.
		UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
		UnitInch,       // 4 -- Each unit is 1 inch.
		UnitDocument,   // 5 -- Each unit is 1/300 inch.
		UnitMillimeter  // 6 -- Each unit is 1 millimeter.
	};
}

class CGlyphString
{
public:
	CGlyphString();
	CGlyphString(const std::wstring& wsString, float fX = 0, float fY = 0);
	~CGlyphString();
	void SetString(const std::wstring& wsString, float fX = 0, float fY = 0);
	void SetStringGID(const LONG& gid, float fX = 0, float fY = 0);
	void Reset();
	int  GetLength();
	void SetBBox(int nIndex, float fLeft, float fTop, float fRight, float fBottom);
	void SetMetrics(int nIndex, float fWidth, float fHeight, float fHoriAdvance, float fHoriBearingX, float fHoriBearingY, float fVertAdvance, float fVertBearingX, float fVertBearingY);
	void SetStartPoint(int nIndex, float fX, float fY);
	void SetState(int nIndex, EGlyphState eState);
	void GetBBox(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom, int nIndex = -1, int nType = 0);
	void GetBBox2(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom);
	void SetCTM(float fA, float fB, float fC, float fD, float fE ,float fF);
	void ResetCTM();
	void Transform(float *pfX, float *pfY);
	void SetTrans(float fX, float fY);
	TGlyph* GetAt(int nIndex);
	BOOL GetNext(TGlyph*& pGlyph);
public:
	float   m_fTransX;
	float   m_fTransY;

	float   m_fX; // ���������� ��������� ����� ��� ���������
	float   m_fY; //

	float   m_fEndX; // ���������� �������� �����
	float   m_fEndY; //

	double  m_arrCTM[6];     // ���������� ������� ��������������
	double  m_dIDet;         // (����������� ������� ��������������)^(-1)
	
private:
	TGlyph* m_pGlyphsBuffer; // ������� � ������ ������
	int     m_nGlyphsCount;  // ���������� �������� � ������
	int     m_nGlyphIndex;   // ����� �������� �������
};

#endif /* _BUILD_GLYPH_STRING_H */