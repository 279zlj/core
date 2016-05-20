#pragma once

#include "stdAfx.h"

#include "XlsxConverter.h"

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"

using namespace cpdoccore;

namespace Oox2Odf
{

XlsxConverter::XlsxConverter(const std::wstring & path) 
{
	const OOX::CPath oox_path(CString(path.c_str()));			

	xlsx_document = new OOX::Spreadsheet::CXlsx(oox_path);	
	output_document = new odf::package::odf_document(L"spreadsheet");
}

void XlsxConverter::write(const std::wstring & path)
{
	if (!output_document)return;
	output_document->write(path);
}

void XlsxConverter::convert()
{
	if (!xlsx_document)return;
	if (!output_document)return;

	ods_context = new odf::ods_conversion_context(output_document);

	if (!ods_context)return;
	
	convert_styles();
		
	ods_context->start_document();

	convert_sheets();

	ods_context->end_document();

}
void XlsxConverter::convert_sheets()
{
	if (!ods_context) return;
	
	const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->GetWorkbook();
	if (!Workbook) return;

	CAtlMap<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = xlsx_document->GetWorksheets();
	if(Workbook->m_oSheets.IsInit())
	{			
		CSimpleArray<OOX::Spreadsheet::CSheet*>& aWs = Workbook->m_oSheets->m_arrItems;//������ ������ ����� 
		
		for(int i = 0, length = aWs.GetSize(); i < length; ++i)
		{
			OOX::Spreadsheet::CSheet* pSheet = aWs[i];
				
			if(pSheet->m_oRid.IsInit())
			{
				CString sSheetRId = pSheet->m_oRid.get2().ToString();
				CAtlMap<CString, OOX::Spreadsheet::CWorksheet*>::CPair* pPair = arrWorksheets.Lookup(sSheetRId);
				if (NULL != pPair)
				{
					OOX::Spreadsheet::CWorksheet *pWorksheet = pPair->m_value;
					if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
					{
						std::wstring name = string2std_string(pSheet->m_oName.get2());
						ods_context->start_sheet(name);
						convert(pWorksheet);
						ods_context->end_sheet();						
					}
				}

			}
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	//������� ��������� �������� 
	if (oox_sheet->m_oSheetFormatPr.IsInit())
		convert(oox_sheet->m_oSheetFormatPr.GetPointer());
	//if (oox_sheet->m_oSheetPr.IsInit())
	//	convert(oox_sheet->m_oSheetPr.GetPointer());

	//�������
	ods_context->start_columns();
		for (long col = 0 ; oox_sheet->m_oCols.IsInit() && col < oox_sheet->m_oCols->m_arrItems.GetSize(); col++)
		{
			convert(oox_sheet->m_oCols->m_arrItems[col]);
		}
	ods_context->end_columns();

	//������
	ods_context->start_rows();
		for (long row = 0 ; oox_sheet->m_oSheetData.IsInit() && row < oox_sheet->m_oSheetData->m_arrItems.GetSize(); row++)
		{
			convert(oox_sheet->m_oSheetData->m_arrItems[row]);
		}
	ods_context->end_rows();
}
void XlsxConverter::convert(OOX::Spreadsheet::CRow *oox_row)
{
	int row_number = oox_row->m_oR.IsInit() ? oox_row->m_oR->GetValue() : -1;

	bool _default = true;
	
	if (oox_row->m_oHt.IsInit() || oox_row->m_oCustomFormat.IsInit()) _default = false;
	
	ods_context->start_row(row_number,1,_default);
	
	if (oox_row->m_oHidden.IsInit())		ods_context->current_table().set_row_hidden(true);
	if (oox_row->m_oCollapsed.IsInit())		ods_context->current_table().set_row_hidden(true);
	
	if (oox_row->m_oS.IsInit() && ( oox_row->m_oCustomFormat.IsInit() && oox_row->m_oCustomFormat->GetValue()==1))
	{
		int xfd_id = oox_row->m_oS->GetValue();

		std::wstring style_cell_name  = ods_context->styles_context().find_odf_style_name(xfd_id,odf::style_family::TableCell);
		ods_context->current_table().set_row_default_cell_style(style_cell_name );
	}
	if (oox_row->m_oHt.IsInit() == true)
	{
		double height = oox_row->m_oHt->GetValue();
		ods_context->current_table().set_row_height(height);
		//����� �������������� ����������� !!!
	}
	if (oox_row->m_oCustomHeight.IsInit() && oox_row->m_oCustomHeight->GetValue() == 1)
	{ 
		ods_context->current_table().set_row_optimal_height(false);
	}

	for (long cell = 0 ; cell < oox_row->m_arrItems.GetSize();cell++)
	{
		convert(oox_row->m_arrItems[cell]);
	}
	ods_context->end_row();
}
void XlsxConverter::convert(OOX::Spreadsheet::CCell *oox_cell)
{
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCellMetadata;
				//nullable<SimpleTypes::COnOff<>>						m_oShowPhonetic;
				//nullable<SimpleTypes::Spreadsheet::CCellTypeType<>>	m_oType;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

	std::wstring ref = oox_cell->m_oRef.IsInit() ? string2std_string(oox_cell->m_oRef.get()) : L"";

	int ifx_style = oox_cell->m_oStyle.IsInit() ? oox_cell->m_oStyle->GetValue() : -1;

	ods_context->start_cell(ref,ifx_style);

				//nullable<CFormula>	m_oFormula;
				//nullable<CSi>		m_oRichText;
				//nullable<CText>		m_oValue;

	ods_context->end_cell();
}
void XlsxConverter::convert(OOX::Spreadsheet::CCol *oox_column)
{
	if (oox_column == NULL)return;

	int start_column = oox_column->m_oMin.IsInit() ? oox_column->m_oMin->GetValue() : 0 ;
	int repeated =	(oox_column->m_oMax.IsInit() ? oox_column->m_oMax->GetValue() : 0) - 
					(oox_column->m_oMin.IsInit() ? oox_column->m_oMin->GetValue() : 0) + 1;

	ods_context->add_column(start_column, repeated);

	double width = oox_column->m_oWidth.IsInit() ? oox_column->m_oWidth->GetValue() : -1;

	if (width < 0 || (oox_column->m_oBestFit.IsInit()  && oox_column->m_oBestFit->GetValue() == 1))
	{
		ods_context->current_table().set_column_optimal_width(true);
	}
	else if (oox_column->m_oCustomWidth.IsInit() == false || 
			(oox_column->m_oCustomWidth.IsInit() == true && oox_column->m_oCustomWidth->GetValue() == 1))
	{
		ods_context->current_table().set_column_width(width);
		ods_context->current_table().set_column_optimal_width(false);
		// ���� �� ��������������� ��������� ����������� !!!
		//???
	}

	if (oox_column->m_oStyle.IsInit())
	{	
		int xfd_id = oox_column->m_oStyle->GetValue();

		std::wstring style_cell_name  = ods_context->styles_context().find_odf_style_name(xfd_id,odf::style_family::TableCell);
		ods_context->current_table().set_column_default_cell_style(style_cell_name );
	}
	else
	{
		//����� �������� ��������� ����� ��� ����� ������� �����
		//???
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CSheetFormatPr *oox_sheet_format)
{
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBaseColWidth;
				//nullable<SimpleTypes::COnOff<>>					m_oCustomHeight;
				//nullable<SimpleTypes::CDouble>					m_oDefaultColWidth;
				//nullable<SimpleTypes::CDouble>					m_oDefaultRowHeight;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelCol;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelRow;
				//nullable<SimpleTypes::COnOff<>>					m_oThickBottom;
				//nullable<SimpleTypes::COnOff<>>					m_oThickTop;
				//nullable<SimpleTypes::COnOff<>>					m_oZeroHeight;
//���� �� ������� :(
	if (oox_sheet_format->m_oDefaultColWidth.IsInit())
	{
		ods_context->styles_context().create_style(L"",odf::style_family::TableColumn, true, false, -1);		
		{	
			double width =  oox_sheet_format->m_oDefaultColWidth->GetValue();

			odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
			if (style)
			{
				odf::style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 				if (column_properties)
					column_properties->style_table_column_properties_attlist_.style_column_width_ = odf::length(width/2.,odf::length::cm);
			}
		}		
		ods_context->styles_context().add_default(ods_context->styles_context().last_state());
	}
	if (oox_sheet_format->m_oDefaultRowHeight.IsInit())
	{
		ods_context->styles_context().create_style(L"",odf::style_family::TableRow, true, false, -1);	
		{
			double height = oox_sheet_format->m_oDefaultRowHeight->GetValue();

			odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
			if (style)
			{
				odf::style_table_row_properties * row_properties = style->style_content_.get_style_table_row_properties();
 				if (row_properties)
					row_properties->style_table_row_properties_attlist_.style_row_height_ = odf::length(height,odf::length::pt);
				row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; //???? �� ���� c���� �� 
			}
		}
		ods_context->styles_context().add_default(ods_context->styles_context().last_state());
	}

}
void XlsxConverter::convert_styles()
{
	if (!ods_context) return;
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
	if (!xlsx_styles)return;

	if (!xlsx_styles->m_oCellStyleXfs.IsInit())return;

	for (long i=0;i< xlsx_styles->m_oCellStyleXfs->m_oCount->GetValue(); i++)//in styles record
	{
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, false, true);
	}
	
	for (long i=0;i< xlsx_styles->m_oCellXfs->m_oCount->GetValue(); i++)
	{
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i],i, true,false);

	}
	
	for (long i=0;i< xlsx_styles->m_oCellStyles->m_oCount->GetValue(); i++)//styles.xml
	{
		//���� ����� ��� � 1 �� � ������ .. ���� ���������, �������, ...
		//� ��� ����������
		convert(xlsx_styles->m_oCellStyles->m_arrItems[i],i);
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CFont * font, odf::office_element_ptr  & odf_style_)
{
	if (font == NULL)return;
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;
	
	odf::style_text_properties * text_properties = style->style_content_.get_style_text_properties();//���� �������� ��� - ����������

 	if (text_properties == NULL)return;
	if (font->m_oBold.IsInit())
	{
		if (font->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}
	if (font->m_oColor.IsInit())
	{
		convert(font->m_oColor.GetPointer(),text_properties->content().fo_color_);
	}
	if (font->m_oUnderline.IsInit())
	{
		//convert_element ????
	}
	if (font->m_oItalic.IsInit())
	{
		if (font->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Normal);
	}
	if (font->m_oSz.IsInit())
	{
		convert(font->m_oSz->m_oVal->GetValue(), text_properties->content().fo_font_size_);
	}
	if (font->m_oFamily.IsInit())
	{
	}

	if (font->m_oRFont.IsInit())
	{
		text_properties->content().style_font_name_ = string2std_string(font->m_oRFont->m_sVal.get());
	}
	/////
	//...
	/////
}
void XlsxConverter::convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size)
{
	 _CP_OPT(odf::length) odf_length;

	 convert(oox_font_size, odf_length);
	 
	 if (odf_length)
		 odf_font_size = odf::font_size(odf_length.get());
}
void XlsxConverter::convert(double oox_size,  _CP_OPT(odf::length) & odf_size)
{
	//����� ������� �������������� ����� oox_size
	//???
	odf_size = odf::length(oox_size, odf::length::pt);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFill * fill, odf::office_element_ptr  & odf_style_)
{
	if (fill == NULL)return;
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;

	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();//���� �������� ��� - ����������

 	if (cell_properties == NULL)return;

	if (fill->m_oGradientFill.IsInit())
	{
	}
	if (fill->m_oPatternFill.IsInit())
	{//solid, gradient
		if (fill->m_oPatternFill->m_oPatternType.IsInit())
		{}
		
		if (fill->m_oPatternFill->m_oFgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oFgColor.GetPointer(), 
				cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_);
		}
		else if (fill->m_oPatternFill->m_oBgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oBgColor.GetPointer(), 
				cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_);
		}	
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::background_color) & odf_bckgrd_color)
{
	if (!color)return;

	_CP_OPT(odf::color) odf_color;
	convert(color, odf_color);

	if (odf_color)odf_bckgrd_color = odf::background_color(odf_color.get());
}
void XlsxConverter::convert(OOX::Spreadsheet::CNumFmt *numFmt, odf::office_element_ptr  & odf_style_)
{
	if (!numFmt)return;


}
void XlsxConverter::convert(OOX::Spreadsheet::CBorder *border, odf::office_element_ptr  & odf_style_)
{
	if (!border)return;

}

