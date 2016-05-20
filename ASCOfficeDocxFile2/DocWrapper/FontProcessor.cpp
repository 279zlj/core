#include "stdafx.h"
#include "FontProcessor.h"

#include "../../Common/ASCUtils.h"

using SimpleTypes::EPitch;

namespace DocWrapper {
	
	bool FontProcessor::useSystemFonts = false;
	
	FontProcessor::FontProcessor() 
			: fontManager(NULL) {}
	FontProcessor::~FontProcessor() {
		RELEASEINTERFACE(fontManager);
	}
	
	void FontProcessor::setFontDir(const CString& fontDir) {
		this->fontDir = fontDir;
		initFontManager();
	}
	void FontProcessor::setFontTable(OOX::CFontTable* fontTable) {
		for (int i = 0; i < fontTable->m_arrFonts.size(); ++i)
			addToFontMap(*fontTable->m_arrFonts[i]);
	}
	
	CString FontProcessor::getFont(const CString& name) {
		CString fontName = _T("Arial");
		if (fontMap.find(name) != fontMap.end())
			fontName = fontMap[name];
		else
		{
			OOX::CFont font;
			font.m_sName = name;
			addToFontMap(font);
			if (fontMap.find(name) != fontMap.end())
				fontName = fontMap[name];
		}
		return fontName;
	}
	void FontProcessor::getFonts(CAtlArray<CString>& fonts) {
		fonts.RemoveAll();
		std::map<CString, CString>::iterator it = fontMap.begin();
		for (; it != fontMap.end(); ++it) {
			bool contains = false;
			for (int i = 0; i < (int) fonts.GetCount(); ++i)
				if (fonts[i] == it->second) {
					contains = true;
					break;
				}
			if (!contains)
				fonts.Add(it->second);
		}
	}
	
