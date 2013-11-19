#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "Object.h"

class Dict;
class Stream;
struct PSObject;
class PSStack;

//------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------

#define funcMaxInputs        32
#define funcMaxOutputs       32
#define sampledFuncMaxInputs 16

class Function 
{
public:

	Function();

	virtual ~Function();

	static Function *Parse(Object *pFuncObject);

	// ��������� Domain � Range
	BOOL Initialize(Dict *pDict);

	virtual Function *Copy() = 0;

	// ���� �������
	//   -1 : identity
	//    0 : sampled
	//    2 : exponential
	//    3 : stitching
	//    4 : PostScript
	virtual int GetType() = 0;

	int GetInputSize() 
	{ 
		return m_nInputDim; 
	}
	int GetOutputSize() 
	{ 
		return m_nOutputDim; 
	}

	double GetDomainMin(int nIndex) 
	{ 
		return m_arrDomain[nIndex][0]; 
	}
	double GetDomainMax(int nIndex) 
	{ 
		return m_arrDomain[nIndex][1]; 
	}
	double GetRangeMin(int nIndex) 
	{ 
		return m_arrRange[nIndex][0]; 
	}
	double GetRangeMax(int nIndex) 
	{ 
		return m_arrRange[nIndex][1]; 
	}
	BOOL   HasRange() 
	{ 
		return m_bHasRange; 
	}

	// ����������, ������ ����� �������. pInput - ����� �������� ����������, pOutput - ��������� ��������.
	virtual void Transform(double *pInput, double *pOutput) = 0;

	virtual BOOL IsValid() = 0;

protected:


	int    m_nInputDim;                   // Function: R^(InputDim) -> R^(OutputDim)
	int    m_nOutputDim;                  // 
	double m_arrDomain[funcMaxInputs][2]; // ������� � �������� ��� ������ �������� ����������
	double m_arrRange[funcMaxOutputs][2]; // ������� � �������� ��� ������� ���������� ���������
	BOOL   m_bHasRange;                   // Domain ������ ���� ����� ������, � Range ����� �������������
};

//------------------------------------------------------------------------
// IdentityFunction
//------------------------------------------------------------------------

class IdentityFunction: public Function
{
public:

	IdentityFunction();
	virtual ~IdentityFunction();
	virtual Function *Copy() 
	{ 
		return new IdentityFunction(); 
	}
	virtual int GetType() 
	{ 
		return -1; 
	}
	virtual void Transform(double *pInput, double *pOutput);
	virtual BOOL IsValid() 
	{
		return TRUE; 
	}

private:
};

//------------------------------------------------------------------------
// SampledFunction (Type0 Function)
//------------------------------------------------------------------------

// �� ����� ���� ��� ����������������� �������. ����� ������ ���� ������ 
// ����� � �������� ������� � ������ ������, � ����� ������ ������� 
// �������������� ��� ��������������� ������� �� �������� ���������, ������
// ������������ ������� �� ���� Order, 1 - �������� ������������, 
// 3 - ������������.

// ���� ����������� ������ �������� ������������.
class SampledFunction: public Function 
{
public:

	SampledFunction(Object *pFuncObject, Dict *pDict);
	virtual ~SampledFunction();
	virtual Function *Copy() 
	{ 
		return new SampledFunction(this); 
	}
	virtual int GetType() 
	{ 
		return 0; 
	}
	virtual void Transform(double *pInput, double *pOutput);
	virtual BOOL IsValid() 
	{
		return m_bValid; 
	}


	int     GetSampleSize(int nIndex) 
	{ 
		return m_arrSize[nIndex]; 
	}
	double  GetEncodeMin(int nIndex) 
	{ 
		return m_arrEncoder[nIndex][0]; 
	}
	double  GetEncodeMax(int nIndex) 
	{ 
		return m_arrEncoder[nIndex][1]; 
	}
	double  GetDecodeMin(int nIndex) 
	{ 
		return m_arrDecoder[nIndex][0]; 
	}
	double  GetDecodeMax(int nIndex) 
	{ 
		return m_arrDecoder[nIndex][1]; 
	}
	double *GetSamples() 
	{ 
		return m_pSamples; 
	}

private:

	SampledFunction(SampledFunction *pFunc);

private:

