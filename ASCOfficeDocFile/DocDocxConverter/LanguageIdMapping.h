﻿#pragma once

#include "IMapping.h"
#include "PropertiesMapping.h"
#include "LanguageId.h"
#include "../Common/XmlTools.h"

namespace DocFileFormat
{
	typedef enum _LanguageType
	{
		Default,
		EastAsian,
		Complex
	} LanguageType;

	class LanguageIdMapping : public PropertiesMapping, public IMapping
	{
	public: 
		LanguageIdMapping( XmlUtils::CXmlWriter* writer, LanguageType type );
		LanguageIdMapping( XMLTools::XMLElement<wchar_t>* parentElement, LanguageType type );
		virtual ~LanguageIdMapping();
		void Apply( IVisitable* lid );

		static wstring getLanguageCode( LanguageId* lid );
	
	private: 
		LanguageType _type;
		XMLTools::XMLElement<wchar_t>* _parent;
	}; 
}