	void FontProcessor::initFontManager() {
		RELEASEINTERFACE(fontManager);

		fontManager = NULL;
		CoInitialize(NULL);
		CoCreateInstance(ASCGraphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, __uuidof(ASCGraphics::IASCFontManager), (void**) &fontManager);

		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = fontDir.AllocSysString();
		fontManager->SetAdditionalParam(L"InitializeFromFolder", var);
		RELEASESYSSTRING(var.bstrVal);

		if (useSystemFonts) {
			CString options = _T("<FontManagerOptions><FontDir path='") + fontDir + _T("' /></FontManagerOptions>");
			BSTR bsOptions = options.AllocSysString();


#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
			fontManager->Init(bsOptions, VARIANT_TRUE, VARIANT_FALSE);
#else
			fontManager->Initialize(bsOptions);
#endif


			SysFreeString(bsOptions);
		}

#ifdef BUILD_CONFIG_FULL_VERSION
		CString defaultFontName = _T("Arial");
		BSTR defFontName = defaultFontName.AllocSysString();
		fontManager->SetDefaultFont(defFontName);
		SysFreeString(defFontName);
#endif
	}
	void FontProcessor::addToFontMap(OOX::CFont& font) {
		XmlUtils::CStringWriter parw;
		parw.WriteString(CString(_T("<FontProperties>")));
		if(font.m_oCharset.IsInit())
		{
			SimpleTypes::EFontCharset eCharset = font.m_oCharset->GetValue();
			//�� �������� �� ����� � ������ fontcharsetANSI � fontcharsetDefault, ������ ��� �� ������� �� ������, � dll �������� �� �������
			if(SimpleTypes::fontcharsetANSI != eCharset && SimpleTypes::fontcharsetDefault != eCharset)
				parw.WriteString(_T("<Charset value='") + font.m_oCharset->ToString() + _T("'/>"));
		}
		if(font.m_sName.IsEmpty())
			parw.WriteString(CString(_T("<Name value='Arial'/>")));
		else
		{
			CString sName = font.m_sName;
			ToXmlString(sName);
			parw.WriteString(_T("<Name value='")+ sName + _T("'/>"));
		}
		if (font.m_oAltName.IsInit())
		{
			CString sAltName = *font.m_oAltName;
			ToXmlString(sAltName);
			parw.WriteString(_T("<AltName value='") + sAltName + _T("'/>"));
		}
		parw.WriteString(_T("<FamilyClass name='") + font.m_oFamily.ToString() + _T("'/>"));
		if(font.m_oPanose.IsInit())
			parw.WriteString(_T("<Panose value='") + font.m_oPanose->ToString() + _T("'/>"));
		if (font.m_oPitch.GetValue() == SimpleTypes::pitchFixed)
			parw.WriteString(CString(_T("<FixedWidth value='1'/>")));
		else
			parw.WriteString(CString(_T("<FixedWidth value='0'/>")));
		parw.WriteString(CString(_T("<UnicodeRange ")));
		if (font.m_oUsb0.IsInit())
			parw.WriteString(_T("range1='") + font.m_oUsb0->ToString() + _T("' "));
		if (font.m_oUsb1.IsInit())
			parw.WriteString(_T("range2='") + font.m_oUsb1->ToString() + _T("' "));
		if (font.m_oUsb2.IsInit())
			parw.WriteString(_T("range3='") + font.m_oUsb2->ToString() + _T("' "));
		if (font.m_oUsb3.IsInit())
			parw.WriteString(_T("range4='") + font.m_oUsb3->ToString() + _T("' "));
		if (font.m_oCsb0.IsInit())
			parw.WriteString(_T("coderange1='") + font.m_oCsb0->ToString() + _T("' "));
		if (font.m_oCsb1.IsInit())
			parw.WriteString(_T("coderange2='") + font.m_oCsb1->ToString() + _T("' "));
		parw.WriteString(CString(_T("/>")));
		parw.WriteString(CString(_T("</FontProperties>")));
		CString params = parw.GetData();
		
		BSTR fontPath = NULL;
		BSTR familyName = NULL;
		long index = 0;
		BSTR bstrParams = params.AllocSysString();

#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
		fontManager->GetWinFontByParams(bstrParams, &familyName, &fontPath, NULL, &index);
#else
		fontManager->GetWinFontByParams(bstrParams, &fontPath, &index);
		int status = fontManager->LoadFontFromFile(fontPath, 12, 72, 72, index);
		/*if (!CheckRange(fontManager))
		{
			params = _T("<FontProperties>");
			CString charset;
			charset.Format(_T("%x"), m_Charset);
			params += _T("<Charset value='") + charset + _T("'/>");
			params += _T("<Name value='") + m_Name + _T("'/>");
			params += _T("<AltName value='") + m_AltName + _T("'/>");
			params += _T("</FontProperties>");

			index = 0;
			bstrParams = params.AllocSysString();
			fontManager->GetWinFontByParams(bstrParams, &bstrFontPath, &index);
			SysFreeString(bstrParams);
			status = fontManager->LoadFontFromFile(bstrFontPath, 12, 72, 72, index);
			SysFreeString(bstrFontPath);
		}*/
		fontManager->GetFamilyName(&familyName);		
#endif

		CString resFontName = familyName;

		SysFreeString(bstrParams);
		SysFreeString(fontPath);
		SysFreeString(familyName);

		fontMap[font.m_sName] = resFontName;
	}
	
	bool FontProcessor::checkRange(OOX::CFont& font) {
		return true;
		/*unsigned char charset = (unsigned char) m_Charset;
		unsigned long bit;
		unsigned int index;
		GetCodePageByCharset(charset, &bit, &index);
		VARIANT_BOOL rangeSuits = VARIANT_FALSE;
		fontManager->IsUnicodeRangeAvailable(bit, index, &rangeSuits);
		return rangeSuits == VARIANT_TRUE;*/
	}
	void FontProcessor::ToXmlString(CString& strVal)
	{
		strVal.Replace(L"&",	L"&amp;");
		strVal.Replace(L"'",	L"&apos;");
		strVal.Replace(L"<",	L"&lt;");
		strVal.Replace(L">",	L"&gt;");
		strVal.Replace(L"\"",	L"&quot;");
	}
	
}