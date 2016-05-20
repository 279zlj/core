#ifndef _FONT_ENGINE_H
#define _FONT_ENGINE_H

#include "FontConsts.h"
#include "WinFont.h"

class CFreeTypeFontEngine;
class CFontFile;
class CFont;

//-------------------------------------------------------------------------------------------------------------------------------

struct TInstalledFontCacheEntry
{
	CStringW      wsFamilyName;
	long          lStyle;

	CWinFontInfo *pWinFontInfo;
};

#define INSTALLED_FONTS_CACHE_SIZE 20

//-------------------------------------------------------------------------------------------------------------------------------
// CFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

class CFontEngine 
{
public:

	CFontEngine(BOOL bAntiAliasing, BOOL bUseKerning, BOOL bLoadWinFonts, CString wsWinFontsXml = _T(""), CString wsFontDir = _T(""));
	CFontEngine(BOOL bAntiAliasing, BOOL bUseKerning, BOOL bLoadWinFonts, BYTE* pBuffer = NULL, CString wsFontDir = _T(""));

	~CFontEngine();

	CFontFile *GetFontFile(wchar_t *wsFamilyName, BOOL bBold, BOOL bItalic);
	CFontFile *GetFontFile(wchar_t *wsFilePath, long lIndex);

	// ��������� �����. ���� � ����� � ����� ������ � ����� - ����� � ���������� ��� ����������������.
	CFontFile *LoadFont   (wchar_t *wsFamilyName, BOOL bBold, BOOL bItalic);
	CFontFile *LoadFont   (wchar_t *wsFilePath, long lIndex);

	// ������������� ���������� �����
	void SetDefaultFont(wchar_t *wsFamilyName);
	void SetDefaultFont(wchar_t *wsFilePath, long lIndex);

	// ���������� ��������� �� ����������� �����
	CFont *GetDefaultFont(BOOL bBold, BOOL bItalic);

	// ���������� ���������� � ������������� ������ �� ��� ����� � �����
	CWinFontInfo *GetInstalledFont(long lIndex);
	CWinFontInfo *GetInstalledFont(wchar_t *wsFamilyName, long lStyle, long *plIndex);
	long          GetInstalledFontsCount();
	BOOL          IsStyleAvailable(wchar_t *wsFamilyName, long lStyle);
	CWinFontInfo *GetInstalledFont(CString sParamsXml);
	CWinFontInfo *GetInstalledFont(wchar_t* wsAltFontName, long lCharset, wchar_t* wsFamilyClass, long lStyle, BOOL bFixed, wchar_t *wsPanose, long lUnicodeRange1, long lUnicodeRange2, long lUnicodeRange3, long lUnicodeRange4, long lCodePageRange1, long lCodePageRange2, long lAvgWidth);
	CString GetInstalledFonts(const CString& strName)
	{
		return m_pWinFontList->GetAllStylesByFontName(strName);
	}

	// ������� ���������� Font �� ������� FontFile: ������� ���� ���� � ����, � ���� Font �� ������, ����� ������� ����� ������
	// CFont � ��������� ��� � ���.
	CFont *GetFont(CFontFile *pFontFile);

	void SetAA(BOOL bAA);
private:

	void AddToCache(TInstalledFontCacheEntry oEntry);
	int  FindInCache(wchar_t *wsFamilyName, long lStyle);

private:

	CFont               *m_pFontCache[MAX_FONT_CACHE_SIZE];
	CFont               *m_pDefaultFont[4]; // { Regular, Bold, Italic, Bold Italic}

	CFreeTypeFontEngine *m_pFreeTypeEngine;
	CWinFontList        *m_pWinFontList;

	TInstalledFontCacheEntry m_arrInstalledFontsCache[INSTALLED_FONTS_CACHE_SIZE];
};

#endif /* _FONT_ENGINE_H */
