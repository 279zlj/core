#include "precompiled_cpodf.h"
#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/common/boost_filesystem_version.h>
#include <cpdoccore/odf/odf_document.h>

#include "mediaitems_utils.h"

#include "pptx_drawing.h"
#include "pptx_drawings.h"
#include "pptx_slide_context.h"

#include "../odf/length.h"

#include "../odf/draw_common.h"


namespace cpdoccore {
namespace oox {

class pptx_drawings;
typedef _CP_PTR(pptx_drawings) pptx_drawings_ptr;

struct _rect
{
    double width_;
    double height_;
    double x_;
    double y_;
};

//+ 3-� ������ ������

struct simple_drawing_desc
{
	std::wstring draw_name_;
	
	_CP_OPT(_rect) svg_rect_;

	std::wstring anchor_;
    double anchor_x_;
    double anchor_y_;

    std::wstring xlink_href_;  //������ �� ������ (external)
   
	std::vector<_hlink_desc> hlinks_;

	std::vector<odf::_property> additional_;//for shapes
	
	std::wstring clipping_string_;

	int type_;	//default - frame
};



class pptx_slide_context::Impl
{
public:    
	Impl(const std::wstring & odfPacket) : pptx_drawings_(pptx_drawings::create()), mediaitems_(odfPacket),rId_(1),odfPacket_(odfPacket)
    {} 

    simple_drawing_desc simple_drawing_desc_;    

	std::vector<simple_drawing_desc> images_;
	std::vector<simple_drawing_desc> charts_;
	std::vector<simple_drawing_desc> shapes_;

    void add_drawing(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
    {
        pptx_drawings_->add(d, isInternal, rid, ref, type);
    }
  
	mediaitems & get_mediaitems() { return mediaitems_; }

    bool empty() const
    {
        return pptx_drawings_->empty();
    }

	void clear()
	{
		images_.clear();
		charts_.clear();
		shapes_.clear();
		rId_ = 1;

		pptx_drawings_ = pptx_drawings::create();
	}

    size_t next_rId()
    {
		return rId_++;;
    }

    pptx_drawings_ptr get_drawings()
    {
        return pptx_drawings_;
    }
	std::wstring  odfPacket_;
private:
	size_t rId_;
	mediaitems mediaitems_;
    pptx_drawings_ptr pptx_drawings_;
};



pptx_slide_context::pptx_slide_context(pptx_conversion_context & Context)
 : impl_(new pptx_slide_context::Impl(Context.root()->get_folder()))
{    
	hlinks_size_ = 0;
}
void pptx_slide_context::start_slide()
{    
	impl_->clear();
	hlinks_size_ = 0;
	default_set();
}
void pptx_slide_context::default_set()
{
    impl_->simple_drawing_desc_.xlink_href_ = L"";

    impl_->simple_drawing_desc_.draw_name_ = L"";
  
	impl_->simple_drawing_desc_.additional_.clear();
	impl_->simple_drawing_desc_.anchor_ =L"";
	
	impl_->simple_drawing_desc_.clipping_string_= L"";
	impl_->simple_drawing_desc_.svg_rect_	= boost::none;

	impl_->simple_drawing_desc_.hlinks_.clear();
	impl_->simple_drawing_desc_.additional_.clear();
}


void pptx_slide_context::set_placeHolder_type(std::wstring typeHolder)
{
	impl_->simple_drawing_desc_.additional_.push_back(odf::_property(L"PlaceHolderType",typeHolder));
}

void pptx_slide_context::set_rect(double width_pt, double height_pt, double x_pt, double y_pt)
{
	_rect r = {width_pt,height_pt,x_pt,y_pt};
	impl_->simple_drawing_desc_.svg_rect_= r;
}
void pptx_slide_context::set_translate(double x_pt, double y_pt)
{
	if (impl_->simple_drawing_desc_.svg_rect_)
	{
		_rect r = impl_->simple_drawing_desc_.svg_rect_.get();
		r.x_+=x_pt;
		r.y_+=y_pt;

		impl_->simple_drawing_desc_.svg_rect_= r;
	}
}
void pptx_slide_context::set_scale(double cx_pt, double cy_pt)
{
	if (impl_->simple_drawing_desc_.svg_rect_)
	{
		_rect r = impl_->simple_drawing_desc_.svg_rect_.get();
		r.x_*=cx_pt;
		r.y_*=cy_pt;

		impl_->simple_drawing_desc_.svg_rect_= r;
	}
}
void pptx_slide_context::set_anchor(std::wstring anchor, double x_pt, double y_pt)
{
	impl_->simple_drawing_desc_.anchor_		= anchor;
	impl_->simple_drawing_desc_.anchor_x_	= x_pt;
    impl_->simple_drawing_desc_.anchor_y_	= y_pt;
}
void pptx_slide_context::set_property(odf::_property p)
{
	impl_->simple_drawing_desc_.additional_.push_back(p);
}
std::vector<odf::_property> & pptx_slide_context::get_properties()
{
	return impl_->simple_drawing_desc_.additional_;
}
void pptx_slide_context::set_clipping(std::wstring & str)
{
	impl_->simple_drawing_desc_.clipping_string_= str;
}
std::wstring pptx_slide_context::add_hyperlink(std::wstring const & ref,bool object)
{
	++hlinks_size_;
	std::wstring hId=std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_size_);
	
