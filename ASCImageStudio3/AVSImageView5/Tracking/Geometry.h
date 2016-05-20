#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

//const double M_PI = 3.14159265358979323846;

#define sgn(x)		( ((x)>0) ? 1 : 0 )
#define rad(x)		( (x)*(M_PI/180) )
#define grad(x)		( (x)*(180/M_PI) )
#define sqr(x)		( (x)*(x) )

namespace Geometry
{
	void RotatePoint( Structures::POINTD &point, Structures::POINTD center, double dAngle )
	{
		double newX = point.x - center.x;
		double newY = point.y - center.y;
		
		double sinA = sin( dAngle );
		double cosA = cos( dAngle );

		point.x = newX * cosA - newY * sinA + center.x;
		point.y = newX * sinA + newY * cosA + center.y;
	}
	void RotatePoint( double &pointX, double &pointY, double centerX, double centerY, double dAngle )
	{
		double newX = pointX - centerX;
		double newY = pointY - centerY;
		
		double sinA = sin( dAngle );
		double cosA = cos( dAngle );

		pointX = newX * cosA - newY * sinA + centerX;
		pointY = newX * sinA + newY * cosA + centerY;
	}
	BOOL PointOnSegment(int x1, int y1, int x2, int y2, int ptx, int pty, int epsilon)
	{
		//     (x2,y2)
		//       /
		//      /     
		//     /      *P(x,y)
		//    /
		// (x1,y1)


		int scalar1 = (x2-x1)*(ptx-x1) + (y2-y1)*(pty-y1);
		if (scalar1 < 0)
			return FALSE;

		int scalar2 = (x1-x2)*(ptx-x2) + (y1-y2)*(pty-y2);
		if (scalar2 < 0)
			return FALSE;

		// point is inside the bundle - detect distance
		double dA = (y2-y1);
		double dB = (x1-x2);
		double dC = y1*(x2-x1)-x1*(y2-y1);

		//assert ( _hypot(dA, dB) != 0 );

		double dDist = (dA*ptx + dB*pty + dC)/_hypot(dA, dB);

		if (fabs(dDist) < epsilon)
			return TRUE;

		return FALSE;
		
	}
	
	BOOL PointOnSegmentD (double x1, double y1, double x2, double y2, double ptx, double pty, double epsilon)
	{
		//     (x2,y2)
		//       /
		//      /     
		//     /      *P(x,y)
		//    /
		// (x1,y1)


		double scalar1 = (x2-x1)*(ptx-x1) + (y2-y1)*(pty-y1);
		if (scalar1 < 0.0)
			return FALSE;

		double scalar2 = (x1-x2)*(ptx-x2) + (y1-y2)*(pty-y2);
		if (scalar2 < 0.0)
			return FALSE;

		// point is inside the bundle - detect distance
		double dA = (y2-y1);
		double dB = (x1-x2);
		double dC = y1*(x2-x1)-x1*(y2-y1);

		//assert ( _hypot(dA, dB) != 0 );

		double dDist = (dA*ptx + dB*pty + dC)/_hypot(dA, dB);

		if (fabs(dDist) < epsilon)
			return TRUE;

		return FALSE;
		
	}
	double GetLength(double aX, double aY, double bX, double bY)
	{
		return _hypot(bX - aX, bY - aY);
	}

	// ������ ���� ��� ������� a (�� 0 �� 180)
	double GetAngle(double aX, double aY, double bX, double bY, double cX, double cY)
	{
		double a = GetLength(cX, cY, bX, bY);
		double b = GetLength(cX, cY, aX, aY);
		double c = GetLength(aX, aY, bX, bY);

		if (fabs(a) < 0.01 || fabs(b) < 0.01 || fabs(c) < 0.01)
			return 0;

		return acos((b*b + c*c - a*a)/(2*b*c));
	}
	
