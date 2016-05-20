// ASCDocxRenderer.h : Declaration of the CAVSDocxRenderer

#pragma once
#include "resource.h"       // main symbols

#include "..\..\..\ASCImageStudio3\ASCGraphics\Interfaces\ASCRenderer.h"
#include "..\..\..\Common\ASCUtils.h"
#include "..\..\..\Common\xmlutils.h"

#import ".\..\..\..\Redist\ASCOfficeUtils.dll" raw_interfaces_only
#import ".\..\..\..\Redist\ASCImageStudio3.dll" raw_interfaces_only

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Docx.h"

#pragma comment(lib, "Utility.lib")
#pragma comment(lib, "Common.lib")
#pragma comment(lib, "DocxFormat.lib")
#pragma comment(lib, "XML.lib")
#pragma comment(lib, "OfficeSvmFile.lib")

#include "Document.h"
#include "ASCOfficeCriticalSection.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IAVSDocxRenderer
[
	object,
	uuid("5BEC40F1-CA4F-4275-BCC8-D4ABF47D896D"),
	dual,	helpstring("IAVSDocxRenderer Interface"),
	pointer_default(unique)
]
__interface IAVSDocxRenderer : IAVSRenderer
{
  [id(11001), helpstring("method Initialize")]  HRESULT Initialize([in]BSTR bsXMLOptions);
  [id(11002), helpstring("method Save")]  HRESULT Save();
};

AVSOfficeCriticalSection criticalSection;

// CAVSDocxRenderer

[
	coclass,
	default(IAVSDocxRenderer),
	threading(apartment),
	vi_progid("AVSOfficeDocxRenderer.AVSDocxRenderer"),
	progid("AVSOfficeDocxRenderer.AVSDocxRenderer.1"),
	version(1.0),
	uuid("77B37E21-16F0-4BCC-8901-DFD89B962174"),
	helpstring("AVSDocxRenderer Class")
]
class ATL_NO_VTABLE CAVSDocxRenderer :
	public IAVSDocxRenderer
{
public:
	CAVSDocxRenderer():
    ooxDocument(DocxRenderer::TextAssociationTypeNoFrames)
	{
	}

    DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
	  this->fontSize = 0;
	  this->fontStyle = 0;

	  this->penColor = 0;
	  this->penSize = 0;

	  this->brushColor = 0;

	  tempPath = L"";
	  tempFile = L"";
	  tempDir = L"";
	  return S_OK;
	}

	void FinalRelease()
	{
	  this->pOfficeUtils.Release();

	  criticalSection.Enter();

	  if( boost::filesystem::is_directory(this->tempDir) )
		  boost::filesystem::remove_all( boost::filesystem::wpath( this->tempDir ) );

	  criticalSection.Leave(); 
	}

