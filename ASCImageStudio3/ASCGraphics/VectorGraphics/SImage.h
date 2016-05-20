#ifndef _SIMAGE_H
#define _SIMAGE_H

#include "STypes.h"
#include "SClip.h"

class SImage;
class SBitmap;
//struct SGlyphBitmap;
class SState;
class SPattern;
class SScreen;
class SPath;
class SXPath;
//class SFont;
struct SPipe;

//-------------------------------------------------------------------------------------------------------------------------------

// ��������� ��������� ������ �� pPixels � Image mask.
typedef BOOL (*SImageMaskSource)(void *pData, SColorPointer pPixels);

// ��������� ��������� ������ �� pPixels � Image.
typedef BOOL (*SImageSource)(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);

//-------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
// SImage
//-------------------------------------------------------------------------------------------------------------------------------

class SImage 
{
public:

	SImage(SBitmap *pBitmap, BOOL bVectorAA, SScreenParams *pScreenParams = NULL);
	SImage(SBitmap *pBitmap, BOOL bVectorAA, SScreen *pScreen);

	~SImage();

	//-- ��������� ������ �� �������� SState ------------------------------------------------------------------------------------

	double    *GetMatrix();
	SPattern  *GetStrokePattern();
	SPattern  *GetFillPattern();
	SScreen   *GetScreen();
	SBlendFunc GetBlendFunc();
	double     GetStrokeAlpha();
	double     GetFillAlpha();
	double     GetLineWidth();
	int        GetLineCap();
	int        GetLineJoin();
	double     GetMiterLimit();
	double     GetFlatness();
	double    *GetLineDash();
	int        GetLineDashLength();
	double     GetLineDashPhase();
	SClip     *GetClip();
	SBitmap   *GetSoftMask();
	BOOL       GetInNonIsolatedGroup();

	//-- �������� ��������� �������� SState -------------------------------------------------------------------------------------

	void SetMatrix(double *pMatrix);
	void SetStrokePattern(SPattern *pPattern);
	void SetFillPattern(SPattern *pPattern);
	void SetScreen(SScreen *pScreen);
	void SetBlendFunc(SBlendFunc pFunction);
	void SetStrokeAlpha(double dAlpha);
	void SetFillAlpha(double dAlpha);
	void SetLineWidth(double dLineWidth);
	void SetLineCap(int nLineCap);
	void SetLineJoin(int nLineJoin);
	void SetMiterLimit(double dMiterLimit);
	void SetFlatness(double dFlatness);
	void SetLineDash(double *pLineDash, int nLineDashLength, double dLineDashPhase); // ������ <pLineDash> ����������
	void SetStrokeAdjust(BOOL bStrokeAdjust);
	void ClipResetToRect(double dX0, double dY0, double dX1, double dY1);
	int  ClipToRect(double dX0, double dY0, double dX1, double dY1);
	int  ClipToPath(SPath *pPath, BOOL bEO);
	void SetSoftMask(SBitmap *pSMask);
	void SetInNonIsolatedGroup(SBitmap *pAlpha0Bitmap, int nAlpha0X, int nAlpha0Y);

	//-- ���������/��������������� SState (������� ����� ���� �������� SState) --------------------------------------------------

	void SaveState();
	int  RestoreState();

	//-- ������� ��������� ------------------------------------------------------------------------------------------------------

	// ��������� ������ ����� ������.
	void Clear(SColorPointer pColor, unsigned char unAlpha = 0x00);

	// ������� Path, ��������� ������� StrokePattern.
	int  Stroke(SPath *pPath);

	// �������� Path, ��������� ������� FillPattern.
	int  Fill(SPath *pPath, BOOL bEO);

	// �������� Path, ��������� �������� XOR.
	int  XorFill(SPath *pPath, BOOL bEO);

	// ������ ������, ��������� FillPattern.
	//int  FillChar(double dX, double dY, int nChar, SFont *pFont);

	// ������ ������, ��������� FillPattern. ��� ������� �� ����������� ������, �.�. ���������� pGlyph->bFreeData.
	//int  FillGlyph(double dX, double dY, SGlyphBitmap *pGlyph);

	// ������ �����, ��������� ������� FillColor. ������ nHeight ����� �� nWidth �������� �� pSrc. ���� �������� "1", ����� ������� ��������
	// � ������� ������, � ���� "0" - ������� ���������. ������� pMatrix ������ �������������� ���������� �������� � ������� (�������������) 
	// ������� ������� ��� ������ ��������.
	int  FillImageMask(SImageMaskSource pSrc, void *pSrcData, int nWidth, int nHeight, double *pMatrix, BOOL bGlyphMode);

	// ������ �����������. ������ nHeight ����� �� nWidth �������� �� pSrc. ��������������, ��� ������� ������������ eColorMode. ���� bAlpha = True,
	// ����� ���������� �������� �����, ������������ ������� pSrc; � ��������� ������, ���������� �����-�����.
	int  DrawImage(SImageSource pSrc, void *pSrcData, SColorMode eColorMode, BOOL bAlpha, int nWidth, int nHeight, double *pMatrix);

