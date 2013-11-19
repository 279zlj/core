#include "precompiled_cpodf.h"
#include "styles_list.h"

#include <cpdoccore/xml/serialize.h>
#include "serialize_elements.h"
#include "style_text_properties.h"

namespace cpdoccore { 
namespace odf {
    
using xml::xml_char_wc;

//  text_list_style_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_style_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:display-name", style_display_name_);
    CP_APPLY_ATTR(L"text:consecutive-numbering", text_consecutive_numbering_);
}

// text:list-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_style::ns = L"text";
const wchar_t * text_list_style::name = L"list-style";

void text_list_style::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_style_attr_.add_attributes(Attributes);
}

void text_list_style::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"text" == Ns && L"list-level-style-number" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);
    else if (L"text" == Ns && L"list-level-style-bullet" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

//  text_list_level_style_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:level", text_level_, (unsigned int)0);
}

//  text_list_level_style_number_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_number_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:style-name", text_style_name_);  
    common_num_format_attlist_.add_attributes(Attributes);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:display-levels", text_display_levels_, (unsigned int)1);    
    CP_APPLY_ATTR(L"text:start-value", text_start_value_, (unsigned int)1);    
}

//  text_list_level_style_bullet_attr
//////////////////////////////////////////////////////////////////////////////////////////////////

void text_list_level_style_bullet_attr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:style-name", text_style_name_);
    CP_APPLY_ATTR(L"text:bullet-char", text_bullet_char_);
    common_num_format_prefix_suffix_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:bullet-relative-size", text_bullet_relative_size_);
}

// text:list-level-style-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_number::ns = L"text";
const wchar_t * text_list_level_style_number::name = L"list-level-style-number";

void text_list_level_style_number::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_level_style_attr_.add_attributes(Attributes);
    text_list_level_style_number_attr_.add_attributes(Attributes);
}

void text_list_level_style_number::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if		(L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);    
    else if (L"style" == Ns && L"style-text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
     else if (L"style" == Ns && L"text-properties"	== Name)
        CP_CREATE_ELEMENT(text_properties_); 
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_properties::ns = L"style";
const wchar_t * style_list_level_properties::name = L"list-level-properties";

void style_list_level_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_text_align_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"text:space-before", text_space_before_);
    CP_APPLY_ATTR(L"text:min-label-width", text_min_label_width_);
    CP_APPLY_ATTR(L"text:min-label-distance", text_min_label_distance_);
    CP_APPLY_ATTR(L"style:font-name", style_font_name_);
    CP_APPLY_ATTR(L"fo:width", fo_width_);
    CP_APPLY_ATTR(L"fo:height", fo_height_);

    common_vertical_rel_attlist_.add_attributes(Attributes);
    common_vertical_pos_attlist_.add_attributes(Attributes);
}

void style_list_level_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"list-level-label-alignment")
    {
        CP_CREATE_ELEMENT(style_list_level_label_alignment_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}

// style:list-level-label-alignment
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_label_alignment::ns = L"style";
const wchar_t * style_list_level_label_alignment::name = L"list-level-label-alignment";

void style_list_level_label_alignment::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:label-followed-by", text_label_followed_by_);
    CP_APPLY_ATTR(L"text:list-tab-stop-position", text_list_tab_stop_position_);
    CP_APPLY_ATTR(L"fo:text-indent", fo_text_indent_);
    CP_APPLY_ATTR(L"fo:margin-left", fo_margin_left_);

	if ((text_label_followed_by_) && (text_label_followed_by_.get() == L"listtab")) text_label_followed_by_ = L"tab";
}

void style_list_level_label_alignment::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

// text:list-level-style-bullet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_bullet::ns = L"text";
const wchar_t * text_list_level_style_bullet::name = L"list-level-style-bullet";

void text_list_level_style_bullet::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_list_level_style_attr_.add_attributes(Attributes);
    text_list_level_style_bullet_attr_.add_attributes(Attributes);
}

void text_list_level_style_bullet::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);
    else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

std::wstring GetNumFormat(const std::wstring & Format)
{
    if (L"1" == Format)
        return L"decimal";
    else if (L"i" == Format)
        return L"lowerRoman";
    else if (L"I" == Format)
        return L"upperRoman";
    else if (L"a" == Format)
        return L"lowerLetter";
    else if (L"A" == Format)
        return L"upperLetter";
    else
        return L"decimal";
}

std::wstring GetLevelText(unsigned int displayLevels,
                          unsigned int textLevel,
                          oox::docx_conversion_context & Context)
{
    std::wstring res;
    if (displayLevels > 1)
        res = GetLevelText(displayLevels - 1, textLevel - 1, Context);

    if (displayLevels > 1)
        res += L".";
    res += L"%";
    res += boost::lexical_cast<std::wstring>(textLevel);
    return res;
}

