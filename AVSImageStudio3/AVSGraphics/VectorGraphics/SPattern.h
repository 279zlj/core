#ifndef _SPATTERN_H
#define _SPATTERN_H

#include "STypes.h"

class SScreen;

//-------------------------------------------------------------------------------------------------------------------------------
// SPattern
//-------------------------------------------------------------------------------------------------------------------------------

class SPattern 
{
public:

	SPattern();

	virtual SPattern *Copy() = 0;

	virtual ~SPattern();

	// ������� ���������� ���� �������.
	virtual void GetColor(int nX, int nY, SColorPointer pColor) = 0;

	// ������� ���������� True, ���� ������ Pattern ������ ��� ������� ������ �����.
	virtual BOOL IsStatic() = 0;

private:
};

//-------------------------------------------------------------------------------------------------------------------------------
// SSolidColor
//-------------------------------------------------------------------------------------------------------------------------------

class SSolidColor: public SPattern 
{
public:

	SSolidColor(SColorPointer pColor);

	virtual SPattern *Copy() 
	{ 
		return new SSolidColor( m_pColor ); 
	}

	virtual ~SSolidColor();

	virtual void GetColor(int nX, int nY, SColorPointer pColor);

	virtual BOOL IsStatic() 
	{ 
		return TRUE; 
	}

private:

	SColor m_pColor;
};

#endif /* _SPATTERN_H */
