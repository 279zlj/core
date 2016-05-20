#ifndef _PATH_H
#define _PATH_H

__interface IAVSGraphicsPath;
class ISimpleGraphicsPath;
//-------------------------------------------------------------------------------------------------------------------------------
// TPathPoint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathPoint 
{
	double dX;
	double dY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CPath.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

// ���� ��� ������ ����� ������ subpath
#define PathFirst         0x01

// ���� ��� ��������� ����� ������ subpath
#define PathLast          0x02

// ���� subpath ���������, ����� ��� ��������� � ������ ����� ������ ���������, �� ���� � ������ ������ ��������� 
#define PathClosed        0x04

// ����, ����������, ��� ������ ����� �������� ����������� ��� ������ �����
#define PathCurve         0x08

//-------------------------------------------------------------------------------------------------------------------------------
// TPathHint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathHint 
{
	int nFirstControl;
	int nSecondControl;
	int nFirstPoint;
	int nLastPoint;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CFontPath
//-------------------------------------------------------------------------------------------------------------------------------

class CFontPath 
{
public:

	CFontPath();

	CFontPath *�opy() 
	{ 
		return new CFontPath(this); 
	}

	~CFontPath();

	void Append(CFontPath *pPath);

	int MoveTo(double dX, double dY);

	int LineTo(double dX, double dY);

	int CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	int Close();

	void Offset(double dDx, double dDy);

	void Reverse();

	void ToMM(double dHorDpi, double dVerDpi);

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

	CString ToXmlString();
	BOOL    ToInterface(IAVSGraphicsPath **ppPath);
	BOOL    ToInterface(ISimpleGraphicsPath *pPath);

private:

	CFontPath(CFontPath *pPath);
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

private:

	TPathPoint    *m_pPoints;      // ������ �����
	unsigned char *m_pFlags;       // ������ ������, ���������� �������� ����� � SubPath
	int            m_nPointsCount; // ���������� ����� 
	int            m_nSize;        // ���������������� ������ �������
	
	int            m_nCurSubpath;  // ����� ������ ����� ���������� SubPath

	TPathHint     *m_pHints;       //
	int            m_nHintsCount;
	int            m_nHintsSize;   
};

#endif /* _PATH_H */
