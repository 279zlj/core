﻿#pragma once

#include "PropertiesMapping.h"
#include "PictureDescriptor.h"
#include "TwipsValue.h"
#include "ConversionContext.h"
#include "OfficeDrawing/Shapetypes/PictureFrameType.h"
#include "OfficeDrawing/Shape.h"
#include "OfficeDrawing/MetafilePictBlip.h"
#include "OfficeDrawing/BitmapBlip.h"
#include "VMLShapeTypeMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class VMLPictureMapping: public PropertiesMapping, public IMapping
	{
	public:
		VMLPictureMapping( ConversionContext* ctx, XmlUtils::CXmlWriter* writer, bool olePreview, IMapping* caller, bool isBulletPicture = false );
		virtual ~VMLPictureMapping();
		virtual void Apply( IVisitable* visited );
		std::wstring GetShapeId() const;

	private:
		/// Writes a border element
		void writePictureBorder (const wchar_t* name, const BorderCode* brc);
		void appendStyleProperty( wstring* b, const wstring& propName, const wstring& propValue ) const;

	protected:
		/// Copies the picture from the binary stream to the zip archive 
		/// and creates the relationships for the image.
		bool CopyPicture (BlipStoreEntry* oBlipEntry);

	public:

		static std::wstring GetTargetExt (Global::BlipType nType);
		static std::wstring GetContentType (Global::BlipType nType);

		bool							m_isEquation;
		bool							m_isEmbedded;
		std::string						m_embeddedData;
	private:
		
		ConversionContext*				m_ctx;
		IMapping*						m_caller;
		int								m_nImageId;
		std::wstring					m_ShapeId;
		
		bool							m_isOlePreview;
		
		bool							m_isBulletPicture;
		XMLTools::XMLElement<wchar_t>*	m_imageData;
		
	};
}