	int     m_arrSize[funcMaxInputs];        // ���������� ��������� ��������, ������� ����� ��������� ������ ��������
	double  m_arrEncoder[funcMaxInputs][2];  // ������� � �������� ��� Domain encoder
	double  m_arrDecoder[funcMaxOutputs][2]; // ������� � �������� ��� Range decoder
	double  m_arrInputMult[funcMaxInputs];   // 
	int     m_arrIndexMult[funcMaxInputs];	 // 
	double *m_pSamples;                      // ���������� ���� ��������
	int     m_nSamplesCount;                 // ���������� �������� ��������
	double *m_pBuffer;                       // ����� ��� ������� ��������������

	BOOL    m_bValid;
};

//------------------------------------------------------------------------
// ExponentialFunction
//------------------------------------------------------------------------

class ExponentialFunction: public Function 
{
public:

	ExponentialFunction(Object *pFuncObject, Dict *pDict);
	virtual ~ExponentialFunction();
	virtual Function *Copy() 
	{ 
		return new ExponentialFunction(this); 
	}
	virtual int GetType() 
	{ 
		return 2; 
	}
	virtual void Transform(double *pInput, double *pOutput);
	virtual BOOL IsValid() 
	{ 
		return m_bValid; 
	}

	double *GetC0() 
	{ 
		return m_arrC0; 
	}
	double *GetC1() 
	{ 
		return m_arrC1; 
	}
	double  GetN() 
	{ 
		return m_dN; 
	}

private:

	ExponentialFunction(ExponentialFunction *pFunc);

private:

	double m_arrC0[funcMaxOutputs];
	double m_arrC1[funcMaxOutputs];
	double m_dN;

	BOOL   m_bValid;
};

//------------------------------------------------------------------------
// StitchingFunction
//------------------------------------------------------------------------

// �������� �������. � ������� m_arrBounds �������� ����� ������� ���������
// �������. Domain0 < Bounds0 < Bounds1 < ... < Bounds(K - 2) < Domain1.
// i-�� ������� ������ �� ������������� [ Bounds(i - 1), Bounds(i) ),
// ��� Bounds(-1) = Domain0, Bound(k-1) = Domain1 (��������� ������, � �������
// m_arrBounds ��� �������� �� ������).
class StitchingFunction: public Function 
{
public:

	StitchingFunction(Object *pFuncObject, Dict *pDict);
	virtual ~StitchingFunction();
	virtual Function *Copy() 
	{ 
		return new StitchingFunction(this); 
	}
	virtual int GetType() 
	{ 
		return 3; 
	}
	virtual void Transform(double *pInput, double *pOutput);
	virtual BOOL IsValid() 
	{ 
		return m_bValid; 
	}

	int GetFunctionsCount() 
	{
		return m_nCount; 
	}
	Function *GetFunction(int nIndex) 
	{ 
		return m_ppFuncs[nIndex]; 
	}
	double *GetBounds() 
	{ 
		return m_arrBounds; 
	}
	double *GetEncode() 
	{ 
		return m_arrEncode; 
	}
	double *GetScale() 
	{ 
		return m_arrScale; 
	}

private:

	StitchingFunction(StitchingFunction *pFunc);

private:

	int        m_nCount;    // ���������� �������
	Function **m_ppFuncs;   // ���� �������
	double    *m_arrBounds; // ������� �������� ����������� �������
	double    *m_arrEncode; // ������� ������� �������� �������
	double    *m_arrScale;

	BOOL       m_bValid;
};

//------------------------------------------------------------------------
// PostScriptFunction
//------------------------------------------------------------------------

class PostScriptFunction: public Function 
{
public:

	PostScriptFunction(Object *pFuncObject, Dict *pDict);
	virtual ~PostScriptFunction();
	virtual Function *Copy() 
	{ 
		return new PostScriptFunction(this); 
	}
	virtual int GetType() 
	{ 
		return 4; 
	}
	virtual void Transform(double *pInput, double *pOutput);
	virtual BOOL IsValid() 
	{ 
		return m_bValid; 
	}
	StringExt *GetCodeString() 
	{ 
		return m_seCodeString; 
	}

private:

	PostScriptFunction(PostScriptFunction *pFunc);
	BOOL ParseCode(Stream *pStream, int *pnCodePos);
	StringExt *GetToken(Stream *pStream);
	void ResizeCode(int nNewSize);
	void Exec(PSStack *pStack, int nCodePos);

private:

	StringExt *m_seCodeString;
	PSObject  *m_pCode;
	int        m_nCodeSize;

	BOOL       m_bValid;
};

#endif /* _FUNCTION_H */
