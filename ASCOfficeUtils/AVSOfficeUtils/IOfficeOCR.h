#pragma once

//------------------------------------------------------------------------------------------------------
// IOfficeOCR
//------------------------------------------------------------------------------------------------------
// �������� �����������, ������ ����� ( ���� �� ���� ), ������� ��������� �� �����������
//------------------------------------------------------------------------------------------------------

[ object, uuid("804C1FF6-7020-477E-AD9F-980E9201C237"), dual, pointer_default(unique) ]
__interface IOfficeOCR : IDispatch
{
	[id(1)]			HRESULT Recognize([in] IUnknown *Image, [out, retval] BSTR* Text);					// �������� �����������, ���������� �����

	[id(2), propput]HRESULT OutputFormat([in] LONG Type);												// ���������� ������ ��������� ������ ( �� ��������� XML )
	[id(2), propget]HRESULT OutputFormat([ out, retval ] LONG *Type);									// ���������� ������ ��������� ������ ( �� ��������� XML )

	[id(3), propput]HRESULT GrayLevel([in] LONG Type);													// ���������� ������� ������ ( �� ��������� 0 - ������������� )
	[id(3), propget]HRESULT GrayLevel([ out, retval ] LONG *Type);										// ���������� ������� ������ ( �� ��������� 0 - ������������� )

	[id(4), propput]HRESULT DustSize([in] LONG Type);													// ���������� ������ ���� ( �� ��������� -1 - ������������� )
	[id(4), propget]HRESULT DustSize([ out, retval ] LONG *Type);										// ���������� ������ ���� ( �� ��������� -1 - ������������� )

	[id(5), propput]HRESULT SpaceWidthDots([in] LONG Type);												// ���������� ������ ��������/����� ( �� ��������� 0 - ������������� )
	[id(5), propget]HRESULT SpaceWidthDots([ out, retval ] LONG *Type);									// ���������� ������ ��������/����� ( �� ��������� 0 - ������������� )

	[id(6), propput]HRESULT Certainty([in] LONG Type);													// ���������� ������� ������������� ������� ( �� ��������� 95 )
	[id(6), propget]HRESULT Certainty([ out, retval ] LONG *Type);										// ���������� ������� ������������� ������� ( �� ��������� 95 )

	[id(100000)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);			// ��������� �������������� ����������
	[id(100001)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);	// ��������� �������������� ����������
};