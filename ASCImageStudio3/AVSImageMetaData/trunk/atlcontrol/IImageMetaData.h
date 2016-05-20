/*!	\file IImageMetaData.h
*
*	�������� ����������� ��� ������ � �����������
*
*	��������� IImageMetaDataParser ������������ ��� ����������� ����������������
*	��������� ���������� ������ �����, ����� ���
*	Exif (http://en.wikipedia.org/wiki/EXIF),
*	IPTC (http://en.wikipedia.org/wiki/IPTC_Information_Interchange_Model),
*	XMP (http://en.wikipedia.org/wiki/Extensible_Metadata_Platform) 
*	� ��.
*
*	��� ������ ������ IImageMetaDataParser::ParseFile ��� IImageMetaDataParser::ParseStream ������������
*	������ ������ � ����� ����������.
*	����� ��������� ����� ���������� ��� ������������ ����� ������������ ��������� IImageMetaDataParser::AvailDataTypeCount.
*	����� ����� ����� �������� ���� ��������� ���������� � ���������� ��� ������ � ���� ��� ������ �������
*	IImageMetaDataParser::MetaDataType � IImageMetaDataParser::MetaData ��������������.
*
*	��� ���������� ������������ ����� ������� ����������������� ��������, �������� �� ���� (�������� 1 - ��� Exif, 2 - ��� IPTC � �.�.).
*	
*	������� ��������� ����� ����� IImageMetaDataParser::MetaData, �� ����� ��������� ����� ��� ���� ����������
*	��������� IImageMetaDataCommon. ����� ���� �� ����� �������� (����� IImageMetaDataCommon::GetCommonXML) ������� XML ������ ����,
*	����������� ��� ���� ����� ����������, ��������������� � �������� ��� ����������� ������
*	������������ � ���� "��� ���������" - "��������".
*
*	��� ��������� ������������ �����, ������������ � XML, �������� �� �������� �� ���������
*	(��������, ��� ������������� �������� �� ������ �����), ������ ����� IImageMetaDataCommon::SetTranslateXML
*	
*	����� IImageMetaDataCommon::GetXML ���������� XML, ����������� ��� ����������� ���� ����������. ��������, ��� Exif
*	����� ����������� ������ �� ��������������� �����, �� �������������, �����, ����� � ���������.
*
*	��� ������� ����������� ���� ����������, �� ����� ��������� ���������, ������������� �� IImageMetaDataCommon
*	� ����������� ��� ������� ����.
*/

#ifndef _AVS_IIMAGEMETADATA_INCLUDE_H_
#define _AVS_IIMAGEMETADATA_INCLUDE_H_
#pragma once