void write_label_alignment_props(std::wostream & strm, style_list_level_label_alignment * labelAlignment)
{
    // ODF 1.2
	int position = labelAlignment->get_text_list_tab_stop_position() ? labelAlignment->get_text_list_tab_stop_position()->get_value_unit(length::pt) : 0;

    if (position >0)
    {
        strm << L"<w:tabs>";
        strm << L"<w:tab w:pos=\"" << 
            (int)( 0.5 + 20.0 * position ) << "\" w:val=\"num\""
            << " />";
        strm << L"</w:tabs>";
    }

    int w_hanging = (int)( 0.5 - 20.0 * labelAlignment->get_fo_text_indent().get_value_or( length(0, length::pt) ).get_value_unit(length::pt) );
    int w_left = (int)( 0.5 + 20.0 * labelAlignment->get_fo_margin_left().get_value_or( length(0, length::pt) ).get_value_unit(length::pt) );

    strm << L"<w:ind w:left=\"" << w_left << "\" w:hanging=\"" << w_hanging << "\" />";                   
}

void write_level_justification(std::wostream & strm, style_list_level_properties * listLevelProperties)
{
    std::wstring w_lvlJc;

    const text_align textAlign = listLevelProperties ? 
        listLevelProperties->get_common_text_align().fo_text_align_.get_value_or( text_align(text_align::Left) ) : 
    text_align(text_align::Left);

    if (textAlign.get_type() == text_align::Right)
        w_lvlJc = L"right";
    else if (textAlign.get_type() == text_align::Center)
        w_lvlJc = L"center";
    else 
        w_lvlJc = L"left";

    if (!w_lvlJc.empty())
        strm << L"<w:lvlJc w:val=\"" << w_lvlJc << "\" />";   
}


}

void text_list_level_style_number::docx_convert(oox::docx_conversion_context & Context)
{
	if (text_list_level_style_attr_.get_text_level() - 1 >= 9)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( style_list_level_properties_.get() );
  
    style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;


    strm << L"<w:lvl w:ilvl=\"" << (text_list_level_style_attr_.get_text_level() - 1) << L"\" >";
    
    strm << L"<w:start w:val=\"" <<  text_list_level_style_number_attr_.text_start_value_ << L"\" />";
    strm << L"<w:numFmt w:val=\"" << GetNumFormat( text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ ) << "\" />";

	if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
	{
		strm << L"<w:suff w:val=\"" << labelAlignment->text_label_followed_by_.get() << "\" />";
	}else 
		strm << L"<w:suff w:val=\"tab\" />";

    // w:lvlText
    {
        std::wstring w_lvlText;
        w_lvlText += text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_prefix_.get_value_or(L"");

        // 
        const unsigned int displayLevels = text_list_level_style_number_attr_.text_display_levels_;
        const unsigned int textLevel = text_list_level_style_attr_.get_text_level();
    
        w_lvlText += GetLevelText(displayLevels, textLevel, Context);

        w_lvlText += text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_suffix_.get_value_or(L"");

        if (!w_lvlText.empty())
            strm << L"<w:lvlText w:val=\"" << w_lvlText << "\" />";
    }
    
    // w:lvlJc
    write_level_justification(strm, listLevelProperties);

    double spaceBeforeTwip = 0.0;
    if (listLevelProperties && listLevelProperties->text_space_before_)
    {
        spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
    }
    
    double minLabelWidthTwip = 0.0;
    if (listLevelProperties && listLevelProperties->text_min_label_width_)
    {
        minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
    }

    double minLabelDistanceTwip = 0.0;
    if (listLevelProperties && 
        !text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_.empty() &&
        listLevelProperties->text_min_label_distance_)
    {
        minLabelDistanceTwip = 20.0 * listLevelProperties->text_min_label_distance_->get_value_unit(length::pt);
    }
    {
        strm << L"<w:pPr>";
        
        if (labelAlignment)
        {
            write_label_alignment_props(strm, labelAlignment);                                 
        }
        else
        {    
            strm << L"<w:ind";
            strm << L" w:left=\"" << ((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)) << "\"";
            
            if (spaceBeforeTwip < 0.0)
            {
                strm << L" w:firstLine=\"" << ((int)(minLabelWidthTwip + 0.5)) << "\"";
            }
            else
            {
                double hanging = 0.0;
                if ( (int)minLabelWidthTwip == 0)
                {
                    if (spaceBeforeTwip < 0)
                        hanging = spaceBeforeTwip;
                    else
                        hanging = -spaceBeforeTwip;
                }
                else
                {
                    hanging = minLabelWidthTwip;
                }

                strm << L" w:hanging=\"" <<  ((int)( hanging  + 0.5)) << "\"";
            }
            strm << L" />";
        }
    
        strm << L"</w:pPr>";
    }

    if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
    {
        Context.get_styles_context().start();
        textProperties->content().docx_convert(Context);
        Context.get_styles_context().write_text_style( Context );
    }

    strm << L"</w:lvl>";
}

