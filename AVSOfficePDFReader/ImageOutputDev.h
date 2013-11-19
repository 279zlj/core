#ifndef _SIMAGE_OUTPUTDEV_H
#define _SIMAGE_OUTPUTDEV_H

#include "STypes.h"
#include "OutputDevice.h"
#include "GState.h"

class Gr8BitFont;
class SBitmap;
class SImage;
class SPath;
class SPattern;
class SFontEngine;
class SFont;
class T3FontCache;
struct T3FontCacheTag;
struct T3GlyphStack;
struct TransparencyGroup;

//-------------------------------------------------------------------------------------------------------------------------------

// ������������ ������ ���� ��� �������� Type3-�������
#define T3FontCacheSize 8

//-------------------------------------------------------------------------------------------------------------------------------
// SImageOutputDev
//-------------------------------------------------------------------------------------------------------------------------------

class SImageOutputDev: public OutputDev 
{
public:

	SImageOutputDev(GlobalParams *pGlobalaParams, SColorMode eColorMode, int nBitmapRowPad, BOOL bReverseVideo, SColorPointer pPaperColor, BOOL nBitmapTopDown = TRUE, BOOL bAllowAA = TRUE);

	virtual ~SImageOutputDev();

	virtual BOOL UpSideDown() 
	{ 
		return TRUE; 
	}

	virtual BOOL UseDrawChar() 
	{ 
		return TRUE; 
	}

	virtual BOOL InterpretType3Chars() 
	{ 
		return TRUE; 
	}

	//---------------------------------------------------------------------------------------------------------------------------

	virtual void StartPage(int nPageIndex, GrState *pGState);

	virtual void EndPage();

	//----- Save/Restore GState
	virtual void SaveGState(GrState *pGState);
	virtual void RestoreGState(GrState *pGState);

	//----- ��������� ���������� � GState
	virtual void UpdateAll(GrState *pGState);
	virtual void UpdateCTM(GrState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32);
	virtual void UpdateLineDash(GrState *pGState);
	virtual void UpdateFlatness(GrState *pGState);
	virtual void UpdateLineJoin(GrState *pGState);
	virtual void UpdateLineCap(GrState *pGState);
	virtual void UpdateMiterLimit(GrState *pGState);
	virtual void UpdateLineWidth(GrState *pGState);
	virtual void UpdateStrokeAdjust(GrState *pGState);
	virtual void UpdateFillColor(GrState *pGState);
	virtual void UpdateStrokeColor(GrState *pGState);
	virtual void UpdateBlendMode(GrState *pGState);
	virtual void UpdateFillOpacity(GrState *pGState);
	virtual void UpdateStrokeOpacity(GrState *pGState);

	//----- ��������� ��������� ����������
	virtual void UpdateFont(GrState *pGState);

	//----- ��������� Path
	virtual void Stroke(GrState *pGState);
	virtual void Fill(GrState *pGState);
	virtual void EoFill(GrState *pGState);

	//----- Path clipping
	virtual void Clip(GrState *pGState);
	virtual void EoClip(GrState *pGState);
	virtual void ClipToStrokePath(GrState *pGState);

	//----- ����� ������
	virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen);
	virtual BOOL BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen);
	virtual void EndType3Char(GrState *pGState);
	virtual void EndTextObject(GrState *pGState);

	//----- ��������� Type 3 - �������
	virtual void Type3D0(GrState *pGState, double dWx, double dWy);
	virtual void Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy);

	//----- ����� ��������
	virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage);
	virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, BOOL inlineImg);
	virtual void DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert);
	virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);

	//----- Transparency groups � SMasks
	virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask);
	virtual void EndTransparencyGroup(GrState *pGState);
	virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox);
	virtual void SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunc, GrColor *pBackdropColor);
	virtual void ClearSoftMask(GrState *pGState);

	//----- �������������� ������� ��� ������� ����������

	// ��� ������� ����� �������, ����� ����������, ��� ��� ������ ����� PDF-��������.
	void     NewPDF(XRef *pXref);
 
	void     SetPaperColor(SColorPointer pPaperColor);
	BOOL     IsReverseVideo() 
	{ 
		return m_bReverseVideo; 
	}
	void     SetReverseVideo(BOOL bReverseVideo) 
	{ 
		m_bReverseVideo = bReverseVideo; 
	}

	SBitmap *GetBitmap() 
	{ 
		return m_pBitmap; 
	}
	int      GetBitmapWidth();
	int      GetBitmapHeight();
	SBitmap *TakeBitmap();
	SImage  *GetSImage() 
	{ 
		return m_pSImage; 
	}

	void     GetModRegion(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY);
	void     ClearModRegion();
	void     SetFillColor(int nR, int nG, int nB);
	SFont   *GetFont(StringExt *seName, double *pTextMatrix);
	SFont   *GetCurrentFont() 
	{ 
		return m_pFont; 
	}
	virtual BOOL GetVectorAntialias();
	virtual void SetVectorAntialias(BOOL bVectorAA);

private:

	void SetupScreenParams(double dHorDPI, double dVerDPI);
	SPattern *GetColor(GrGray oGray, GrRGB *pRGB);
	SPath *ConvertPath(GrState *pGState, GrPath *pPath);
	void DoUpdateFont(GrState *pGState);
	void DrawType3Glyph(T3FontCache *pT3Font, T3FontCacheTag *pT3Tag, unsigned char *pData);
	static BOOL ImageMaskSrc  (void *pData, SColorPointer pColorLine);
	static BOOL ImageSrc      (void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);
	static BOOL AlphaImageSrc (void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);
	static BOOL MaskedImageSrc(void *pData, SColorPointer pColorLine, unsigned char *pAlphaLine);

private:
	
	SColorMode    m_eColorMode;      // �������� �������� ������

	int           m_nBitmapRowPad;   //
	BOOL          m_bBitmapTopDown;  // �������� ���� ������ ����? 

	BOOL          m_bAllowAA;        //
	BOOL          m_bVectorAA;       //
	BOOL          m_bReverseVideo;   // Reverse video mode
	SColor        m_pPaperColor;     // ���� ������(��������/������� ����)
	SScreenParams m_pScreenParams;   // ��������� ������ (������ �������, ����� ������/������� ������ � �.�.)

	XRef         *m_pXref;           // ������� Xref ��� ������� PDF-���������

	SBitmap      *m_pBitmap;         // �������� ������, �� ������� �� ��� ��������
	SImage       *m_pSImage;         // �������� ������ ��� ���������
	SFontEngine  *m_pFontEngine;     // ������ ��� ������� ������

	T3FontCache  *m_pT3FontCache[T3FontCacheSize]; // ��� �� �������� ���� Type 3
	int           m_nT3FontsCount;   // ���������� ��������� ������� �� m_pT3FontCache
	T3GlyphStack *m_pT3GlyphStack;   // ���� � ��������� ��� Type3-������

	SFont        *m_pFont;           // ������� �����
	BOOL          m_bNeedFontUpdate; // ���� True, ����� ������� ����� ����� ��������

	SPath        *m_pTextClipPath;   // Clipping path ( ����������� �� ������ )

	TransparencyGroup *m_pTranspGroupStack; // ���� Transparency group

};

#endif /* _SIMAGE_OUTPUTDEV_H */