	// ������ ���� ��� ������� a (����� �������������� ������ � ��������)
	double GetAngle(double aX, double aY, double bX, double bY, double epsilon = 0.001)
	{
		double dx = bX - aX;
		double dy = bY - aY;
		
		double r = _hypot(dx, dy);

		double sdy = (dy >= 0 ? 1.0 : -1.0);

		if (r < epsilon)
			return 0;

		return acos(dx/r)*sdy;
	}
	
	// ������� �������� ������� � ��������� ���������
	double GetEllipseLength(double dEllipseRadiusX, double dEllipseRadiusY)
	{
		// ����� ������������ ������� ��������� �������: pi*(a+b)
		// double dEllipseLength = 3.14159265359*(dLetterRadiusX + dLetterRadiusY);

		// ���� �������� �� ������������ ������, �������� ������ ��� ����� ����������������, 
		// �� �������� ������, ��������, ����� ������� �������� (David Cantrell) 
		// L=4 (a + b) - 2 (4 - pi) a b / [ (a^s)/2 + (b^s)/2 ]^(1/s) 
		// ��� s= 0.825056176207

		double dEllipseS = 0.825056176207;
		double dEllipseLengthA = pow(0.5*pow(dEllipseRadiusX, dEllipseS) + 0.5*pow(dEllipseRadiusY, dEllipseS), 1/dEllipseS);
		double dEllipseLength = 4 * (dEllipseRadiusX + dEllipseRadiusY) - 2 *(4 - 3.14159265359) * dEllipseRadiusX * dEllipseRadiusY / dEllipseLengthA; 

		return dEllipseLength;
	}
	
	void GetEllipsePointCoord(double& x, double& y, double a, double b, double angle, double epsilon = 0.001)
	{
		double dNorm = sqrt(sqr(b) * sqr(cos(angle)) + sqr(a)*sqr(sin(angle)));

		if (dNorm >= epsilon)
		{
			x = (a*b) * cos(angle)/dNorm;
			y = (a*b) * sin(angle)/dNorm;
		}
		else
		{
			x = 0.;
			y = 0.;
		}
	}

	void GetEllipsePointCoord(int& x, int& y, double a, double b, double angle)
	{
		double dX, dY;

		GetEllipsePointCoord(dX, dY, a, b, angle);

		x = (int)dX;
		y = (int)dY;
	}
	
	// �����, ����������� ������ ����������� ���������� ��������� � ���������� �����
	class CRotateManager
	{
	public:
		
		CRotateManager()
		{
			m_pTable = new float[91];

			for (int index = 0; index < 91; ++index)
				m_pTable[index] = (float)sin(index*3.14159265359/180);
		}
		virtual ~CRotateManager()
		{
			delete[] m_pTable;
		}
		
		double GetSin(double dAngle)
		{
			int nAngle = (int)dAngle;

			// TODO: ���� ���� �������� ����������� "�������" ������

			if (nAngle < -360)
			{
				return GetSin(NormalizeAngle(dAngle));
			}
			else if (nAngle < -270)
			{
				return m_pTable[360 + nAngle];
			}
			else if (nAngle < -180)
			{
				return m_pTable[-nAngle - 180];
			}
			else if (nAngle < -90)
			{
				return -m_pTable[180 + nAngle];
			}
			else if (nAngle < 0)
			{
				return -m_pTable[-nAngle];
			}
			else if (nAngle <= 90)
			{
				return m_pTable[nAngle];
			}
			else if (nAngle <= 180)
			{
				return m_pTable[180 - nAngle];
			}
			else if (nAngle < 270)
			{
				return -m_pTable[nAngle - 180];
			}
			else if (nAngle <= 360)
			{
				return -m_pTable[360 - nAngle];
			}

			return GetSin(NormalizeAngle(dAngle));
		}
		double GetCos(double dAngle)
		{
			return GetSin(90 - dAngle);
		}
		
		void RotatePoint(double dX, double dY, double dCenterX, double dCenterY, double dScaleX, double dScaleY, double dAngle, double& dResultX, double& dResultY)
		{
			double dNewX = dX - dCenterX;
			double dNewY = dY - dCenterY;
			
			double dSinA = GetSin(dAngle);
			double dCosA = GetCos(dAngle);

			dResultX = dCenterX + dScaleX*(dNewX * dCosA - dNewY * dSinA);
			dResultY = dCenterY + dScaleY*(dNewX * dSinA + dNewY * dCosA);
		}
		
