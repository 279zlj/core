/*! \file ImageRawErrorDescription.h
 *	\brief ���� ������ ���������� AVSImageRaw, ������� ��������� ���������� �������� ������ �� ����
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */

#pragma once
#include <windows.h>

#define ERRORDESC_STR LPCOLESTR
#define ERRORDESC_STR_ ERRORDESC_STR

#define E_IMAGERAW_BASE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0300)
#define E_IMAGERAW(ErrorCode) (E_IMAGERAW_BASE + ErrorCode)

#define E_IMAGERAW_DESC(ErrorCode)		c_str_##ErrorCode

#define IMAGERAW_UNEXPECTED_ERROR					1	//!< ����������� ������
#define IMAGERAW_OPENFILE_ERROR						2	//!< ������ �������� �����
#define IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR		3	//!< ���������� ���������� ������ DCRaw
#define IMAGERAW_UNSUPPORTERDFORMAT_ERROR			4	//!< ���������������� ������ �����
#define IMAGERAW_DECODE_ERROR						5	//!< ������ ������������� �����
#define IMAGERAW_NOTHUMBNAIL_ERROR					6	//!< ����������� �� �������� �������� ��� �������������
#define IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR		7	//!< �������� ��� ������������� ����������������� �������
#define IMAGERAW_READING_NEFCURVE_ERROR				8	//!< Error reading NEF curve
#define IMAGERAW_NOCAMWB_ERROR						9	//!< Cannot use camera white balance
#define IMAGERAW_DCRAW_OUTOFMEMORY_ERROR			10	//!< Out of memory
#define IMAGERAW_FILE_NOT_OPENED_ERROR				11	//!< File not opened 
#define IMAGERAW_LOAD_THUMBNAIL_ERROR				12	//!< Cannot load thumbnail
#define IMAGERAW_DECODE_THUMBNAIL_ERROR				13	//!< Cannot decode thumbnail image
#define	IMAGERAW_MEMORY_ERROR						14  //!< Memory manipulation error 


/*!
 *	\brief	�������� ��������� �������� ������
 *
 *	\param[in]	lErrorCode	��� ������
 *	\param[out]	bsErrorDesc	������ BSTR �� ��������� ��������� ������,
 *							������ ����� ������������� ������ ���������� ������
 *							������� \a SysFreeString()
 *
 *	\return				HRESULT, ����� ���������� S_OK � ������ ������; E_POINTER, ����� \a bsErrorDesc == NULL ��� E_INVALIDARG,
 *						� ������ ������������ ���� ������
 */
HRESULT GetStringErrorDesc(LONG lErrorCode,BSTR * bsErrorDesc);