namespace 
{

wchar_t convert_bullet_char(wchar_t c)
{
    //return c;
    wchar_t res = c;
    switch(c)
    {
    case L'\xE00C': res = L'\xF075';/*L'\xF0A8'*/; break; 
    case L'\xE00A': res = L'\xF0A7'; break; 
    case L'\x2611': res = L'\xF0FE'; break; 
    case L'\x2022': res = L'\x2022'; break; 
    case L'\x25CF': res = L'\xF0B7'; break; 
    case L'\x27A2': res = L'\xF0D8'; break; 
    case L'\x2714': res = L'\xF0FC'; break; 
    case L'\x25A0': res = L'\xF0A7'; break; 
    case L'\x25CB': res = L'\x006F'; break; 
    case L'\x2794': res = L'\xF0DA'; break; 
    case L'\x2717': res = L'\xF04F'; break; 
    case L'\x2013': res = L'\x2013'; break;
    default:break;
    }
    return res;
}

}

void text_list_level_style_bullet::docx_convert(oox::docx_conversion_context & Context) 
{    
	if (text_list_level_style_attr_.get_text_level() - 1 >= 9)
        return;

    std::wostream & strm = Context.output_stream();
    style_list_level_properties * listLevelProperties = dynamic_cast<style_list_level_properties *>( style_list_level_properties_.get() );
    
	style_list_level_label_alignment * labelAlignment = listLevelProperties ?
        dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;

////////////////////////////////////////////////////
	strm << L"<w:lvl w:ilvl=\"" << (text_list_level_style_attr_.get_text_level() - 1) << L"\" >";
    strm << L"<w:numFmt w:val=\"bullet\" />";
   
	if ((labelAlignment) && (labelAlignment->text_label_followed_by_))
	{
		strm << L"<w:suff w:val=\"" << labelAlignment->text_label_followed_by_.get() << "\" />";
	}else 
		strm << L"<w:suff w:val=\"tab\" />";


    const wchar_t bullet = text_list_level_style_bullet_attr_.text_bullet_char_.get_value_or(L'\x2022');
    strm << L"<w:lvlText w:val=\"" << convert_bullet_char(bullet) << "\"/>";

    // w:lvlJc
    write_level_justification(strm, listLevelProperties);


    //strm << L"<w:pPr>";
    //style_list_level_label_alignment * labelAlignment = listLevelProperties ?
    //    dynamic_cast<style_list_level_label_alignment *>(listLevelProperties->style_list_level_label_alignment_.get()) : NULL;
    //if (labelAlignment)
    //{
    //    write_label_alignment_props(strm, labelAlignment);                                 
    //}
    //strm << L"</w:pPr>";
   


    double spaceBeforeTwip = 0.0;
    if (listLevelProperties && listLevelProperties->text_space_before_)
    {
        spaceBeforeTwip = 20.0 * listLevelProperties->text_space_before_->get_value_unit(length::pt);
    }
    
    double minLabelWidthTwip = 0.0;
    if (listLevelProperties && listLevelProperties->text_min_label_width_)
    {
        minLabelWidthTwip = 20.0 * listLevelProperties->text_min_label_width_->get_value_unit(length::pt);
    }

    double minLabelDistanceTwip = 0.0;

    {
		strm << L"<w:pPr>";
        

        if (labelAlignment)
        {
            write_label_alignment_props(strm, labelAlignment);                                 
        }
        else
        {    
            strm << L"<w:ind";
            strm << L" w:left=\"" << ((int)(minLabelWidthTwip + spaceBeforeTwip + 0.5)) << "\"";
            
            if (spaceBeforeTwip < 0.0)
            {
                strm << L" w:firstLine=\"" << ((int)(minLabelWidthTwip + 0.5)) << "\"";
            }
            else
            {
                double hanging = 0.0;
                if ( (int)minLabelWidthTwip == 0)
                {
                    if (spaceBeforeTwip < 0)
                        hanging = spaceBeforeTwip;
                    else
                        hanging = -spaceBeforeTwip;
                }
                else
                {
                    hanging = minLabelWidthTwip;
                }

                strm << L" w:hanging=\"" <<  ((int)( hanging  + 0.5)) << "\"";
            }
            strm << L" />";
        }
    
        strm << L"</w:pPr>";
    }

    if (style_text_properties * textProperties = dynamic_cast<style_text_properties *>(style_text_properties_.get()))
    {
        Context.get_styles_context().start();
        textProperties->content().docx_convert(Context);
        Context.get_styles_context().write_text_style( Context );
    }
    
    strm << L"</w:lvl>";
}

}
}
