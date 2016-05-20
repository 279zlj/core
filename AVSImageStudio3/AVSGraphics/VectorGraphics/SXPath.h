#ifndef _SXPATH_H
#define _SXPATH_H

#include "STypes.h"

class SPath;
struct SXPathAdjust;

//-------------------------------------------------------------------------------------------------------------------------------

#define MaxCurveSplits (1 << 10)

//-------------------------------------------------------------------------------------------------------------------------------
// SXPathSegment
//-------------------------------------------------------------------------------------------------------------------------------

struct SXPathSegment 
{
	double       dFirstX;  // ���������� ������ �����
	double       dFirstY;  //
	double       dSecondX; // ���������� ������ �����
	double       dSecondY; //
	double       dDxDy;    // ������ ���� �������: Dx / Dy 
	double       dDyDx;    // ������ ���� �������: Dy / Dx
	unsigned int unFlags;
};

#define XPathFirst   0x01	// ������ ������� SubPath
#define XPathLast    0x02	// ��������� ������� SubPath
#define XPathEnd0    0x04	// ������ �����(���������) ��������� SubPath
#define XPathEnd1    0x08 // ������ �����(���������) ��������� SubPath
#define XPathHoriz   0x10 // �������������� ������� (dSecondY == dSecondX) (dDxDy ������������)
#define XPathVert    0x20 // ������������ ������� (dFirstY == dFirstX) (dDyDx ������������)
#define XPathFlip	   0x40	// ������������ ������� dFirstY > dSecondY

//-------------------------------------------------------------------------------------------------------------------------------
// SXPath
//-------------------------------------------------------------------------------------------------------------------------------

class SXPath 
{
public:

	// ��������� � ���������� <pPath>. ��������� ��� �� ���������������� ��������� � ���������� ��������� � ������� ������� <pMatrix>.
	// ���� <bCloseSubpaths> == true, ��������� ��� �������� SubPaths.
	SXPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bCloseSubpaths);

	SXPath *Copy() 
	{ 
		return new SXPath(this); 
	}

	~SXPath();

	// �������� ��� ���������� �� AntiAliasingSize.
	void AntiAliasingScale();

	void Sort();

private:

	SXPath();
	SXPath(SXPath *pXPath);
	void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
	void StrokeAdjust(SXPathAdjust *pAdjust, double *pdX, double *pdY);
	void Resize(int nSegmentsCount);
	void AddCurve(double dX0, double dY0, double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dFlatness, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1);
	void AddSegment(double dX0, double dY0, double dX1, double dY1, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1);

private:

	SXPathSegment *m_pSegments;      // ������ ���������
	int            m_nSegmentsCount; // ���������� ���������
	int            m_nSize;          // ������ ������� m_pSegments

	friend class SXPathScanner;
	friend class SClip;
	friend class SImage;
	friend class CAVSGraphics;
};

#endif /* _SXPATH_H */
