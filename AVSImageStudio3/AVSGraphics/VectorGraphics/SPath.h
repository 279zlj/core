#ifndef _SPATH_H
#define _SPATH_H

//-------------------------------------------------------------------------------------------------------------------------------
// SPathPoint
//-------------------------------------------------------------------------------------------------------------------------------

struct SPathPoint 
{
	double dX;
	double dY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// SPath.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

// ���� ��� ������ ����� ������ subpath
#define SPathFirst         0x01

// ���� ��� ��������� ����� ������ subpath
#define SPathLast          0x02

// ���� subpath ���������, ����� ��� ��������� � ������ ����� ������ ���������, �� ���� � ������ ������ ��������� 
#define SPathClosed        0x04

// ����, ����������, ��� ������ ����� �������� ����������� ��� ������ �����
#define SPathCurve         0x08

//-------------------------------------------------------------------------------------------------------------------------------
// SPathHint
//-------------------------------------------------------------------------------------------------------------------------------

struct SPathHint 
{
	int nFirstControl;
	int nSecondControl;
	int nFirstPoint;
	int nLastPoint;
};

//-------------------------------------------------------------------------------------------------------------------------------
// SPath
//-------------------------------------------------------------------------------------------------------------------------------

class SPath 
{
public:

	SPath();

	SPath *�opy() 
	{ 
		return new SPath(this); 
	}

	SPath& operator=(const SPath& oPath);

	~SPath();

	void Append(SPath *pPath);

	int MoveTo(double dX, double dY);

	int LineTo(double dX, double dY);

	int CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	int Close();

	void Offset(double dDx, double dDy);

	int GetCount() 
	{ 
		return m_nPointsCount; 
	}
	void GetPoint(int nIndex, double *pdX, double *pdY, unsigned char *punFlag)
    { 
		*pdX = m_pPoints[nIndex].dX; 
		*pdY = m_pPoints[nIndex].dY; 
		*punFlag = m_pFlags[nIndex]; 
	}

	BOOL GetCurPoint(double *pdX, double *pdY);

	// ��������� ���� StrokeAdjust.
	void AddStrokeAdjustHint(int nFirstControl, int nSecondControl, int nFirstPoint, int nLastPoint);


protected:

	SPath(SPath *pPath);
	void Resize(int nPointsCount);
	BOOL NoCurrentPoint() 
	{ 
		return m_nCurSubpath == m_nPointsCount; 
	}
	BOOL OnePointSubpath() 
	{ 
		return m_nCurSubpath == m_nPointsCount - 1; 
	}
	BOOL OpenSubpath() 
	{ 
		return m_nCurSubpath < m_nPointsCount - 1; 
	}

protected:

	SPathPoint    *m_pPoints;      // ������ �����
	unsigned char *m_pFlags;       // ������ ������, ���������� �������� ����� � SubPath
	int            m_nPointsCount; // ���������� ����� 
	int            m_nSize;        // ���������������� ������ �������
	
	int            m_nCurSubpath;  // ����� ������ ����� ���������� SubPath

	SPathHint     *m_pHints;       //
	int            m_nHintsCount;
	int            m_nHintsSize;

	friend class   SXPath;
	friend class   SImage;
	friend class   CAVSGraphics;
};

#endif /* _SPATH_H */
