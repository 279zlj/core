/*!
 *	\file	ImageRaw3.h
 *	\brief	Declaration of the CImageRaw3
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */

#pragma once
#include "resource.h"       
#include "ImageRawATLError.h"
#include "ImageRawFileDesc.h"
#include "ImageRawImports.h"
#include "ImageRawOptions.h"

#include "dcrawwrapper.h"


//#include ".\dcraw\imagerawcore.h"

//#include "dcrawdata_core.h"
//#include "ufrawdata_core.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


/*!
 *	\interface	IImageRaw3	ImageRaw3.h	"ImageRaw3.h"
 *	\brief		���������������� ��� ��������� raw-���������, ��. �������� ������ CImageRaw3, ��������������� ������ ���������
 */
[
	object,
	uuid("3B76AE12-C20A-47B6-A733-E7B53C4C7E0C"),
	dual,	helpstring("IImageRaw3 Interface"),
	pointer_default(unique)
]
__interface IImageRaw3 : IDispatch
{
	[id(1), helpstring("method OpenFile")]				HRESULT OpenFile([in] BSTR fileName);
	[id(3), helpstring("method CloseFile")]				HRESULT CloseFile(void);
	[id(4), helpstring("method ApplyXML")]				HRESULT ApplyXML([in] BSTR bsSettingXML, [in] LONG lMode);
	[id(5), helpstring("method GetXML")]				HRESULT GetXML([out] BSTR * bsSettingXML, [in] LONG lMode);
	[id(6), helpstring("method GetMetaData")]			HRESULT GetMetaData([out] BSTR * bsMetaDataXML);
	[id(7), helpstring("method GetPreviewImage")]		HRESULT GetPreviewImage([out] IUnknown** piUncFrame);
	[id(8), helpstring("method GetThumbnailImage")]		HRESULT GetThumbnailImage([out] IUnknown** piUncFrame);
	[id(9), helpstring("method RenderImage")]			HRESULT RenderImage([out] IUnknown** piUncFrame);

	[id(100), helpstring("method SetAdditionalParam")]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(101), helpstring("method GetAdditionalParam")]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);

};

[
	object,
	uuid("30E61409-7C4F-49f4-A6AA-603B22AFD18D"),
	dual,
	pointer_default(unique)
]
__interface IImageRaw3Metadata : IDispatch
{
	[id(1)]	HRESULT GetMetadata([in] BSTR fileName);
	[id(2)]	HRESULT IFDCount([in] DWORD dwReserved, [out, retval] DWORD * dwCount);
	[id(3)]	HRESULT IFDTag([in] DWORD dwReserved, [out, retval] USHORT * dwTag);
	[id(4)]	HRESULT IFDOffset([in] DWORD dwIndex, [out, retval] DWORD * dwOffset);
	[id(5)]	HRESULT BaseOffset([in] DWORD dwIndex, [out, retval] DWORD * dwOffset);
	[id(6)]	HRESULT GetFlags([in] DWORD dwReserved, [out, retval] DWORD * dwFlags);
};

/*!
 *	\interface _IImageRaw3Events ImageRaw3.h "ImageRaw3.h"
 */
[
	dispinterface,
	uuid("9E4A9B25-3630-4B34-A15C-05A28A2D21AF"),
	helpstring("_IImageRaw3Events Interface")
]
__interface _IImageRaw3Events
{
};


/*!
 *	\class		CImageRaw3 ImageRaw3.h "ImageRaw3.h"
 *
 *	\brief		coclass, �������������� ��������� IImageRaw3, �����������
 *				���������������� ��� ������ � raw-�������������
 *
 *	\details	com-����� � ���������� ���������� IImageRaw3
 *				��� threading-������: apartment
 *				��������� ������� support_error_info ��� ��������� ���������� ISupportErrorInfoImpl
 */