	// ��������� ������������� ������� �� pSrc � ������ SImage.
	int  Composite(SBitmap *pSrc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nWidth, int nHeight, BOOL bNoClip, BOOL bNonIsolated);

	// ��������� ������ SImage � ������� ������. ����� ��� �������� ����� �������������� ������ 1.
	void CompositeBackground(SColorPointer pColor);

	// �������� ������������� ������� �� pSrc �� ������ ������� SImage. �������� ����� ������ ��� ������������� ������� 0.
	int  BlitTransparent(SBitmap *pSrc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nWidth, int nHeight);

	//-- �������������� ������� -------------------------------------------------------------------------------------------------

	// ������ Path ��� �������, �� ������� Path ������� ����� �������, ��������� ��������� �����. ���� bFlatten = True, ����� ������� 
	// ������� Path(�� ������� m_dFlatten) � ���������� ������������ �����.
	SPath *MakeStrokePath(SPath *pPath, BOOL bFlatten = TRUE);

	// ���������� ������ ������� SImage.
	SBitmap *GetBitmap() 
	{ 
		return m_pBitmap; 
	}

	// ������� ���������� ������� �������, � ������� ������������� ��������� ���������.
	void GetModRegion(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
	{ 
		*pnMinX = m_nModRegMinX; 
		*pnMinY = m_nModRegMinY; 
		*pnMaxX = m_nModRegMaxX; 
		*pnMaxY = m_nModRegMaxY;
	}

	// ������������� ������� ������� � ���������� ����������� �� �������� �������.
	void ClearModRegion();

	// SClipResult ��������� �������� ���������.
	SClipResult GetClipResult() 
	{ 
		return m_pOpClipRes; 
	}

 
	BOOL GetVectorAA() 
	{
		return m_bVectorAA; 
	}
	void SetVectorAA(BOOL bVectorAA) 
	{ 
		m_bVectorAA = bVectorAA; 
	}

private:

	void PipeInit(SPipe *pPipe, int nX, int nY, SPattern *pPattern, SColorPointer pColorSrc, double dAlphaInput, BOOL bUsesShape, BOOL bNonIsolatedGroup);
	void PipeRun(SPipe *pPipe);
	void PipeSetXY(SPipe *pPipe, int nX, int nY);
	void PipeIncreaseX(SPipe *pPipe);
	void DrawPixel(SPipe *pPipe, int nX, int nY, BOOL bNoClip);
	void DrawAAPixelInit();
	void DrawAAPixel(SPipe *pPipe, int nX, int nY);
	void DrawSpan(SPipe *pPipe, int nX0, int nX1, int nY, BOOL bNoClip);
	void DrawAALine(SPipe *pPipe, int nX0, int nX1, int nY);

	void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
	void UpdateModX(int x);
	void UpdateModY(int y);

	
	void   StrokeNarrow(SPath *pPath);
	void   StrokeWide(SPath *pPath);
	SPath *FlattenPath(SPath *pPath, double *pMatrix, double dFlatness);
	void   FlattenCurve(double dX0, double dY0, double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double *pMatrix, double dFlatness_2, SPath *pFlatPath);
	SPath *MakeDashedPath(SPath *pPath);
	int    FillWithPattern(SPath *pPath, BOOL bEO, SPattern *pPattern, double dAlpha);
	//int    FillGlyph2(int nX, int nY, SGlyphBitmap *pGlyph);

private:

	static SPipeResultColorCtrl m_arrePipeResultColorNoAlphaBlend[];
	static SPipeResultColorCtrl m_arrePipeResultColorAlphaNoBlend[];
	static SPipeResultColorCtrl m_arrePipeResultColorAlphaBlend[];
	static int                  m_arrnPipeNonIsoGroupCorrection[];

private:

	SBitmap     *m_pBitmap;
	SState      *m_pState;
	SBitmap     *m_pAABuffer;
	int          m_nAABufferY;

	SBitmap     *m_pAlpha0Bitmap; // ��� Non-isolated �����, ������ �������� �������� �����-������
	int          m_nAlpha0X;      //
	int          m_nAlpha0Y;      // ����� � m_pAlpha0Bitmap

	double       m_arrdAAGamma[AntiAliasingSize * AntiAliasingSize + 1];

	int          m_nModRegMinX;  //
	int          m_nModRegMinY;  // ������� �������, � ������� ������������� ��������� ���������
	int          m_nModRegMaxX;  // (�.�. ��������� ����� ������ ������� GetModificationRegion())
	int          m_nModRegMaxY;  //

	SClipResult  m_pOpClipRes;   // ��������� ��������� �������� ��������� �� ��������� � Clipping

	BOOL         m_bVectorAA;

};

#endif /* _SIMAGE_H */
