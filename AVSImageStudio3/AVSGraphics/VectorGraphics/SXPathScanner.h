#ifndef _SXPATH_SCANNER_H
#define _SXPATH_SCANNER_H

#include "STypes.h"

class SXPath;
class SBitmap;
struct SIntersect;
class CPixels;

//-------------------------------------------------------------------------------------------------------------------------------
// SXPathScanner
//-------------------------------------------------------------------------------------------------------------------------------

class SXPathScanner 
{
public:

	// <pXPath> ������ ���� ������������.
	SXPathScanner(SXPath *pXPath, BOOL bEO);

	~SXPathScanner();

	// ���������� BBox ��� Path.
	void GetBBox(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
    { 
		*pnMinX = m_nMinX; 
		*pnMinY = m_nMinY; 
		*pnMaxX = m_nMaxX; 
		*pnMaxY = m_nMaxY; 
	}

	// ���������� BBox ��� Path � ������ AntiAliasing.
	void GetBBoxAA(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY);

	// ���������� ����������� � ������������ �������� X ��� ���������� Y.
	void GetSpanBounds(int nY, int *pnSpanMinX, int *pnSpanMaxX);

	// True, ���� ����� (<x>,<y>) ����� ������ Path.
	BOOL IsInsidePath(int nX, int nY);

	// True, ���� ���� �������� ([<nX0>,<nX1>], <nY>) ����� ������ Path.
	BOOL IsInsidePath(int x0, int x1, int y);

	// ���������� ��������� ���������, ������� ������ Path ��� <nY>. ���� <nY> ����������, �� �������� � ���������� ������� �������
	// GetNextSpan, ����� ���������� ������ ���������� ��� ������� <nY>. ���������� FALSE, ���� ��� ������ ����������� ��� <nY>.
	BOOL GetNextSpan(int nY, int *pnX0, int *pnX1);

	// �������� ���� AA-����� � <pAABuffer>. ������� ���������� ����������� � ������������ �������� ��������� �������� � ����������
	// ����������.
	void RenderAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);
	void RenderAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY);

	// �������� ��-�����, ������� �������� ��������. �� �����, ��� ��������� ������� ��������� ������ ���������� [<nX0>,<nX1>]. 
	// ������� ��������� �������� ���������� <nX0> � <nX1>.
	void ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);
	void ClipAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY);

private:

	void ComputeIntersections(int nY);

private:

	SXPath     *m_pXPath; 
	BOOL        m_bEO;
	int         m_nMinX;       //
	int         m_nMinY;       // BBox ��� XPath
	int         m_nMaxX;       //
	int         m_nMaxY;       //

	int         m_nInterY;     // ������� �������� Y
	int         m_nInterIndex; // ������� ����� � <inter> - ������������ � GetNextSpan 
	int         m_nInterCount; // ������� EO/NZWN ������� - ������������ � GetNextSpan 
	int         m_nXPathIndex; // ������� ����� � <m_pXPath> - ������������� � ComputeIntersections
	SIntersect *m_pInter;      // ������ ����������� ��� <m_nInterY>
	int         m_nInterLen;   // ���������� ��������� � ������� <m_pInter>
	int         m_nInterSize;  // ������ ������� <inter>
};

#endif /* _SXPATH_SCANNER_H */
