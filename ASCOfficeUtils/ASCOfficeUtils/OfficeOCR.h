#pragma once
#include "resource.h"
#include "IOfficeOCR.h"
#include "../../../AVSVideoStudio3/Common/AVSATLError.h"

#define OUTPUT_FORMAT_TYPE_XML	0
#define OUTPUT_FORMAT_TYPE_TEXT	1

[ coclass, uuid("5E4BC6BB-26B4-4237-894E-2C872842A8EE"), threading(apartment), vi_progid("AVSOfficeUtils.OfficeOCR"), progid("AVSOfficeUtils.OfficeOCR.1"), version(1.0), support_error_info(IOfficeOCR) ]
class ATL_NO_VTABLE COfficeOCR : public IOfficeOCR, public CAVSATLError
{
public :

	COfficeOCR();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

public :

	STDMETHOD ( Recognize )( IUnknown *Image, BSTR* Text );					// �������� �����������, ���������� �����

	STDMETHOD ( put_OutputFormat )( LONG Type );							// ���������� ������ ��������� ������ ( �� ��������� XML )
	STDMETHOD ( get_OutputFormat )( LONG *Type );							// ���������� ������ ��������� ������ ( �� ��������� XML )

	STDMETHOD ( put_GrayLevel )( LONG Type );								// ���������� ������� ������ ( �� ��������� 0 - ������������� )
	STDMETHOD ( get_GrayLevel )( LONG *Type );								// ���������� ������� ������ ( �� ��������� 0 - ������������� )

	STDMETHOD ( put_DustSize )( LONG Type );								// ���������� ������ ���� ( �� ��������� -1 - ������������� )
	STDMETHOD ( get_DustSize )( LONG *Type );								// ���������� ������ ���� ( �� ��������� -1 - ������������� )

	STDMETHOD ( put_SpaceWidthDots )( LONG Type );							// ���������� ������ ��������/����� ( �� ��������� 0 - ������������� )
	STDMETHOD ( get_SpaceWidthDots )( LONG *Type );							// ���������� ������ ��������/����� ( �� ��������� 0 - ������������� )

	STDMETHOD ( put_Certainty )( LONG Type );								// ���������� ������� ������������� ������� ( �� ��������� 95 )
	STDMETHOD ( get_Certainty )( LONG *Type );								// ���������� ������� ������������� ������� ( �� ��������� 95 )

	STDMETHOD ( SetAdditionalParam )( BSTR ParamName, VARIANT ParamValue );	// ��������� �������������� ����������
	STDMETHOD ( GetAdditionalParam )( BSTR ParamName, VARIANT *ParamValue );// ��������� �������������� ����������

private :

	LONG m_lOutputFormatType;												// ������ ��������� ������ ( �� ��������� XML )
	LONG m_lGrayLevel;														// ������� ������ ( �� ��������� 0 - ������������� )
	LONG m_lDustSize;														// ������ ���� ( �� ��������� -1 - ������������� )
	LONG m_lSpaceWidthDots;													// ������ ��������/����� ( �� ��������� 0 - ������������� )
	LONG m_lCertainty;														// ������� ������������� ������� ( �� ��������� 95 )
};