[
	coclass,
	default(IImageRaw3, _IImageRaw3Events),
	threading(apartment),
	support_error_info("IImageRaw3"),
	event_source(com),
	vi_progid("AVSImageRaw3.ImageRaw3"),
	progid("AVSImageRaw3.ImageRaw3.1"),
	version(1.0),
	uuid("858B32D9-7B90-45A6-B461-2FF4949FC1FA"),
	helpstring("ImageRaw3 Class")
]
class ATL_NO_VTABLE CImageRaw3 :
	public IImageRaw3,
	public CImageRawATLError<CImageRaw3>,
	public IImageRaw3Metadata
{
public:

	/*!
	 *	\brief	Class constructor
	 */
	CImageRaw3()
	{
	}

	__event __interface _IImageRaw3Events;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	/*!
	 *	\brief	Final construct
	 */
	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	/*!
	 *	\brief	Final release
	 */
	void FinalRelease()
	{
		m_imageraw_core._close();
	}

public:
	
	/*!
	 *	\brief		Implementation of IImageRaw3::OpenFile(BSTR fileName): open raw image
	 *	\param[in]	fileName Input file name
	 *	\return		hresult value
	 */
	STDMETHOD(OpenFile)(BSTR fileName);

	/*!
	 *	\brief		Implementation of IImageRaw3::ApplyXML(BSTR bsSettingXML, LONG lMode): apply settings XML
	 *	\details	������ RAW-��������� � ������������ � ������� XML, �������� ���������� \a bsSettingXML
	 *				��������� ����� ������ ������������ ����� � ������ ����������� ��� ��������� preview
	 *				� ��������� �����������. ��� ����� ������� ������ ������ � �������� \a lMode.
	 *				�������� ������ ����� ��������� �� ����� ��������� � ���������� ��� ����������� ����� ���������
	 *				����������� ����� ����� ������������� � ������ ���� ������ ��������� ������ �������� �� �
	 *				����������, ��������, ����� ������������ ������� ���������
	 *
	 *	\param[in]	bsSettingXML	XML c ����������� raw-���������
	 *	\param[in]	lMode			����� ��������� (����������/�����������, �������� (?))
	 *	\return		hresult value
	 *
	 *	\todo		��������� ������ ������, ���������� �������������, ���� ��� ����������
	 *	\todo		��������� ����� ������� �������� ��������� ��������� preview,
	 *				����� ��� ������ ����������� ��� ��������� ���������� ������� �����������
	 */
	STDMETHOD(ApplyXML)(BSTR bsSettingXML, LONG lMode);

	/*!
	 *	\brief		Implementation of IImageRaw3::GetXML(BSTR * bsSettingXML): get settings XML
	 *	\details	�������� XML c �������� ����������� raw-���������
	 *				��� �������� ������ �����, ������ ������������ ��������� ��������� �� ���������,
	 *				��������, ����������� ��� ������� �����.
	 *
	 *	\param[out]	bsSettingXML XML c ����������� raw-���������
	 *	\return		hresult value
	 *	\todo		�������� ��������� ��� ������� ������
	 */
	STDMETHOD(GetXML)(BSTR * bsSettingXML, LONG lMode);

	/*!
	 *	\brief		Implementation of IImageRaw3::GetMetaData(BSTR * bsMetaDataXML): get image meta data
	 *	\details	��������� ����-������ �����������, �������� ���������� EXIF
	 *
	 *	\param[out]	bsMetaDataXML XML c ��������� ����-������
	 *	\return		hresult value
	 */
	STDMETHOD(GetMetaData)(BSTR * bsMetaDataXML);

	
	/*!
	 *	\brief		Implementation of IImageRaw3::GetPreviewImage(IUnknown** piUncFrame): get preview
	 *	\details	��������� preview-����������� � ���������� ����������� (���� ��������) 
	 *	\param[out]	piUncFrame ��������� �� ������, �������������� ��������� IAVSUncompressedVideoFrame
	 *	\return		hresult value
	 */
	STDMETHOD(GetPreviewImage)(IUnknown** piUncFrame);

	/*!
	 *	\brief		Implementation of IImageRaw3::GetThumbnailImage(IUnknown** piUncFrame): get thumbnail image
	 *	\details	��������� ��������� �����������, ������� ����� �������������� � RAW-����� ������ � ��������� �������
	 *				������ ��������� ��������� � JPEG-��������
	 *
	 *	\param[out]	piUncFrame ��������� �� ������, �������������� ��������� \a IAVSUncompressedVideoFrame
	 *	\return		hresult value
	 *
	 *	\todo		��������� ��� �������� �� ������: ���� uncompressed-��������, ���� ������������ JPEG
	 *				(��� ������ ������), ������� ����������� � RAW-�����
	 */
	STDMETHOD(GetThumbnailImage)(IUnknown** piUncFrame);

	/*!
	 *	\brief		Implementation of IImageRaw3::RenderImage(IUnknown** piUncFrame): render image		
	 *	\details	��������� ��������� ��������������� �����������, ����� ������ �����������
	 *				��� �������� ����� �������, � �������� ���������� ���� raw-���������
	 *
	 *	\param[out]	piUncFrame ��������� �� ������, �������������� ��������� IAVSUncompressedVideoFrame
	 *	\return		hresult value
	 */
	STDMETHOD(RenderImage)(IUnknown** piUncFrame);

	/*!
	 *	\brief		Implementation of IImageRaw3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
	 *	\details	
	 */
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);

	/*!
	 *	\brief		Implementation of IImageRaw3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
	 *	\details
	 *
	 *	\return		hresult value
	 */
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);

	/*!
	 *	\brief		Implementation of IImageRaw3::CloseFile(): close file
	 *	\details	�������� �����: ����� ���� ����������, ������������ ������ � �.�.
	 *
	 *	\return		hresult value
	 */
	STDMETHOD(CloseFile)(void);


	// IImageRaw3Metadata

	STDMETHOD(GetMetadata)(BSTR fileName);
	STDMETHOD(IFDCount)(DWORD dwReserved, DWORD * dwCount);
	STDMETHOD(IFDTag)(DWORD dwIndex, USHORT * nTag);
	STDMETHOD(IFDOffset)(DWORD dwIndex, DWORD * dwOffset);
	STDMETHOD(BaseOffset)(DWORD dwIndex, DWORD * dwOffset);
	STDMETHOD(GetFlags)(DWORD dwReserved, DWORD * dwFlags);

protected:

	bool m_SimpleMode; //!< ����������� ����� ���������
	ImageRaw3SimpleModeOptions	m_OptionsSimple; //!< ��������� ������������ ������ ���������


	void _ExtractImage();

	//c_ufraw_data ufraw_data;

	c_imageraw_core m_imageraw_core;

	MediaCore::IAVSUncompressedVideoFrame * piVideoFrame;

};