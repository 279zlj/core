#pragma once

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#include <atldefine.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <wingdi.h>

#pragma warning ( disable : 4267 )

#define NODOCX
#define PPT_DEF
#define PPTX_DEF

#define INLINE_MEDIA_LIBRARIES		// ���������� ���������� � ImageManager.h

#include "../../../../../Common/GdiPlusEx.h"

#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>

#pragma comment( lib, "shell32.lib" ) // �������� shell32.lib

using namespace ATL;

//#import "X:/AVS/Redist/AVSVideoFile3.dll"					named_guids raw_interfaces_only rename_namespace("VideoFile")
#import "X:/AVS/Redist/AVSOfficeStudio/AVSOfficeUtils.dll"	raw_interfaces_only