#ifndef _SSCREEN_H
#define _SSCREEN_H

#include "STypes.h"

struct SScreenParams;

//-------------------------------------------------------------------------------------------------------------------------------
// SScreen
//-------------------------------------------------------------------------------------------------------------------------------

class SScreen 
{
public:

	SScreen(SScreenParams *pParams = NULL);
	SScreen(SScreen *pScreen);
	~SScreen();

	SScreen *Copy() 
	{ 
		return new SScreen(this); 
	}

	// ������� �������� ������� � ����� (nX, nY) (� ����� �����).
	int GetGrayPixel(int nX, int nY, unsigned char unValue);

	// ������� ���������� TRUE, ���� �������� unValue ���� ������ m_unMaxValue, ���� ������ m_unMinValue.
	BOOL IsStatic(unsigned char unValue);

private:

	void BuildDispersedMatrix(int nX, int nY, int nValue, int nDelta, int nOffset);
	void BuildClusteredMatrix();
	void BuildSCDMatrix(int nRadius);
	int  Distance(int nX0, int nY0, int nX1, int nY1);

private:

	unsigned char *m_pMatrix;    // Threshold matrix
	int            m_nSize;      // ������ Threshold matrix
	unsigned char  m_unMinValue; // ����� ������� �� ��������� ������, ��� m_unMinValue ��������� ������
	unsigned char  m_unMaxValue; // ����� ������� �� ��������� ������, ��� m_unMaxValue ��������� �����
};

#endif /* _SSCREEN_H */
