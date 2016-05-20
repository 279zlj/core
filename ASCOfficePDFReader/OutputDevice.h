#ifndef _OUTPUT_DEVICE_H
#define _OUTPUT_DEVICE_H

#include "CharTypes.h"
#include "GlobalParams.h"

class StringExt;
class GrState;
class GrPath;
struct GrColor;
class GrColorSpace;
class GrImageColorMap;
class GrFunctionShading;
class GrAxialShading;
class GrRadialShading;
class Stream;
class Links;
class Link;
class Catalog;
class Page;
class Function;

//-------------------------------------------------------------------------------------------------------------------------------
// OutputDev
//-------------------------------------------------------------------------------------------------------------------------------

class OutputDev 
{
public:

	OutputDev() {}

	virtual ~OutputDev() {}

	//------ ���������� � �������� ����������

	// True, ���� ����� � ����������� (0,0) ��������� � ����� ������� ����.
	virtual BOOL UpSideDown() = 0;

	// ����� ������ � ������� ������� DrawChar() ��� DrawString()?
	virtual BOOL UseDrawChar() = 0;

	// ������������ �� ������ ���������� TilingPatternFill?  ���� ���, �����
	// TilingPatternFill ����� ���������� � ������� ���������� ����� �������
	// ����������� ����������.
	virtual BOOL UseTilingPatternFill() 
	{ 
		return FALSE; 
	}

	// ������������ �� ������ ���������� FunctionShadedFill? ���� ���, ����� ������
	// Shaded fills ����� �������� ����������� ����� ������� ����������� ����������.
	virtual BOOL UseFunctionalShadedFills()
	{
		return FALSE;
	}
	// ������������ �� ������ ���������� AxialShadedFill? ���� ���, ����� ������
	// Shaded fills ����� �������� ����������� ����� ������� ����������� ����������.
	virtual BOOL UseAxialShadedFills()
	{
		return FALSE;
	}
	// ������������ �� ������ ���������� RadialShadedFill? ���� ���, ����� ������
	// Shaded fills ����� �������� ����������� ����� ������� ����������� ����������.
	virtual BOOL UseRadialShadedFills()
	{
		return FALSE;
	}

	// �������������� �� DrawForm()?
	virtual BOOL UseDrawForm() 
	{
		return FALSE; 
	}

	// ���������� ������� ClipToPath ��� ����� ��� ������� Clip, EoClip, ClipToPathStroke
	virtual BOOL UseClipTo()
	{
		return FALSE;
	}


	// ���������� �� ������ ���������� ������ BeginType3Char/EndType3Char?
	// � ��������� ������ ���� � ������ Type3 ����� ��������� � �������
	// ������� DrawChar/DrawString.
	virtual BOOL InterpretType3Chars() = 0;

	// �������������� �� ������ ����������� ���-������ ����� ������?
	virtual BOOL NeedNonText() 
	{ 
		return TRUE; 
	}

	// �������������� �� ������ ����������� ������������� ������� � ������� Path
	virtual BOOL UseFillAndStroke()
	{
		return FALSE;
	}

	// � ����� ���� �� ���������� ������ ���������� ��������
	virtual BOOL UseSimpleTransparentGroup()
	{
		return FALSE;
	}

	//------ ���������� � �������� ����������

	// ������������� ����������� ������� ��������������.
	virtual void SetDefaultCTM(double *pCTM);

	// ���������, ������ �� �� ���������� ������ ����� ��������. ���� �����������
	// �������� False, ����� �������� �����������. ��� �������, OutputDev ����� 
	// ������������ �������������� ��������, ����� �������� ��������, ������ ���
	// ������ �������� False.
	virtual BOOL CheckPageSlice(Page *pPage, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, int nSliceX, int nSliceY, int nSliceW, int nSliceH, BOOL bPrinting, Catalog *pCatalog, BOOL (*pAbortCheckCallBack)(void *pData) = NULL, void *pAbortCheckData = NULL)
    { 
		return TRUE; 
	}

	// ����� ��������.
	virtual void StartPage(int nPageNum, GrState *pGState) {}
	// ����� ��������.
	virtual void EndPage() {}

	// ����� ����������� ��������.
	virtual void Dump() {}

	// ��������� ������
	virtual BOOL IsStopped() { return FALSE ;}

	//----- ������� ��������� (���������������� � ����������)

	// �����������
	virtual void ConvertDeviceToUser(double dDevX, double dDevY, double *pdUserX, double *pdUserY);
	virtual void ConvertUserToDevice(double dUserX, double dUserY, int *pnDevX, int *pnDevY);

	double *GetDefaultCTM() 
	{ 
		return m_arrDefaultCTM; 
	}
	double *GetDefaultInvCTM() 
	{ 
		return m_arrDefaultInvCTM; 
	}

	//----- Save/Restore GState
	virtual void SaveGState(GrState *pGState) {}
	virtual void RestoreGState(GrState *pGState) {}