public:
    // IAVSRenderer Methods

	// ��� ���������-----------------------------------------------------------------------------
	STDMETHOD(get_Type)(LONG* lType);

	//-------- ������� ��� ������ �� ��������� --------------------------------------------------
	STDMETHOD(NewPage)();
	STDMETHOD(get_Height)(double* dHeight);
	STDMETHOD(put_Height)(double dHeight);
	STDMETHOD(get_Width)(double* dWidth);
	STDMETHOD(put_Width)(double dWidth);
	STDMETHOD(get_DpiX)(double* dDpiX);
	STDMETHOD(get_DpiY)(double* dDpiY);
	//-------- ������� ��� ������� �������� ������ ----------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML);

    STDMETHOD(get_PenColor)(LONG* lColor);
	STDMETHOD(put_PenColor)(LONG lColor);
	STDMETHOD(get_PenAlpha)(LONG* lAlpha);
	STDMETHOD(put_PenAlpha)(LONG lAlpha);
	STDMETHOD(get_PenSize)(double* dSize);
	STDMETHOD(put_PenSize)(double dSize);
	STDMETHOD(get_PenDashStyle)(BYTE* val);
	STDMETHOD(put_PenDashStyle)(BYTE val);
	STDMETHOD(get_PenLineStartCap)(BYTE* val);
	STDMETHOD(put_PenLineStartCap)(BYTE val);
	STDMETHOD(get_PenLineEndCap)(BYTE* val);
	STDMETHOD(put_PenLineEndCap)(BYTE val);
	STDMETHOD(get_PenLineJoin)(BYTE* val);
	STDMETHOD(put_PenLineJoin)(BYTE val);
	STDMETHOD(get_PenDashOffset)(double* val);
	STDMETHOD(put_PenDashOffset)(double val);
	STDMETHOD(get_PenAlign)(LONG* val);
	STDMETHOD(put_PenAlign)(LONG val);
	STDMETHOD(get_PenMiterLimit)(double* val);
	STDMETHOD(put_PenMiterLimit)(double val);
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern);

	STDMETHOD(SetBrush)(BSTR bsXML);

	STDMETHOD(get_BrushType)(LONG* lType);
	STDMETHOD(put_BrushType)(LONG lType);
	STDMETHOD(get_BrushColor1)(LONG* lColor);
	STDMETHOD(put_BrushColor1)(LONG lColor);
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha);
	STDMETHOD(get_BrushColor2)(LONG* lColor);
	STDMETHOD(put_BrushColor2)(LONG lColor);
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha);
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath);
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath);
	STDMETHOD(get_BrushTextureMode)(LONG* lMode);
	STDMETHOD(put_BrushTextureMode)(LONG lMode);
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha);
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha);
	STDMETHOD(get_BrushLinearAngle)(double* dAngle);
	STDMETHOD(put_BrushLinearAngle)(double dAngle);
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height);

	STDMETHOD(SetFont)(BSTR bsXML);

    STDMETHOD(get_FontName)(BSTR* bsName);
	STDMETHOD(put_FontName)(BSTR bsName);
	STDMETHOD(get_FontPath)(BSTR* bsName);
	STDMETHOD(put_FontPath)(BSTR bsName);
	STDMETHOD(get_FontSize)(double* dSize);
	STDMETHOD(put_FontSize)(double dSize);
	STDMETHOD(get_FontStyle)(LONG* lStyle);
	STDMETHOD(put_FontStyle)(LONG lStyle);
	STDMETHOD(get_FontStringGID)(BOOL* bGID);
	STDMETHOD(put_FontStringGID)(BOOL bGID);
	STDMETHOD(get_FontCharSpace)(double* dSpace);
	STDMETHOD(put_FontCharSpace)(double dSpace);

	STDMETHOD(SetShadow)(BSTR bsXML);

    STDMETHOD(get_ShadowDistanceX)(double* val);
	STDMETHOD(put_ShadowDistanceX)(double val);
	STDMETHOD(get_ShadowDistanceY)(double* val);
	STDMETHOD(put_ShadowDistanceY)(double val);
	STDMETHOD(get_ShadowBlurSize)(double* val);
	STDMETHOD(put_ShadowBlurSize)(double val);
	STDMETHOD(get_ShadowColor)(LONG* val);
	STDMETHOD(put_ShadowColor)(LONG val);
	STDMETHOD(get_ShadowAlpha)(LONG* val);
	STDMETHOD(put_ShadowAlpha)(LONG val);
	STDMETHOD(get_ShadowVisible)(BOOL* val);
	STDMETHOD(put_ShadowVisible)(BOOL val);

	STDMETHOD(SetEdgeText)(BSTR bsXML);

    STDMETHOD(get_EdgeVisible)(LONG* val);
	STDMETHOD(put_EdgeVisible)(LONG val);
	STDMETHOD(get_EdgeColor)(LONG* val);
	STDMETHOD(put_EdgeColor)(LONG val);
	STDMETHOD(get_EdgeAlpha)(LONG* val);
	STDMETHOD(put_EdgeAlpha)(LONG val);
	STDMETHOD(get_EdgeDist)(double* val);
	STDMETHOD(put_EdgeDist)(double val);

    //-------- ������� ��� ������ ������ --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(CommandDrawTextEx)(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	//-------- ������� ��� ������ ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType);
	STDMETHOD(EndCommand)(DWORD lType);
    //-------- ������� ��� ������ � Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY);
	STDMETHOD(PathCommandLineTo)(double fX, double fY);
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
	STDMETHOD(PathCommandClose)();
	STDMETHOD(PathCommandEnd)();
	STDMETHOD(DrawPath)(long nType);
	STDMETHOD(PathCommandStart)();
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY);

	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(PathCommandTextEx)(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
    //-------- ������� ��� ������ ����������� --------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight);
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight);
	//------------------------------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);

    STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF);
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
	STDMETHOD(ResetTransform)(void);
	STDMETHOD(get_ClipMode)(LONG* plMode);
	STDMETHOD(put_ClipMode)(LONG lMode);
	
    //IAVSDocxRenderer Methods
	STDMETHOD(Initialize)(BSTR bsXMLOptions);
    STDMETHOD(Save)();

private:
    const unsigned long LoadFromResource( LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName ) const;
	void CreateOriginDocx( const std::wstring& tempFileName, const std::wstring& tempDirName ) const;

private:
    std::vector<std::wstring> imageFiles;
	AVSOfficeUtils::IOfficeUtilsPtr pOfficeUtils;
	AVSGraphics::IAVSFontManagerPtr pFontManager;
	
	DocxRenderer::Document ooxDocument;

	std::wstring fontPath;
	double fontSize;
	std::wstring fontName;
	long fontStyle;

	long penColor;
	double penSize;

	long brushColor;	

    std::wstring tempPath;
    std::wstring tempFile;
	std::wstring tempDir;
};

