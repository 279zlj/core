#include "precompiled_cpodf.h"

#include "pptx_conversion_context.h"
#include "pptx_text_context.h"

#include <iostream>
#include <list>
#include <boost/foreach.hpp>

#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/xml/utils.h>

#include "../odf/odfcontext.h"
#include "../odf/style_text_properties.h"
#include "../odf/calcs_styles.h"


namespace cpdoccore {
namespace oox {

class pptx_text_context::Impl: boost::noncopyable
{
public:
	Impl(odf::odf_read_context & odf_context_,	pptx_conversion_context & pptx_context_);
public:
	styles_context & get_styles_context() { return styles_context_; }

	void add_text(const std::wstring & text);
    
    void start_paragraph(const std::wstring & styleName);
    void end_paragraph();

    void start_span(const std::wstring & styleName);
    void end_span();
    std::wstring end_span2();

	void start_object();
	std::wstring end_object();

	void start_base_style(const std::wstring baseStyleName, const odf::style_family::type baseStyleType);
	void end_base_style();

	void ApplyTextProperties(std::wstring style,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type);
	void ApplyParagraphProperties(std::wstring style,odf::paragraph_format_properties & propertiesOut, odf::style_family::type Type);
	void ApplyListProperties(odf::paragraph_format_properties & propertiesOut, int Level);

	void set_local_styles_container(odf::styles_container*  local_styles_);//��� ���� ����� �������� ���������� � ������ ���������

	void end_hyperlink(std::wstring hId);
	void start_hyperlink();

    void start_list(const std::wstring & StyleName, bool Continue = false);
    void end_list();
    void start_list_item(bool restart = false);
	void end_list_item();

	void start_field(field_type type, const std::wstring & styleName);
    void end_field();

	bool in_list_;

private:
    styles_context styles_context_;

	odf::odf_read_context & odf_context_ ;
	std::wstring hyperlink_hId;
	
	bool in_span;
	bool in_paragraph;

	odf::styles_container * local_styles_ptr_;

    void write_rPr(std::wostream & strm);
	void write_pPr(std::wostream & strm);
	void write_t(std::wostream & strm);
   
    std::wstring dump_paragraph();
	void dump_run();
	void dump_field();
  
	size_t paragraphs_cout_; //???? ���� �� �� ������ �������?
   
	std::wstringstream text_; //���������� �����
    std::wstringstream paragraph_; //���������� ����������� ���������
    std::wstringstream run_; //���������� ����������� ����� � ���� ����� ������� ����������
   
	std::wstring		paragraph_style_name_;
    std::wstring		span_style_name_;

	std::wstring			base_style_name_;
	odf::style_family::type base_style_family_;//Presentation Or SpreadSheet
/////////////lists////////////
    std::list<std::wstring> list_style_stack_;
    bool first_element_list_item_;
    // ������� ��� ��������� ���� ��������� � �������� ����������� ������
    size_t new_list_style_number_;
    // ������� �������������� ���������
    boost::unordered_map<std::wstring, std::wstring> list_style_renames_;
   
	void write_list_styles(std::wostream & strm);
	//void write_list_properties(std::wostream & strm);
	std::wstring find_list_rename(const std::wstring & ListStyleName);
	std::wstring current_list_style();
///////////////////////////

	field_type field_type_;
	std::wstringstream field_value_;