/*!	\interface	IImageMetaDataParser
*	\breif	��������� ������������� ���������������� ��������� ���������� ������� ����
*/
[
	object,
	uuid("9C564E31-0ED0-45e3-B447-71E308724130"),
	dual,	helpstring("IImageMetaDataParser Interface"),
	pointer_default(unique)
]
__interface IImageMetaDataParser: IDispatch
{
	/*!	\brief	���������� ���������� �� �����
	*	
	*	\param[in]	bstrFileName	��� �����, ������ BSTR
	*	\param[in]	dwMode			����� ��������� (���������������)
	*/
	[id(10000 + 1)] HRESULT	ParseFile([in] BSTR bstrFileName, [in] DWORD dwMode);
	
	/*!	\breif	���������� ���������� �� ������ (���������� ������ ParseFile, �� ������ �������������� �� ������)
	*
	*	\param[in]	pStream		����� IStream �� �������� ������ ������
	*	\param[in]	dwMode		����� ��������� (���������������)
	*	
	*	\remarks	����� ����� ���� ������� � ������ ����� �� ����� ��������� ���������� �� �����, �������
	*				�� ���������� ��������������� �� ����� (�������� ����������� � ������ thumbnail �� raw-�����)
	*/
	[id(10000 + 2)] HRESULT ParseStream([in] IStream * pStream, [in] BSTR bstrFileName);

	/*!	\breif	�������� ����� ��������� ����� ����������
	*
	*	\param	dwCount[out]	����� ��������� ����� ����������. � ������ ���� �� ������� ������� ������� ����������,
	*							���������� �������� 0
	*/
	[id(10000 + 3), propget] HRESULT AvailDataTypeCount([out, retval] DWORD * dwCount);
	
	/*!	\brief	�������� ��������� ���������� �� ����������� ������
	*
	*	\param[in]	dwNum	���������� ����� ������������� ���������� (�� 0 �� \a AvailDataTypeCount)
	*	\param[out]	pMetaData	��������� �� ���������, ����� ������� �������� ���������� ��� NULL � ������ ������.
	*							� ������ ��������� ����������, �������������, ��� �� ����� ��������� ��������� \a IImageMetaDataCommon - ����� ��� ���� ����������
	*	
	*	\returns	E_INVALIDARG, � ������ ���� ������� �������������� ���������� ���������� ����� ����������;
	*				SUCCEEDED � ������ �������� ����������, FAILED - � ��������� ������
	*/
	[id(10000 + 4)] HRESULT MetaData([in] DWORD dwNum, [out, retval] IUnknown ** pMetaData);

	/*!	\brief	�������� ��� ���������� �� ����������� ������
	*
	*	\param[in]	dwNum	���������� ����� ������������� ���������� (�� 0 �� \a AvailDataTypeCount)
	*	\param[out]	dwDataType	������������� ���� ���������� ��� ���� � ������ ������
	*	
	*	\returns	E_INVALIDARG, � ������ ���� ������� �������������� ���������� ����� ����������;
	*				SUCCEEDED � ������ �������� ����������, FAILED - � ��������� ������
	*/
	[id(10000 + 5)] HRESULT MetaDataType([in] DWORD dwNum, [out, retval] DWORD * dwDataType);
	
	/*!	\brief		���������� �������������� ��������, �������� �� �����
	*/
	[id(10000 + 100)] HRESULT	SetAdditionalParam([in]BSTR ParamName, [in] VARIANT ParamValue);

	/*!	\brief		�������� �������������� ��������, �������� �� �����
	*/
	[id(10000 + 101)] HRESULT	GetAdditionalParam([in]BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

/*!	\interface IImageMetaDataCommon
*	\brief	����� ��� ���� ����� ���������� ���������.
*			������������ ��� ��������� ���� ���������� � XML ������ ���� ��� ���� ����� ����������
*/
[
	object,
	uuid("0EA09BA9-CBBE-460a-8DB0-EE4E34441679"),
	dual,	helpstring("IImageMetaDataCommon Interface"),
	pointer_default(unique)
]
__interface IImageMetaDataCommon: IDispatch
{
	/*!	\breif	�������� ��� ����������
	*/
	[id(10000 + 1), propget] HRESULT	MetaDataType([out, retval] DWORD * dwMode);

	/*!	\breif	�������� XML � �����������
	*	
	*	\remarks	������ XML ����� ��� ���� ����� ���������� (���� '��� ���������' - '��������')
	*/
	[id(10000 + 2)] HRESULT	GetCommonXML([in] DWORD dwMode, [out, retval] BSTR * bstrMetaDataXML);

	/*!	\breif	�������� XML � �����������
	*	
	*	\remarks	������ XML ����������� ��� ������� ���� ����������
	*/
	[id(10000 + 3)] HRESULT GetXML([in] DWORD dwMode, [out, retval] BSTR * bstrMetaDataXML);

	/*!	\breif	���������� XML � ����������� ��� ����������� �������� �����
	*	
	*	\param[in]	bstrTranslateXML XML � ����������� �����
	*
	*	\remarks	����� ��������� ��� ���� ����� ������ �� �������� �������� ��������� ����� � ���
	*				������������� ��������. �������, �������� ���� 'ExposureTime' (Exif Tag ID 33434)
	*				� ���������� �������� ����� ��������������� ����� ��������� 'Exposure time', � ������� - '����� ��������'
	*/
	[id(10000 + 4)] HRESULT	SetTranslateXML([in] BSTR bstrTranslateXML);

	/*!	\brief		���������� �������������� ��������, �������� �� �����
	*/
	[id(10000 + 100)] HRESULT	SetAdditionalParam([in]BSTR ParamName, [in] VARIANT ParamValue);

	/*!	\brief		�������� �������������� ��������, �������� �� �����
	*/
	[id(10000 + 101)] HRESULT	GetAdditionalParam([in]BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

/*!	\interface IImageMetaDataExif
*	\brief	���������� ���� Exif
*	
*	\remarks	���������������� ���������� ����� ����������� (������ ��� ����������)
*/
[
	object,
	uuid("D1680997-BC53-4124-8F9A-CCEE05A6D3E7"),
	dual,
	pointer_default(unique)
]
__interface IImageMetaDataExif: IImageMetaDataCommon
{
	[id(1)] HRESULT GetValue([in]USHORT TagId, [out, retval] BSTR * bstrValue);
	[id(2)] HRESULT GetType([in]USHORT TagId, [out, retval] USHORT * nType);
	[id(3)] HRESULT GetCount([in]USHORT TagId, [out, retval] UINT * nCount);
};

/*!	\interface IImageMetaDataIPTC
*	\brief	���������� ���� IPTC
*	
*	\remarks	���������������� ���������� ����� ����������� (������ ��� ����������)
*/
[
	object,
	uuid("6BE1ECC2-B1AE-4e45-A46F-40230909AFC2"),
	dual,
	pointer_default(unique)
]
__interface IImageMetaDataIPTC: IImageMetaDataCommon
{
};


#endif

