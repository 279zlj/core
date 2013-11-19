#ifndef _SCLIP_H
#define _SCLIP_H

#include "STypes.h"
#include "SMathExt.h"

class SPath;
class SXPath;
class SXPathScanner;
class SBitmap;

//-------------------------------------------------------------------------------------------------------------------------------

enum SClipResult 
{
	clipAllInside,
	clipAllOutside,
	clipPartial
};

//-------------------------------------------------------------------------------------------------------------------------------
// SClip
//-------------------------------------------------------------------------------------------------------------------------------

class SClip
{
public:

	SClip(double dMinX, double dMinY, double dMaxX, double dMaxY, BOOL bAntialias);

	SClip *Copy() 
	{ 
		return new SClip(this); 
	}

	~SClip();

	void ResetToRect(double dX0, double dY0, double dX1, double dY1);

	// ���������� Clip � �������� ���������������.
	int ClipToRect(double dX0, double dY0, double dX1, double dY1);

	// ���������� Clip � <pPath>.
	int ClipToPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bEO);

	// ���������� TRUE, ���� (<dX>,<dY>) ����� ������ Clip.
	BOOL IsInsideClip(int dX, int dY);

	// ��������� ������������ ��������� �������������� ������������ Clip. ������������ ��������:
	//   - clipAllInside
	//     ���� ���� ������������� ���������� ������ Clipping Region, �.�. ��� ����� �������������� �������.
	//   - clipAllOutside
	//     ���� ������������ �� ����� ����������� � Clipping Region, �.�. ��� ����� ���������
	//   - clipPartial
	//     ���� ������������� �������� ����� ������ � �������� ������� ������������ Clipping Region
	SClipResult CheckRectangle(int dRectMinX, int dRectMinY, int dRectMaxX, int dRectMaxY);

	// ����������� �������, ������ ����������� � ��������������� ����������.
	SClipResult CheckSpan(int dSpanMinX, int dSpanMaxX, int nSpanY);

	// �������� ��-�����, ������� �������� ��������. �� �����, ��� ��������� ������� ��������� ������ ���������� [<nX0>,<nX1>]. 
	// ������� ��������� �������� ���������� <nX0> � <nX1>.
	void ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);

	int GetMinX() 
	{ 
		return m_nMinX; 
	}
	int GetMaxX() 
	{ 
		return m_nMaxX; 
	}
	int GetMinY() 
	{ 
		return m_nMinY; 
	}
	int GetMaxY() 
	{ 
		return m_nMaxY; 
	}

	int GetPathsCount() 
	{ 
		return m_nPathsCount; 
	}

private:

	SClip(SClip *pClip);
	void Resize(int nPathsCount);

private:

	BOOL            m_bAntiAlias;
  
	double          m_dMinX;
	double          m_dMinY;
	double          m_dMaxX;
	double          m_dMaxY;

	int             m_nMinX;
	int             m_nMinY;
	int             m_nMaxX;
	int             m_nMaxY;

	SXPath        **m_ppPaths;
	unsigned char  *m_pFlags;
	SXPathScanner **m_pScanners;
	int             m_nPathsCount;
	int             m_nSize;
};

#endif /* _SCLIP_H */