void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color)
{
	if (!color)return;

	if(color->m_oRgb.IsInit())//easy, faster,realy  !!
	{
		odf_color = odf::color(std::wstring(L"#") + string2std_string(color->m_oRgb->ToString()));
		return;
	}

	SimpleTypes::Spreadsheet::CHexColor *oRgbColor=NULL;
	if(color->m_oThemeColor.IsInit())
	{
		OOX::CTheme * xlsx_theme= xlsx_document->GetTheme();

		int theme_ind = color->m_oThemeColor->GetValue();
		//CString strColor = color->m_oThemeColor->ToString();
		unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
		bool res=false;

		//???
		switch(theme_ind)//� ��� ��� CColorMapping �� ������ !!!
		{
			case SimpleTypes::themecolorLight1:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oLt1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorLight2:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oLt2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorDark1:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oDk1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorDark2:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oDk2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent1:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent2:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent3:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent3.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent4:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent4.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent5:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent5.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent6:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent6.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorFollowedHyperlink:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oFolHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorHyperlink:
				res=xlsx_theme->m_oThemeElements.m_oClrScheme.m_oHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
		}
		if (res)
			oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(ucR,ucG,ucB,ucA);
	}
	if(color->m_oIndexed.IsInit())
	{
		OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
		int ind = color->m_oIndexed->GetValue();

		if(xlsx_styles->m_oColors.IsInit() && xlsx_styles->m_oColors->m_oIndexedColors.IsInit())
		{
			if (xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind])
				oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb.get());
		}
		else
		{
			unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
			if(OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(ind, ucR, ucG, ucB, ucA))
				oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(ucR,ucG,ucB,ucA);
		}
	}
	if (oRgbColor)
	{
		CString str=oRgbColor->ToString();
		odf_color = odf::color(std::wstring(L"#") + string2std_string(str.Right(str.GetLength()-2)));
		delete oRgbColor;
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CCellStyle * cell_style, int oox_id)
{
	if (cell_style == NULL)return;
	
	bool automatic = false;
	bool root = true;

	std::wstring style_name = L"";
	if (cell_style->m_oName.IsInit())style_name = string2std_string(cell_style->m_oName.get());

	ods_context->styles_context().create_style(style_name,odf::style_family::TableCell, automatic, root, oox_id);

	odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
	if (!style)return;

	int parent_id = cell_style->m_oXfId.IsInit() ? cell_style->m_oXfId->GetValue() : -1;

	if (parent_id >=0) 
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(parent_id, odf::style_family::TableCell);
	
	//m_oBuiltinId, m_oCustomBuiltin, m_oHidden, m_oILevel;	???????
}
void XlsxConverter::convert(OOX::Spreadsheet::CXfs * xfc_style, int oox_id, bool automatic, bool root)
{
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();

	int id_parent	= xfc_style->m_oXfId.IsInit()		? xfc_style->m_oXfId->GetValue()	: -1; //parent 
	int fill_id		= xfc_style->m_oFillId.IsInit()	? xfc_style->m_oFillId->GetValue()		: -1;
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): -1;
	int font_id		= xfc_style->m_oFontId.IsInit()	? xfc_style->m_oFontId->GetValue()		: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context().create_style(L"",odf::style_family::TableCell, automatic, root, oox_id); 

	odf::office_element_ptr & elm_style = ods_context->styles_context().last_state().get_office_element();
	
	if (xlsx_styles->m_oFonts.IsInit())		convert(xlsx_styles->m_oFonts->m_arrItems[font_id], elm_style); //��������� ����� applyFont
	if (xlsx_styles->m_oFills.IsInit())		convert(xlsx_styles->m_oFills->m_arrItems[fill_id], elm_style); //��������� ����� applyFill
	if (xlsx_styles->m_oNumFmts.IsInit())	convert(xlsx_styles->m_oNumFmts->m_arrItems[numFmt_id], elm_style); 
	if (xlsx_styles->m_oBorders.IsInit())	convert(xlsx_styles->m_oBorders->m_arrItems[border_id], elm_style); 

	odf::style* style = dynamic_cast<odf::style*>(elm_style.get());
	if (!style)return;

	if (id_parent >=0)
	{
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(id_parent, odf::style_family::TableCell);
			
	}
	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();
	
		

}
} // namespace Docx2Odt