	//----- ��������� ���������� � GState
	virtual void UpdateAll(GrState *pGState);
	virtual void UpdateCTM(GrState *pGState, double dM11, double dM12, double dM21, double dM22, double dM31, double dM32) {}
	virtual void UpdateLineDash(GrState *pGState) {}
	virtual void UpdateFlatness(GrState *pGState) {}
	virtual void UpdateLineJoin(GrState *pGState) {}
	virtual void UpdateLineCap (GrState *pGtate) {}
	virtual void UpdateMiterLimit(GrState *pGState) {}
	virtual void UpdateLineWidth(GrState *pGState) {}
	virtual void UpdateStrokeAdjust(GrState *pGState) {}
	virtual void UpdateFillColorSpace(GrState *pGState) {}
	virtual void UpdateStrokeColorSpace(GrState *pGState) {}
	virtual void UpdateFillColor(GrState *pGState) {}
	virtual void UpdateStrokeColor(GrState *pGState) {}
	virtual void UpdateBlendMode(GrState *pGState) {}
	virtual void UpdateFillOpacity(GrState *pGState) {}
	virtual void UpdateStrokeOpacity(GrState *pGState) {}
	virtual void UpdateFillOverprint(GrState *pGState) {}
	virtual void UpdateStrokeOverprint(GrState *pGState) {}
	virtual void UpdateTransfer(GrState *pGState) {}

	//----- ��������� ��������� ����������
	virtual void UpdateFont(GrState *pGState) {}
	virtual void UpdateTextMatrix(GrState *pGState) {}
	virtual void UpdateCharSpace(GrState *pGState) {}
	virtual void UpdateRender(GrState *pGState) {}
	virtual void UpdateRise(GrState *pGState) {}
	virtual void UpdateWordSpace(GrState *pGState) {}
	virtual void UpdateHorizScaling(GrState *pGState) {}
	virtual void UpdateTextPos(GrState *pGState) {}
	virtual void UpdateTextShift(GrState *pGState, double dShift) {}

	//----- ��������� Path
	virtual void Stroke(GrState *pGState) {}
	virtual void Fill(GrState *pGState) {}
	virtual void EoFill(GrState *pGState) {}
	virtual void FillStroke(GrState *pGState) {}
	virtual void EoFillStroke(GrState *pGState) {}
	virtual void TilingPatternFill(GrState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep) {}
	virtual void StartTilingFill  (GrState *pGState) {}
	virtual void EndTilingFill    ()                 {}
	virtual BOOL FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading)
    { 
		return FALSE; 
	}
	virtual BOOL AxialShadedFill   (GrState *pGState, GrAxialShading    *pShading)
    { 
		return FALSE; 
	}
	virtual BOOL RadialShadedFill  (GrState *pGState, GrRadialShading   *pShading)
    { 
		return FALSE; 
	}


	virtual void StartShadedFill(GrState *pGState) {}
	virtual void EndShadedFill  ()                 {}
	
	//----- Path clipping
	virtual void Clip(GrState *pGState) {}
	virtual void ClipAttack(GrState *pGState) {}
	virtual void EoClip(GrState *pGState) {}
	virtual void ClipToStrokePath(GrState *pGState) {}
	virtual void ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, BOOL bEO) {}

	//----- ����� ������
	virtual void BeginStringOperator(GrState *pGState) {}
	virtual void EndStringOperator(GrState *pGState) {}
	virtual void BeginString(GrState *pGState, StringExt *seString) {}
	virtual void EndString(GrState *pGState) {}
	virtual void DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pnUnicode, int nUnLen) {}
	virtual void DrawString(GrState *pGState, StringExt *seString) {}
	virtual BOOL BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pnUnicode, int nUnLen); 
	virtual void EndType3Char(GrState *pGState) {}
	virtual void BegintTextObject(GrState *pGState){}
	virtual void EndTextObject(GrState *pGState) {}

	// begin/end commands
	virtual void BeginCommand(const ULONG& lCommand){}
	virtual void EndCommand(const ULONG& lCommand) {}

	// detect support futures
	virtual bool IsSupportFuture(const CString& sName){ return false; }
	virtual void SetAdditionalParam(const CString& sName, VARIANT& var) {}

	//----- ����� ��������
	virtual void DrawImageMask      (GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage);
	virtual void DrawImage          (GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, BOOL bInlineImage);
	virtual void DrawMaskedImage    (GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert);
	virtual void DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap);

#if OPI_SUPPORT
	//----- OPI
	virtual void OpiBegin(GrState *pGState, Dict *pOpiDict);
	virtual void OpiEnd  (GrState *pGState, Dict *pOpiDict);
#endif

	//----- ��������� Type 3 - �������
	virtual void Type3D0(GrState *pGState, double dWx, double dWy) {}
	virtual void Type3D1(GrState *pGState, double dWx, double dWy, double dLLx, double dLLy, double dURx, double dURy) {}

	//----- Form XObjects
	virtual void DrawForm(Ref oID) {}

	//----- PostScript XObjects
	virtual void PSXObject(Stream *pPSStream, Stream *pLevel1Stream) {}

	//----- Transparency groups � SMasks
	virtual void BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask) {}
	virtual void EndTransparencyGroup(GrState *pGState) {}
	virtual void PaintTransparencyGroup(GrState *pGState, double *pBBox) {}
	virtual void SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunction, GrColor *pBackdropColor) {}
	virtual void ClearSoftMask(GrState *pGState) {}

	//----- Links
	virtual void ProcessLink(Link *pLink, Catalog *pCatalog) {}

	virtual BOOL GetVectorAntialias() 
	{ 
		return FALSE; 
	}
	virtual void SetVectorAntialias(BOOL bVectorAntiAlias) {}

private:

	double        m_arrDefaultCTM[6];    // ������� �������������� �� ���������
	double        m_arrDefaultInvCTM[6]; // �������� �������

protected:

	GlobalParams *m_pGlobalParams;
};

#endif /* _OUTPUT_DEVICE_H */