	protected:
		
		int NormalizeAngle(double dAngle)
		{
			while (dAngle < 0)
				dAngle += 360;
			while (dAngle >= 360)
				dAngle -= 360;

			return (int)dAngle;
		}
		
	protected:

		float* m_pTable;
	};
	
	// �����, ����������� ����������� ��������� ���������� � polyline'��
	class CPolylineManager
	{
		double* m_pPoints; // ���������� ����� (���� double'��)
		double* m_pLengths; // ��������� ����� ���� (pLength[index] - ����� �� ������ polyline, �� ����� � �������� Index)
		
		int m_nPointsCount;
		
	private:

		void Clear()
		{
			if (NULL != m_pPoints)
				delete[] m_pPoints;
			if (NULL != m_pLengths)
				delete[] m_pLengths;

			m_pPoints = NULL;
			m_pLengths = NULL;
			m_nPointsCount = 0;
		}
		
	public:
		
		CPolylineManager()
		{
			m_pPoints = NULL;
			m_pLengths = NULL;
			m_nPointsCount = 0;
		}
		virtual ~CPolylineManager()
		{
			Clear();
		}
		
		BOOL Create(CSimpleArray<double>& arrPoints)
		{
			Clear();

			if ((arrPoints.GetSize() % 2) != 0)
				return FALSE;

			m_nPointsCount = arrPoints.GetSize()/2;
			if (m_nPointsCount < 1)
				return FALSE;

			m_pPoints = new double[2*m_nPointsCount];
			m_pLengths = new double[m_nPointsCount];

			if (NULL == m_pPoints || NULL == m_pLengths)
			{
				Clear();
				return FALSE;
			}

			memcpy(m_pPoints, arrPoints.GetData(), arrPoints.GetSize()*sizeof(double));

			int nPointIndex = 0;

			m_pLengths[0] = 0;
			for (int index = 1; index < m_nPointsCount; ++index, nPointIndex += 2)
			{
				double dLocalLength = _hypot(m_pPoints[nPointIndex + 2] - m_pPoints[nPointIndex + 0], m_pPoints[nPointIndex + 3] - m_pPoints[nPointIndex + 1]);

				m_pLengths[index] = m_pLengths[index - 1] + dLocalLength;
			}

			return TRUE;
		}
		
		int GetPointsCount()
		{
			return m_nPointsCount;
		}
		double GetTotalLength()
		{
			if (m_nPointsCount < 2)
				return 0;

			return m_pLengths[m_nPointsCount - 1];
		}
		double* GetPointsData()
		{
			return m_pPoints;
		}
		double* GetPointsLengths()
		{
			return m_pLengths;
		}
		
		BOOL GetPointByLength(double dLength, double& dX, double& dY, double dEpsilon = 0.001)
		{
			if (dLength < 0 || dLength > GetTotalLength())
				return FALSE;

			for (int index = 1; index < m_nPointsCount; ++index)
			{
				if (dLength > m_pLengths[index] + dEpsilon)
					continue;

				double dKoef1 = (dLength - m_pLengths[index - 1])/(m_pLengths[index] - m_pLengths[index - 1]);
				double dKoef2 = 1.0 - dKoef1;

				int nPointIndex = 2*(index - 1);

				dX = m_pPoints[nPointIndex + 0]*dKoef2 + m_pPoints[nPointIndex + 2]*dKoef1;
				dY = m_pPoints[nPointIndex + 1]*dKoef2 + m_pPoints[nPointIndex + 3]*dKoef1;

				return TRUE;
			}

			return FALSE;
		}
		BOOL GetPointByKoef(double dLengthKoef, double& dX, double& dY, double dEpsilon = 0.001)
		{
			return GetPointByLength(dLengthKoef*GetTotalLength(), dX, dY, dEpsilon);
		}
	};
}