﻿/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "OOXShapeReader.h"
#include "OOXTextItemReader.h"

 OOXShapeReader::OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem)
{
    m_arrElement = elem;

    m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(elem);

    if (m_vmlElement==NULL)
    {
        OOX::Logic::CPicture* pict = dynamic_cast<OOX::Logic::CPicture*>(elem);

        if (pict)
        {
                 if (pict->m_oShape.IsInit())           m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShape.GetPointer());
            else if (pict->m_oShapeArc.IsInit())        m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeArc.GetPointer());
            else if (pict->m_oShapeCurve.IsInit())      m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeCurve.GetPointer());
            else if (pict->m_oShapeLine.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeLine.GetPointer());
            else if (pict->m_oShapeOval.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeOval.GetPointer());
            else if (pict->m_oShapePolyLine.IsInit())   m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapePolyLine.GetPointer());
            else if (pict->m_oShapeRect.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeRect.GetPointer());
            else if (pict->m_oShapeRoundRect.IsInit())  m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeRoundRect.GetPointer());
            else if (pict->m_oShapeType.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeType.GetPointer());
        }

    }
}

bool OOXShapeReader::Parse2( ReaderParameter oParam , RtfShapePtr& oOutput)
{
	if (m_arrElement == NULL) return false;

	for( int i = 0; i < m_arrElement->m_arrItems.size(); i++ )
	{
		if (m_arrElement->m_arrItems[i] == NULL) continue;

		switch(m_arrElement->m_arrItems[i]->getType())
		{
			case OOX::et_v_fill:
			{
				OOX::Vml::CFill* fill = dynamic_cast<OOX::Vml::CFill*>(m_arrElement->m_arrItems[i]); 
				if (!fill) break;
				
				CString srId = fill->m_sId.IsInit() ? fill->m_sId.get2() : _T("") ;

				if (srId.IsEmpty())
                    srId = fill->m_rId.IsInit() ? fill->m_rId->GetValue() : _T("") ;
				
				if (!srId.IsEmpty() && oParam.oReader->m_currentContainer)
				{        
					smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(srId);
				
					if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
					{
						OOX::Image* pImage = (OOX::Image*)oFile.operator->();
						CString sImagePath = pImage->filename().GetPath();

						oOutput->m_oPicture = RtfPicturePtr( new RtfPicture() );
						//todooo проверить что за путь тут выставляется
						OOXPictureGraphicReader::WriteDataToPicture( sImagePath, *oOutput->m_oPicture, oParam.oReader->m_sPath );
					}
				}
				if (fill->m_oColor.IsInit())
					oOutput->m_nFillColor = (fill->m_oColor->Get_B() << 16) + (fill->m_oColor->Get_G() << 8) + fill->m_oColor->Get_R();

				if (fill->m_oColor2.IsInit())
					oOutput->m_nFillColor2 = (fill->m_oColor2->Get_B() << 16) + (fill->m_oColor2->Get_G() << 8) + fill->m_oColor2->Get_R();

				if (fill->m_oOpacity.IsInit())
					oOutput->m_nFillOpacity = fill->m_oOpacity->GetValue() * 100;

				switch(fill->m_oType.GetValue())
				{
					case SimpleTypes::filltypeBackground:		oOutput->m_nFillType = 9;	break;
					case SimpleTypes::filltypeFrame :			oOutput->m_nFillType = 3;	break;
					case SimpleTypes::filltypeGradient:			oOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypeGradientCenter:	oOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypeGradientRadial:	oOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypeGradientUnscaled:	oOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypePattern:			oOutput->m_nFillType = 1;	break;
					case SimpleTypes::filltypeTile:				oOutput->m_nFillType = 2;	break;
					case SimpleTypes::filltypeSolid:
					default:
						break;
				}
				if (fill->m_oAngle.IsInit())
				{
					oOutput->m_nFillAngle = fill->m_oAngle->GetValue();
					if (oOutput->m_nFillType == 4) 
						oOutput->m_nFillType = 7;
				}
				if (fill->m_oFocus.IsInit())
				{
					oOutput->m_nFillFocus = fill->m_oFocus->GetValue();
				}
			}break;
			case OOX::et_v_stroke:
			{
				OOX::Vml::CStroke* stroke = dynamic_cast<OOX::Vml::CStroke*>(m_arrElement->m_arrItems[i]); 
				if (!stroke) break;
				oOutput->m_nLineDashing = stroke->m_oDahsStyle.GetValue(); //совпадают значения

				if (stroke->m_oColor.IsInit())
					oOutput->m_nLineColor = (stroke->m_oColor->Get_B() << 16) + (stroke->m_oColor->Get_G() << 8) + stroke->m_oColor->Get_R();
			}break;
			case OOX::et_v_imagedata:
			{
				OOX::Vml::CImageData* image_data = dynamic_cast<OOX::Vml::CImageData*>(m_arrElement->m_arrItems[i]);
				if (!image_data) break;

				CString srId = image_data->m_oId.IsInit() ? image_data->m_oId.get2() : _T("") ;

				if (srId.IsEmpty())
                    srId = image_data->m_rId.IsInit() ? image_data->m_rId->GetValue() : _T("") ;

				if (oParam.oReader->m_currentContainer)
				{        
					smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(srId);
				
					if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
					{
						oOutput->m_oPicture		= RtfPicturePtr( new RtfPicture() );
						oOutput->m_eShapeType	= RtfShape::st_none;
						oOutput->m_nShapeType	= 75;

						OOX::Image* pImage = (OOX::Image*)oFile.operator->();

						CString sImagePath = pImage->filename().GetPath();
						
						//todooo проверить что за путь тут выставляется
						OOXPictureGraphicReader::WriteDataToPicture( sImagePath, *oOutput->m_oPicture, oParam.oReader->m_sPath );
					}
				}
				int nCropedWidthGoal = oOutput->m_oPicture->m_nWidthGoal;
				if( PROP_DEF != nCropedWidthGoal )
				{
					//делаем crop
					if( image_data->m_oCropLeft.IsInit() )
					{
						float nCropLeft = image_data->m_oCropLeft->GetValue();
						//oOutput->m_oPicture->m_nCropL = nCropLeft * oOutput->m_oPicture->m_nWidthGoal * oOutput->m_oPicture->m_nScaleX / 100;
						oOutput->m_oPicture->m_nCropL = (int)(nCropLeft * oOutput->m_oPicture->m_nWidthGoal);
						oOutput->m_nCropFromLeft = (int)(nCropLeft * 65536);
						nCropedWidthGoal -= oOutput->m_oPicture->m_nCropL;
					}
					if( image_data->m_oCropRight.IsInit())
					{
						float nCropRight =image_data->m_oCropRight->GetValue();
						//oOutput->m_oPicture->m_nCropR = nCropRight * oOutput->m_oPicture->m_nWidthGoal * oOutput->m_oPicture->m_nScaleX / 100;
						oOutput->m_oPicture->m_nCropR = (int)(nCropRight * oOutput->m_oPicture->m_nWidthGoal);
						oOutput->m_nCropFromRight = (int)(nCropRight * 65536);
						nCropedWidthGoal -= oOutput->m_oPicture->m_nCropR;
					}
				}
				int nCropedHeightGoal = oOutput->m_oPicture->m_nHeightGoal;
				if( PROP_DEF != nCropedHeightGoal )
				{
					if( image_data->m_oCropTop.IsInit() )
					{
						float nCropTop = image_data->m_oCropTop->GetValue();
						//oOutput->m_oPicture->m_nCropT = nCropTop * oOutput->m_oPicture->m_nHeightGoal * oOutput->m_oPicture->m_dScaleY / 100;
						oOutput->m_oPicture->m_nCropT = (int)(nCropTop * oOutput->m_oPicture->m_nHeightGoal);
						oOutput->m_nCropFromTop = (int)(nCropTop * 65536);
						nCropedHeightGoal -= oOutput->m_oPicture->m_nCropT;
					}
					if( image_data->m_oCropBottom.IsInit())
					{
						float nCropBottom = image_data->m_oCropBottom->GetValue();
						//oOutput->m_oPicture->m_nCropT = nCropTop * oOutput->m_oPicture->m_nHeightGoal * oOutput->m_oPicture->m_dScaleY / 100;
						oOutput->m_oPicture->m_nCropB = (int)(nCropBottom * oOutput->m_oPicture->m_nHeightGoal);
						oOutput->m_nCropFromBottom = (int)(nCropBottom * 65536);
						nCropedHeightGoal -= oOutput->m_oPicture->m_nCropB;
					}
				}
				//устанавливаем scale
				if( PROP_DEF != oOutput->m_nLeft && PROP_DEF != oOutput->m_nRight && PROP_DEF != nCropedWidthGoal && 0 != nCropedWidthGoal )
				{
					int nWidth = oOutput->m_nRight - oOutput->m_nLeft;
					double dNewScale = 100 * ( 1.0 * nWidth / nCropedWidthGoal );
					oOutput->m_oPicture->m_dScaleX = dNewScale;
				}
				if( PROP_DEF != oOutput->m_nTop && PROP_DEF != oOutput->m_nBottom && PROP_DEF != nCropedHeightGoal && 0 != nCropedHeightGoal )
				{
					int nHeight = oOutput->m_nBottom - oOutput->m_nTop;
					double dNewScale = 100 * ( 1.0 * nHeight / nCropedHeightGoal );
					oOutput->m_oPicture->m_dScaleY = dNewScale;
				}
			}break;
			case OOX::et_wd_wrap:
			{
				OOX::VmlWord::CWrap *wrap = dynamic_cast<OOX::VmlWord::CWrap*>(m_arrElement->m_arrItems[i]);

				if (wrap->m_oType.IsInit())
				{
					switch(wrap->m_oType->GetValue())
					{
					case SimpleTypes::wraptypeNone :	oOutput->m_nWrapType = 3;	break;
					case SimpleTypes::wraptypeSquare:	oOutput->m_nWrapType = 2;	break;
					case SimpleTypes::wraptypeThrough:	oOutput->m_nWrapType = 5;	break;
					case SimpleTypes::wraptypeTight:	oOutput->m_nWrapType = 4;	break;
					case SimpleTypes::wraptypeTopAndBottom:	oOutput->m_nWrapType = 1;	break;
					}
				}
				if (wrap->m_oSide.IsInit())
				{
					switch(wrap->m_oSide->GetValue())
					{
					case SimpleTypes::wrapsideBoth:    oOutput->m_nWrapSideType = 0; break;		
					case SimpleTypes::wrapsideLargest: oOutput->m_nWrapSideType = 3; break;					
					case SimpleTypes::wrapsideLeft:    oOutput->m_nWrapSideType = 1; break;					
					case SimpleTypes::wrapsideRight:   oOutput->m_nWrapSideType = 2; break;		
					}
				}
			}break;
			case OOX::et_v_textbox:
			{
				OOX::Vml::CTextbox *text_box= dynamic_cast<OOX::Vml::CTextbox*>(m_arrElement->m_arrItems[i]);

				if ((text_box) && (text_box->m_oTxtbxContent.IsInit()))
				{
					OOXTextItemReader oTextItemReader;
					
					for (long i=0; i < text_box->m_oTxtbxContent->m_arrItems.size(); i++)
					{
						oTextItemReader.Parse( text_box->m_oTxtbxContent->m_arrItems[i], oParam );
					}

					if( oTextItemReader.m_oTextItems->GetCount() > 0 )
						oOutput->m_aTextItems = oTextItemReader.m_oTextItems;
				}
			}break;
			case OOX::et_v_textpath:
			{
				OOX::Vml::CTextPath *text_path= dynamic_cast<OOX::Vml::CTextPath*>(m_arrElement->m_arrItems[i]);
				if (text_path)
				{
					oOutput->m_bGtext			= 1;
					if (text_path->m_sString.IsInit())
					{
						oOutput->m_sGtextUNICODE	= text_path->m_sString.get();
					}
					if (text_path->m_oStyle.IsInit())
					{
						ParseStyles( *oOutput, text_path->m_oStyle->m_arrProperties );
					}
				}

			}break;
		}
	}
	//проверяем на inline
	if((PROP_DEF == oOutput->m_nLeft/* || 0 == oOutput->m_nLeft */) && ( PROP_DEF == oOutput->m_nTop/* || 0 == oOutput->m_nTop */) && 
		PROP_DEF == oOutput->m_nPositionH && PROP_DEF == oOutput->m_nPositionV )
		oOutput->m_eShapeType = RtfShape::st_inline;
	//если ничего не задали делаем inline
	if( (PROP_DEF == oOutput->m_nLeft || 0 == oOutput->m_nLeft ) && ( PROP_DEF == oOutput->m_nTop || 0 == oOutput->m_nTop ) && PROP_DEF == oOutput->m_nPositionH && PROP_DEF == oOutput->m_nPositionV )
	{
		oOutput->m_nLeft = 0;
		oOutput->m_nTop = 0;
		oOutput->m_nPositionHRelative = 3;
		oOutput->m_nPositionVRelative = 3;
	}
	return true;
}
bool OOXShapeReader::Parse( ReaderParameter oParam , RtfShapePtr& oOutput)
{
	if (m_vmlElement == NULL && m_arrElement) return false;

	if (m_vmlElement == NULL ) return Parse2(oParam , oOutput);

	if( m_vmlElement->m_sId.IsInit())
	{
		oOutput->m_nID = oParam.oReader->m_oOOXIdGenerator.GetId( m_vmlElement->m_sId.get());
	}

	//oOutput->m_nLeft		= 0; //стили только с widht height (например в Numbering)
	//oOutput->m_nTop		= 0;
	oOutput->m_eShapeType	= RtfShape::st_none; //inline or anchor
	
	if ( m_vmlElement->m_oStyle.IsInit())
	{
		if( false == ParseStyles( *oOutput, m_vmlElement->m_oStyle->m_arrProperties ) )
			return false;
	}
	if (OOX::Vml::CShape* shape = dynamic_cast<OOX::Vml::CShape*>(m_vmlElement))
	{
		if (shape->m_oAdj.IsInit())
			ParseAdjustment( *oOutput, shape->m_oAdj.get() );
		
		if (shape->m_sType.IsInit())
		{
			int pos = shape->m_sType->Find(_T("#_x0000_t"));
			if (pos >= 0)
			{				
				oOutput->m_nShapeType	= _wtoi(shape->m_sType->Mid(pos + 9, shape->m_sType->GetLength() - pos - 9).GetString());
			}
		}
	}
	if (OOX::Vml::CRect* rect = dynamic_cast<OOX::Vml::CRect*>(m_vmlElement))
	{
		oOutput->m_nShapeType	= 1;
	}
				
	if (m_vmlElement->m_oFilled.IsInit())
		oOutput->m_bFilled = m_vmlElement->m_oFilled->GetValue() ==  SimpleTypes::booleanFalse ? 0 : 1;

	if (m_vmlElement->m_oStroked.IsInit())
		oOutput->m_bLine = m_vmlElement->m_oStroked->GetValue() ==  SimpleTypes::booleanFalse ? 0 : 1;

	if (m_vmlElement->m_oFillColor.IsInit())
		oOutput->m_nFillColor = (m_vmlElement->m_oFillColor->Get_B() << 16) + (m_vmlElement->m_oFillColor->Get_G() << 8) + m_vmlElement->m_oFillColor->Get_R();

	if (m_vmlElement->m_oStrokeColor.IsInit())
		oOutput->m_nLineColor = (m_vmlElement->m_oStrokeColor->Get_B() << 16) + (m_vmlElement->m_oStrokeColor->Get_G() << 8) + m_vmlElement->m_oStrokeColor->Get_R();

	if( m_vmlElement->m_oStrokeWeight.IsInit())
		oOutput->m_nLineWidth = m_vmlElement->m_oStrokeWeight->ToEmu();

	switch(m_vmlElement->m_oConnectorType.GetValue())
	{
		case SimpleTypes::connectortypeCurved	: oOutput->m_nConnectionType = 2; break;
		case SimpleTypes::connectortypeElbow	: oOutput->m_nConnectionType = 1; break;
		case SimpleTypes::connectortypeNone		: oOutput->m_nConnectionType = 3; break;
		case SimpleTypes::connectortypeStraight	: oOutput->m_nConnectionType = 0; break;
	}

	oOutput->m_bLayoutInCell = m_vmlElement->m_oAllowInCell.GetValue();
	oOutput->m_bAllowOverlap = m_vmlElement->m_oAllowOverlap.GetValue();

	if( m_vmlElement->m_oWrapCoords.IsInit())
	{
		int nPosition = 0;
		CString sPoint =  _T("start");
		for (long i =0 ;i < m_vmlElement->m_oWrapCoords->GetSize(); i++)
		{
			oOutput->m_aWrapPoints.push_back( m_vmlElement->m_oWrapCoords->GetX(i));// todooo y???
		}
	}

	if( m_vmlElement->m_oCoordOrigin.IsInit() )
	{
		oOutput->m_nGroupLeft = m_vmlElement->m_oCoordOrigin->GetX();
		oOutput->m_nGroupTop = m_vmlElement->m_oCoordOrigin->GetY();
	}

	if( m_vmlElement->m_oCoordSize.IsInit())
	{
		oOutput->m_nGroupRight = oOutput->m_nGroupLeft + m_vmlElement->m_oCoordSize->GetX();
		oOutput->m_nGroupBottom = oOutput->m_nGroupTop + m_vmlElement->m_oCoordSize->GetY();
	}

	return Parse2(oParam, oOutput);


	return true;
}