	_hlink_desc desc={hId, ref, object};
	impl_->simple_drawing_desc_.hlinks_.push_back(desc);

	return hId;
}
void pptx_slide_context::set_name(std::wstring const & name)
{
	impl_->simple_drawing_desc_.draw_name_ = name;

}
void pptx_slide_context::start_shape(int type)
{
	impl_->simple_drawing_desc_.type_ = type; //2,3... 
}
void pptx_slide_context::start_image(std::wstring const & path)
{
	impl_->simple_drawing_desc_.xlink_href_ = path; 
	impl_->simple_drawing_desc_.type_ = 0; //frame 
}
void pptx_slide_context::start_chart(std::wstring const & path)
{
	impl_->simple_drawing_desc_.xlink_href_ = path; 
	impl_->simple_drawing_desc_.type_ = 0; //frame 
}
void pptx_slide_context::end_shape()
{
	impl_->shapes_.push_back(impl_->simple_drawing_desc_);
	default_set();
}
void pptx_slide_context::end_image()
{
    impl_->images_.push_back(impl_->simple_drawing_desc_);
	default_set();
}
void pptx_slide_context::end_chart()
{
    impl_->charts_.push_back(impl_->simple_drawing_desc_);
	default_set();
}
bool pptx_slide_context::empty() const
{
    return impl_->empty();
}
void pptx_slide_context::process_drawings()
{
	process_shapes();
	process_images();
	process_charts();
}
void pptx_slide_context::process_images()
{
	using boost::filesystem::wpath;
	int pos_replaicement=0;
	
    BOOST_FOREACH(simple_drawing_desc & pic, impl_->images_)
    {
		pos_replaicement= pic.xlink_href_.find(L"ObjectReplacements"); 
		if (pos_replaicement <0)//��������, � �� ���������� ������
		{
			_pptx_drawing drawing=_pptx_drawing();
			
			_CP_OPT(std::wstring) sTextContent;
			GetProperty(pic.additional_,L"text-content",sTextContent);
			//if (sTextContent)//� ms office �� �������� ������ ������� ������� - ������ ��� �� ���� � �������� ���������
			//{
			//	drawing.type = mediaitems::typeShape;
			//	drawing.sub_type = 2;//rect
			//	drawing.fillRef_inp = pic.xlink_href_; //������� ��������� == ����� �������� 
			//}
			//else
				drawing.type = mediaitems::typeImage;
			
			drawing.id = impl_->next_rId();
			
			drawing.name = pic.draw_name_;

			std::wstring ref;/// ��� ������ �� �������� ������� ������
			bool isMediaInternal = false;
			
			drawing.rId = impl_->get_mediaitems().add_or_find(pic.xlink_href_, drawing.type, isMediaInternal, ref);		
			//drawing.fillId  = impl_->get_mediaitems().add_or_find(drawing.fillRef_inp, mediaitems::typeImage, isMediaInternal, fillRef_out);

			process_common_properties(pic,drawing);

			std::wstring fileName = BOOST_STRING_PATH(wpath(impl_->odfPacket_) / pic.xlink_href_);			
			drawing.clipping_enabled  = odf::parse_clipping(pic.clipping_string_,fileName,drawing.clipping_rect);

			impl_->add_drawing(drawing, isMediaInternal, drawing.rId , ref, drawing.type);
			
		}
    }
}
void pptx_slide_context::process_charts()
{
    using boost::filesystem::wpath;

    BOOST_FOREACH(simple_drawing_desc & pic, impl_->charts_)
    {	
		_pptx_drawing drawing=_pptx_drawing();
		drawing.type = mediaitems::typeChart;
        drawing.name = pic.draw_name_;

		const size_t id = impl_->next_rId();

        std::wstring ref;
        bool isMediaInternal = true;
        drawing.rId = impl_->get_mediaitems().add_or_find(pic.xlink_href_, mediaitems::typeChart, isMediaInternal, ref);
		drawing.id = id;
        
		process_common_properties(pic,drawing);
////////////////////////////////////////////////////////////////
        impl_->add_drawing(drawing, isMediaInternal, drawing.rId, ref, mediaitems::typeChart);
    }
}

void pptx_slide_context::process_common_properties(simple_drawing_desc & pic,_pptx_drawing & drawing)
{
	if (pic.svg_rect_)
	{
		drawing.x = odf::length(pic.svg_rect_.get().x_, odf::length::pt).get_value_unit(odf::length::emu);
		drawing.y = odf::length(pic.svg_rect_.get().y_, odf::length::pt).get_value_unit(odf::length::emu);
		drawing.cx = odf::length(pic.svg_rect_.get().width_, odf::length::pt).get_value_unit(odf::length::emu);
		drawing.cy = odf::length(pic.svg_rect_.get().height_, odf::length::pt).get_value_unit(odf::length::emu);
	}
	
	drawing.additional=pic.additional_;

	drawing.hlinks=pic.hlinks_;
}
void pptx_slide_context::process_shapes()
{
    BOOST_FOREACH(simple_drawing_desc & pic, impl_->shapes_)
    {
		_pptx_drawing drawing=_pptx_drawing();

         std::wstring ref;
        bool isMediaInternal = true;

		drawing.type = mediaitems::typeShape;
		drawing.name = pic.draw_name_;

 		const size_t id = impl_->next_rId();
		
		drawing.rId = impl_->get_mediaitems().add_or_find(pic.xlink_href_, mediaitems::typeShape, isMediaInternal, ref);
		drawing.id = id;
       
		process_common_properties(pic,drawing);
////////////////////////////////////////////////////////////////
		_CP_OPT(std::wstring) sPlaceHolderType;
		GetProperty(pic.additional_,L"PlaceHolderType",sPlaceHolderType);
		if (sPlaceHolderType)drawing.place_holder_type_ = *sPlaceHolderType;

		drawing.sub_type = pic.type_;

		impl_->add_drawing(drawing, isMediaInternal, drawing.rId, ref, mediaitems::typeShape);
    }
}
void pptx_slide_context::dump_rels(rels & Rels)
{
	impl_->get_drawings()->dump_rels(Rels);
}
mediaitems & pptx_slide_context::get_mediaitems()
{
	return impl_->get_mediaitems(); 
}
void pptx_slide_context::serialize(std::wostream & strm)
{
	int next_id = impl_->next_rId();

// ��������� ������������ noGroup ���� ������� ����
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:nvGrpSpPr")
		{
			CP_XML_NODE(L"p:cNvPr")
			{
				CP_XML_ATTR(L"name",L"noGroup");
				CP_XML_ATTR(L"id",next_id);
			}
			CP_XML_NODE(L"p:cNvGrpSpPr");
			CP_XML_NODE(L"p:nvPr");	
		}
		CP_XML_NODE(L"p:grpSpPr")
		{
			CP_XML_NODE(L"a:xfrm")
			{
				CP_XML_NODE(L"a:off")	{CP_XML_ATTR(L"x",0); CP_XML_ATTR(L"y",0);}
				CP_XML_NODE(L"a:ext")	{CP_XML_ATTR(L"cx",0); CP_XML_ATTR(L"cy",0);}
				CP_XML_NODE(L"a:chOff")	{CP_XML_ATTR(L"x",0); CP_XML_ATTR(L"y",0);}
				CP_XML_NODE(L"a:chExt")	{CP_XML_ATTR(L"cx",0); CP_XML_ATTR(L"cy",0);}
			}
		}
	}
	process_drawings();
    impl_->get_drawings()->serialize(strm);   
}

}
}