	pptx_conversion_context & pptx_context_;

};

pptx_text_context::Impl::Impl(odf::odf_read_context & odf_contxt_, pptx_conversion_context & pptx_contxt_): 
		odf_context_(odf_contxt_),	pptx_context_(pptx_contxt_),
		paragraphs_cout_(0),in_paragraph(false),in_span(false),field_type_(none)
{
	new_list_style_number_=0;
	local_styles_ptr_ = NULL;
}

void pptx_text_context::Impl::add_text(const std::wstring & text)
{
	if (field_type_)
		field_value_ << text;
	else
		text_ << text;
}

void pptx_text_context::Impl::set_local_styles_container(odf::styles_container * local_styles_)
{
	local_styles_ptr_= local_styles_;
}

void pptx_text_context::Impl::start_paragraph(const std::wstring & styleName)
{
    if (paragraphs_cout_++ > 0)
    {	
		if (paragraph_style_name_ != styleName)
		{
			dump_paragraph();
		}
		// ����� ����������� ������ � ������ ����������
		//text_ << L"&#10;";
        //text_ << L"\n"; 
	}else
	{
		text_.str(std::wstring());
		field_value_.str(std::wstring());
	}
	paragraph_style_name_ = styleName;
	in_paragraph = true;
}

void pptx_text_context::Impl::end_paragraph()
{
	in_paragraph = false;
}

void pptx_text_context::Impl::start_span(const std::wstring & styleName)//����� ������ � ������(���������) �� ������ ���������� - ���� ����� ���� � 1 �����
{
	if ((span_style_name_ !=styleName  && span_style_name_.length()>0) || in_span)
	{
		dump_run();
	}

	span_style_name_ = styleName;

	in_span=true;
}

void pptx_text_context::Impl::end_span() 
{
    dump_run();
    span_style_name_ = L"";
	
	in_span = false;
}

std::wstring pptx_text_context::Impl::end_span2()
{
    dump_run();
    span_style_name_ = L"";
	
	in_span = false;
    return run_.str();
}
void pptx_text_context::Impl::start_hyperlink()
{
	dump_run();//���������
}

void pptx_text_context::Impl::end_hyperlink(std::wstring hId)
{
	hyperlink_hId = hId;
	dump_run();
	hyperlink_hId = L"";
}
void pptx_text_context::Impl::ApplyTextProperties(std::wstring style_name,odf::text_format_properties_content & propertiesOut, odf::style_family::type Type)
{
	std::vector<const odf::style_instance *> instances;

	odf::style_instance* defaultStyle = NULL;
	odf::style_instance* style = NULL;
	odf::style_instance* baseStyle = NULL;
	
	if (local_styles_ptr_)
	{
		style = local_styles_ptr_->style_by_name(style_name, Type,false/*process_headers_footers_*/);
		defaultStyle = local_styles_ptr_->style_default_by_type(Type);
		baseStyle = local_styles_ptr_->style_by_name(base_style_name_, base_style_family_,false/*process_headers_footers_*/);
	}
	else
	{
		style = odf_context_.styleContainer().style_by_name(style_name, Type,false/*process_headers_footers_*/);
		defaultStyle = odf_context_.styleContainer().style_default_by_type(Type);
		baseStyle = odf_context_.styleContainer().style_by_name(base_style_name_, base_style_family_,false/*process_headers_footers_*/);
	}
	if	(defaultStyle)	instances.push_back(defaultStyle);
	if	(baseStyle)		instances.push_back(baseStyle);
	if	(style)			instances.push_back(style);
	
	if (style)get_styles_context().start_process_style(style);
	else get_styles_context().start_process_style(baseStyle);

	propertiesOut.apply_from(calc_text_properties_content(instances));
}

void pptx_text_context::Impl::ApplyListProperties(odf::paragraph_format_properties & propertiesOut, int Level)
{
	if (Level <0)return;
	if (list_style_stack_.empty())return;
	
	odf::style_list_level_properties	*list_properties= NULL;

	odf::text_list_style * text_list_style = odf_context_.listStyleContainer().list_style_by_name(list_style_stack_.back());
	
	if (text_list_style)
	{
		odf::office_element_ptr  elm = text_list_style->get_content()[Level];
		odf::office_element_ptr  elm_list;

		if (elm->get_type() == odf::typeTextListLevelStyleBullet)
		{
			odf::text_list_level_style_bullet* list_bullet = dynamic_cast<odf::text_list_level_style_bullet *>(elm.get());
			if (list_bullet)elm_list = list_bullet->style_list_level_properties_;
		}
		if (elm->get_type() == odf::typeTextListLevelStyleNumber)
		{
			odf::text_list_level_style_number* list_number = dynamic_cast<odf::text_list_level_style_number *>(elm.get());
			if (list_number)elm_list = list_number->style_list_level_properties_;
		}
	////////////////////
		if (elm_list)
		{
			list_properties = dynamic_cast<odf::style_list_level_properties	*>(elm_list.get());
		}
		
		elm->pptx_convert(pptx_context_);
	}
	if (list_properties)
	{
		propertiesOut.fo_text_indent_ = list_properties->text_min_label_width_;
		if (list_properties->text_space_before_)
		{
			double spaceBeforeTwip = list_properties->text_space_before_->get_value_unit(odf::length::pt);
			if (list_properties->text_min_label_width_)
			{
				spaceBeforeTwip += list_properties->text_min_label_width_->get_value_unit(odf::length::pt);
			}
			if (spaceBeforeTwip>0)
				propertiesOut.fo_margin_left_ = odf::length(spaceBeforeTwip,odf::length::pt);
		}
	}
	
}

void pptx_text_context::Impl::ApplyParagraphProperties(std::wstring style_name,odf::paragraph_format_properties & propertiesOut, odf::style_family::type Type)
{
	std::vector<const odf::style_instance *> instances;

	odf::style_instance* style = NULL;
	odf::style_instance * defaultStyle = NULL;
	odf::style_instance * baseStyle = NULL;
	
	if (local_styles_ptr_)
	{
		style = local_styles_ptr_->style_by_name(style_name, Type,false/*process_headers_footers_*/);
		defaultStyle = local_styles_ptr_->style_default_by_type(Type);
		baseStyle = local_styles_ptr_->style_by_name(base_style_name_, base_style_family_,false/*process_headers_footers_*/);
	}
	else
	{
		style = odf_context_.styleContainer().style_by_name(style_name, Type,false/*process_headers_footers_*/);
		defaultStyle = odf_context_.styleContainer().style_default_by_type(Type);
		baseStyle= odf_context_.styleContainer().style_by_name(base_style_name_, base_style_family_,false/*process_headers_footers_*/);
	}

	if (defaultStyle)	instances.push_back(defaultStyle);
	if (baseStyle)		instances.push_back(baseStyle);
	if (style)			instances.push_back(style);

	if (style)get_styles_context().start_process_style(style);
	else get_styles_context().start_process_style(baseStyle);

	propertiesOut.apply_from(calc_paragraph_properties_content(instances));
}

void pptx_text_context::Impl::write_pPr(std::wostream & strm)
{
	get_styles_context().start();

	int level = list_style_stack_.size()-1;		

	odf::paragraph_format_properties		paragraph_properties_;
	ApplyParagraphProperties (paragraph_style_name_,	paragraph_properties_,odf::style_family::Paragraph);
	
	ApplyListProperties (paragraph_properties_,level);//������������ ����� ������� �� �������� ���������

	paragraph_properties_.pptx_convert(pptx_context_);	
	
	const std::wstring & paragraphAttr  =  get_styles_context().paragraph_attr().str();	
	const std::wstring & paragraphNodes = get_styles_context().paragraph_nodes().str();

	if (level < 0 && paragraphAttr.length() <1 && paragraphNodes.length()<1) return;
	
	strm << L"<a:pPr ";

		if (level>=0)
		{
			strm << L"lvl=\"" << level << L"\" ";
		}

		strm << paragraphAttr;

	strm << ">";
		strm << paragraphNodes;

		if (level >=0 )
		 {
			

			strm << get_styles_context().list_style().str();
		}
	strm << L"</a:pPr>";
}

void pptx_text_context::Impl::write_t(std::wostream & strm)
{
}

void pptx_text_context::Impl::write_rPr(std::wostream & strm)
{
	if (paragraph_style_name_.length()<1 && span_style_name_.length()<1 && !(hyperlink_hId.length()>0)  && base_style_name_.length()<1)return;

	odf::text_format_properties_content			text_properties_paragraph_;
	ApplyTextProperties	(paragraph_style_name_,	text_properties_paragraph_,odf::style_family::Paragraph);
	
	odf::text_format_properties_content		text_properties_span_;
	ApplyTextProperties(span_style_name_,	text_properties_span_,odf::style_family::Text);

	odf::text_format_properties_content text_properties_;

	text_properties_.apply_from(text_properties_paragraph_);
	text_properties_.apply_from(text_properties_span_);

	get_styles_context().start();

	get_styles_context().hlinkClick() = hyperlink_hId;
	text_properties_.pptx_convert(pptx_context_);

	strm << get_styles_context().text_style().str();

}
std::wstring pptx_text_context::Impl::dump_paragraph(/*bool last*/)
{				
	dump_run();//last

	std::wstring & str_run = run_.str();

	if (str_run.length() > 0 || paragraph_style_name_.length() > 0)
	{
		CP_XML_WRITER(paragraph_)
		{
			CP_XML_NODE(L"a:p")
			{
				write_pPr(CP_XML_STREAM());

				CP_XML_STREAM() << run_.str();

				//if (last)
				//{
				//	CP_XML_NODE(L"a:endParaRPr");//- ����������/�� ���������� ����� �� ����������� ����� ������
				//}
			}
		}
		run_.str(std::wstring());
	}
	return paragraph_.str();
}
void pptx_text_context::Impl::dump_field()
{
	if (field_type_ == none) return;
	
	CP_XML_WRITER(run_)
    {
		CP_XML_NODE(L"a:fld")
		{
			switch (field_type_)
			{
			case page_number: 
				{
					CP_XML_ATTR(L"type", L"slidenum");
					CP_XML_ATTR(L"id", L"{5CC2A059-B141-45A7-B910-B096D6D06820}");
				}
				break;
			case date:
				{
					CP_XML_ATTR(L"type", L"datetime1");
					CP_XML_ATTR(L"id", L"{1D1B89AE-8D35-4BB5-B492-6D9BE4F23A39}");
				}							
				break;
			case time:	
				{
					CP_XML_ATTR(L"type", L"datetime11");
					CP_XML_ATTR(L"id", L"{03DA74A9-E3F2-4F30-AAF9-CC1A83980D5E}");
				}
				break;
			}
			//CP_XML_NODE(L"a:r")
			{
				const std::wstring content = xml::utils::replace_text_to_xml(field_value_.str());

				//write_rPr(CP_XML_STREAM());   

				CP_XML_NODE(L"a:t")
				{
				//	CP_XML_ATTR(L"xml:space", L"preserve"); 
					CP_XML_STREAM() << content;
				}
			}
		}
	}
	field_value_.str(std::wstring());
	field_type_ = none;
}
void pptx_text_context::Impl::dump_run()
{
	dump_field();
	
	const std::wstring content = xml::utils::replace_text_to_xml(text_.str());
	if (content.length() <1 &&  span_style_name_.length()<1) return ;     

	CP_XML_WRITER(run_)
    {

        if (content.length()>0)
        {			
			CP_XML_NODE(L"a:r")
			{
				write_rPr(CP_XML_STREAM());   

				CP_XML_NODE(L"a:t")
				{
				//	CP_XML_ATTR(L"xml:space", L"preserve"); 
					CP_XML_STREAM() << content;
				}
	         }
            text_.str(std::wstring());			
        }
    }
	hyperlink_hId =L"";
}

void pptx_text_context::Impl::start_base_style(const std::wstring baseStyleName, const odf::style_family::type baseStyleType)
{
	base_style_name_ = baseStyleName;
	base_style_family_ = baseStyleType;
}

void pptx_text_context::Impl::end_base_style()
{
	base_style_name_ = L"";
	base_style_family_ = odf::style_family::None;
}

void pptx_text_context::Impl::start_object()
{
    paragraphs_cout_ = 0;
   
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
	field_value_.str(std::wstring());
    
	paragraph_style_name_ = L"";
	span_style_name_ = L"";

}

std::wstring pptx_text_context::Impl::end_object()
{
	dump_paragraph(/*true*/);

	std::wstring & str_paragraph = paragraph_.str();

	std::wstringstream str_styles;

	if (str_paragraph.length() > 0)
	{
		write_list_styles(str_styles);
		str_styles << str_paragraph;
	}
  
	paragraphs_cout_ = 0;
    
	run_.str(std::wstring());
	paragraph_.str(std::wstring());
    text_.str(std::wstring());
	field_value_.str(std::wstring());
	
	paragraph_style_name_ = L"";
    span_style_name_=L"";

	return str_styles.str();
}
void pptx_text_context::Impl::start_list_item(bool restart)
{
    if (paragraphs_cout_ > 0 && restart)
    {	
		dump_paragraph();
	}

	in_list_ = true;
    first_element_list_item_ = true;

    if (restart && !list_style_stack_.empty())
    {
        const std::wstring curStyleName = current_list_style();
        const std::wstring newStyleName = curStyleName + boost::lexical_cast<std::wstring>(new_list_style_number_++);
        list_style_renames_[curStyleName] = newStyleName;

        odf::list_style_container & lists = odf_context_.listStyleContainer();
        odf::text_list_style * curStyle = lists.list_style_by_name(curStyleName);
        lists.add_list_style(curStyle, newStyleName);
        end_list();
        start_list(newStyleName);
    }
}

void pptx_text_context::Impl::start_list(const std::wstring & StyleName, bool Continue)
{
    if (paragraphs_cout_ > 0)
    {	
		dump_paragraph();
	}
	 
	if (!StyleName.empty())
    {
        std::wstring name = StyleName;
        if (Continue)
            name = find_list_rename(name);

        list_style_stack_.push_back(name);
    }
    else if (!list_style_stack_.empty())
        list_style_stack_.push_back(list_style_stack_.back());
    else
        list_style_stack_.push_back(L"");
}

void pptx_text_context::Impl::end_list()
{
	in_list_ = false;
  
	list_style_stack_.pop_back();// ���� �� ������� .. ��� �������� � lstStyles - �������
}

std::wstring pptx_text_context::Impl::current_list_style()
{
    if (!list_style_stack_.empty())
        return list_style_stack_.back();    
    else
        return L"";
}
std::wstring pptx_text_context::Impl::find_list_rename(const std::wstring & ListStyleName)
{
    std::wstring name = ListStyleName;
    while (list_style_renames_.count(name) > 0)
        name = list_style_renames_.at(name);

    return name;
}

void pptx_text_context::Impl::end_list_item()
{
	dump_paragraph();
	paragraphs_cout_--;
	paragraph_style_name_ = L"";

	in_list_ = false;
}

void pptx_text_context::Impl::start_field(field_type type, const std::wstring & styleName)
{
	field_type_ = type;
}

void pptx_text_context::Impl::end_field()
{
	dump_run();
	field_type_ = none;
}

void pptx_text_context::Impl::write_list_styles(std::wostream & strm)//defaults style paragraph & lists
{
    odf::list_style_container & list_styles = odf_context_.listStyleContainer();

    if (list_styles.empty())
        return;
 
	CP_XML_WRITER(strm)
	{ 	
		CP_XML_NODE(L"a:lstStyle");
		//{
		//	//defPPr
		//	//...

		//	//list levels 0 - 8
		//	BOOST_FOREACH(std::wstring & st_name, list_style_stack_ )
		//	{
		//		odf::text_list_style * s = list_styles.list_style_by_name(st_name);
		//        
		//		BOOST_FOREACH(odf::office_element_ptr & elm, s->get_content())
		//		{
		//			elm->pptx_convert(pptx_context_);
		//		}
		//	}

		//	CP_XML_STREAM() << get_styles_context().list_style().str();
		//}
	}

	list_style_stack_.clear();
}


///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

pptx_text_context::pptx_text_context(odf::odf_read_context & odf_context_, pptx_conversion_context & pptx_context_):
	impl_(new pptx_text_context::Impl(odf_context_,pptx_context_))
{}


pptx_text_context::~pptx_text_context()
{
}
void pptx_text_context::set_local_styles_container(odf::styles_container*  local_styles_)
{
	return impl_->set_local_styles_container(local_styles_);
}

void pptx_text_context::add_text(const std::wstring & text)
{
    return impl_->add_text(text);
}
bool pptx_text_context::in_list()
{
    return impl_->in_list_;
}
void pptx_text_context::start_paragraph(const std::wstring & styleName)
{
    return impl_->start_paragraph(styleName);
}

void pptx_text_context::end_paragraph()
{
    return impl_->end_paragraph();
}

void pptx_text_context::start_span(const std::wstring & styleName)
{
    return impl_->start_span(styleName);
}

void pptx_text_context::end_span()
{
    return impl_->end_span();
}

std::wstring pptx_text_context::end_span2()
{
    return impl_->end_span2();
}

void pptx_text_context::start_list_item(bool restart)
{
	return impl_->start_list_item(restart);
}
void pptx_text_context::end_list_item()
{
	return impl_->end_list_item();
}
void pptx_text_context::start_list(const std::wstring & StyleName, bool Continue)
{
	return impl_->start_list(StyleName,Continue);
}
void pptx_text_context::end_list()
{
	return impl_->end_list();
}
void pptx_text_context::start_base_style(const std::wstring baseStyleName, const odf::style_family::type baseStyleType)
{
	return impl_->start_base_style(baseStyleName, baseStyleType);
}

void pptx_text_context::end_base_style()
{
	return impl_->end_base_style();
}

void pptx_text_context::start_object()
{
	return impl_->start_object();
}
void pptx_text_context::start_hyperlink()
{
	return impl_->start_hyperlink();
}
void pptx_text_context::end_hyperlink(std::wstring hId)
{
	return impl_->end_hyperlink(hId);
}
std::wstring pptx_text_context::end_object()
{
	return impl_->end_object();
}
styles_context & pptx_text_context::get_styles_context() 
{ 
	return  impl_->get_styles_context() ; 
}
void pptx_text_context::start_field(field_type type, const std::wstring & styleName)
{
	impl_->start_field(type, styleName);
}
void pptx_text_context::end_field()
{
	impl_->end_field();
}

}
}