bool OOXShapeReader::ParseStyle(RtfShape& oShape, SimpleTypes::Vml::CCssProperty* prop)
{
	if (prop == NULL) return false;

	switch(prop->get_Type())
	{
		case SimpleTypes::Vml::cssptUnknown : 
			break;
		case SimpleTypes::Vml::cssptFlip : 
			switch(oShape.m_bFlipH = prop->get_Value().eFlip)
			{
				case SimpleTypes::Vml::cssflipX:	oShape.m_bFlipH = true; break; 
				case SimpleTypes::Vml::cssflipY:	oShape.m_bFlipV = true; break;
				case SimpleTypes::Vml::cssflipXY:	oShape.m_bFlipH = true; oShape.m_bFlipV = true; break; 
				case SimpleTypes::Vml::cssflipYX :	oShape.m_bFlipH = true; oShape.m_bFlipV = true; break; 
			}
			break;
		case SimpleTypes::Vml::cssptWidth : 
			{
				//OOX::SimpleTypes::CPoint oPoint;
				//oPoint.FromPoints(prop->get_Value().dValue);
				
				oShape.m_nRight = oShape.m_nRelRight = (int)(20 * prop->get_Value().oValue.dValue);
				
				if( PROP_DEF != oShape.m_nLeft )
					oShape.m_nRight += oShape.m_nLeft;
				if( PROP_DEF != oShape.m_nRelLeft)
					oShape.m_nRelRight += oShape.m_nRelLeft;
			}break;
		case SimpleTypes::Vml::cssptHeight : 
			{
				oShape.m_nBottom  = oShape.m_nRelBottom = (int)(20 * prop->get_Value().oValue.dValue);
				
				if( PROP_DEF != oShape.m_nTop )
					oShape.m_nBottom += oShape.m_nTop;
				if( PROP_DEF != oShape.m_nRelTop)
					oShape.m_nRelBottom += oShape.m_nRelTop;
			}break;
		case SimpleTypes::Vml::cssptLeft :
			{
				oShape.m_nRelLeft	= prop->get_Value().oValue.dValue;					
				if( PROP_DEF != oShape.m_nRelRight)
					oShape.m_nRelRight += oShape.m_nRelLeft;
			}break;
		case SimpleTypes::Vml::cssptTop : 
			{
				oShape.m_nRelTop = prop->get_Value().oValue.dValue;
				if( PROP_DEF != oShape.m_nRelBottom)
					oShape.m_nRelBottom += oShape.m_nRelTop;
			}break;
		case SimpleTypes::Vml::cssptMarginBottom: 
			{
				oShape.m_nBottom	=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;
		case SimpleTypes::Vml::cssptMarginLeft :
			{
				oShape.m_nLeft		=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;//pt tp twips
		case SimpleTypes::Vml::cssptMarginRight : 
			{
				oShape.m_nRight	=  (int)(20 * prop->get_Value().oValue.dValue );		
			}break;
		case SimpleTypes::Vml::cssptMarginTop:
			{
				oShape.m_nTop		=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoPositionHorizontal: 
			{
				oShape.m_nPositionH = prop->get_Value().eMsoPosHor;	
			}break;
		case SimpleTypes::Vml::cssptMsoPositionHorizontalRelative  :
			{
				oShape.m_nPositionHRelative = prop->get_Value().eMsoPosHorRel;	
			}break;
		case SimpleTypes::Vml::cssptMsoPositionVertical: 
			{
				oShape.m_nPositionV = prop->get_Value().eMsoPosVer;		
			}break;
		case SimpleTypes::Vml::cssptMsoPositionVerticalRelative    :
			{
				oShape.m_nPositionVRelative = prop->get_Value().eMsoPosVerRel;		
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceBottom:
			{
				oShape.m_nWrapDistBottom =  (int)(20 * prop->get_Value().dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceLeft : 
			{
				oShape.m_nWrapDistLeft = (int)(20 * prop->get_Value().dValue );		
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceRight:
			{
				oShape.m_nWrapDistRight = (int)(20 * prop->get_Value().dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceTop : 
			{
				oShape.m_nWrapDistTop =  (int)(20 * prop->get_Value().dValue );		
			}break;
		case SimpleTypes::Vml::cssptMsoWrapEdited: 
			break;
		case SimpleTypes::Vml::cssptMsoWrapStyle: 
			break;
		case SimpleTypes::Vml::cssptPosition: 
			break;
		case SimpleTypes::Vml::cssptRotation: 
			{
				oShape.m_nRotation = 65536 * prop->get_Value().dValue;
				if( PROP_DEF != oShape.m_nRelRight || PROP_DEF != oShape.m_nRelLeft || PROP_DEF != oShape.m_nRelTop || PROP_DEF != oShape.m_nRelBottom  )
					oShape.m_nRelRotation = oShape.m_nRotation;
			}break;
		case SimpleTypes::Vml::cssptVisibility :  
			break;
		case SimpleTypes::Vml::cssptZIndex : 
			{
				oShape.m_eShapeType	= RtfShape::st_anchor;

				int nValue = prop->get_Value().oZIndex.nOrder;
				oShape.m_nZOrder = nValue;
				if( nValue > 0 )
					oShape.m_nZOrderRelative = 0;
				else if( nValue < 0 )
					oShape.m_nZOrderRelative = 1;
			}break;
		case SimpleTypes::Vml::cssptFontFamily:
			{
				oShape.m_sGtextFont = CString(prop->get_Value().wsValue);
			}break;
		case SimpleTypes::Vml::cssptFontSize:
			{
				oShape.m_nGtextSize = prop->get_Value().oValue.dValue;
			}break;

		case SimpleTypes::Vml::cssptDirection                      : 			break;
		case SimpleTypes::Vml::cssptLayoutFlow                     : 			break;
		case SimpleTypes::Vml::cssptMsoDirectionAlt                : 			break;
		case SimpleTypes::Vml::cssptMsoFitShapeToText              : 			break;
		case SimpleTypes::Vml::cssptMsoFitTextToShape              : 			break;
		case SimpleTypes::Vml::cssptMsoLayoutFlowAlt               : 			break;
		case SimpleTypes::Vml::cssptMsoNextTextbox                 :			break;
		case SimpleTypes::Vml::cssptMsoRotate                      : 			break;
		case SimpleTypes::Vml::cssptMsoTextScale                   : 			break;
		case SimpleTypes::Vml::cssptVTextAnchor                    :			break;
		case SimpleTypes::Vml::cssptFont                           :			break;
		case SimpleTypes::Vml::cssptFontStyle                      : 			break;
		case SimpleTypes::Vml::cssptFontVariant                    :			break;
		case SimpleTypes::Vml::cssptFontWeight                     :			break;
		case SimpleTypes::Vml::cssptMsoTextShadow                  : 			break;
		case SimpleTypes::Vml::cssptTextDecoration                 : 			break;
		case SimpleTypes::Vml::cssptVRotateLetters                 : 			break;
		case SimpleTypes::Vml::cssptVSameLetterHeights             : 			break;
		case SimpleTypes::Vml::cssptVTextAlign                     : 			break;
		case SimpleTypes::Vml::cssptVTextKern                      : 			break;
		case SimpleTypes::Vml::cssptVTextReverse                   : 			break;
		case SimpleTypes::Vml::cssptVTextSpacingMode               : 			break;
		case SimpleTypes::Vml::cssptVTextSpacing                   : 			break;		
	}

	//else if(  _T("mso-left-percent") == sProperty )
	//	oShape.m_nPositionHPct = Strings::ToInteger( sValue );
	//else if(  _T("mso-position-horizontal-relative") == sProperty )
	//{
	//	if( _T("page") == sValue )
	//		oShape.m_eXAnchor = RtfShape::ax_page;
	//	else if( _T("margin") == sValue )
	//		oShape.m_eXAnchor = RtfShape::ax_margin;
	//	else if( _T("text") == sValue )
	//		oShape.m_eXAnchor = RtfShape::ax_column;
	//}
	//else if(  _T("mso-top-percent") == sProperty )
	//	oShape.m_nPositionVPct = Strings::ToInteger( sValue );
	//else if(  _T("mso-position-vertical-relative") == sProperty )
	//{
	//	if( _T("page") == sValue )
	//		oShape.m_eYAnchor = RtfShape::ay_page;
	//	else if( _T("margin") == sValue )
	//		oShape.m_eYAnchor = RtfShape::ay_margin;
	//	else if( _T("text") == sValue )
	//		oShape.m_eYAnchor = RtfShape::ay_margin;
	//}
	//else if(  _T("mso-width-relative") == sProperty )
	//{
	//	if( _T("margin") == sValue )
	//		oShape.m_nPctWidthRelative = 0;
	//	else if( _T("page") == sValue )
	//		oShape.m_nPctWidthRelative = 1;
	//	else if( _T("left-margin-area") == sValue )
	//		oShape.m_nPctWidthRelative = 2;
	//	else if( _T("right-margin-area") == sValue )
	//		oShape.m_nPctWidthRelative = 3;
	//	else if( _T("inner-margin-area") == sValue )
	//		oShape.m_nPctWidthRelative = 4;
	//	else if( _T("outer-margin-area") == sValue )
	//		oShape.m_nPctWidthRelative = 5;

	//}
	//else if(  _T("mso-height-relative") == sProperty )
	//{
	//	if( _T("margin") == sValue )
	//		oShape.m_nPctHeightRelative = 0;
	//	else if( _T("page") == sValue )
	//		oShape.m_nPctHeightRelative = 1;
	//	else if( _T("top-margin-area") == sValue )
	//		oShape.m_nPctHeightRelative = 2;
	//	else if( _T("bottom-margin-area") == sValue )
	//		oShape.m_nPctHeightRelative = 3;
	//	else if( _T("inner-margin-area") == sValue )
	//		oShape.m_nPctHeightRelative = 4;
	//	else if( _T("outer-margin-area") == sValue )
	//		oShape.m_nPctHeightRelative = 5;
	//}

	return true;
}

