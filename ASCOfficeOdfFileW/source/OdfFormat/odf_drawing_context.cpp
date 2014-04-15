#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>

#include <iostream>

#include "odf_drawing_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_conversion_context.h"

#include "draw_frame.h"
#include "draw_shapes.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"

namespace cpdoccore 
{
namespace odf
{

struct 	odf_element_state
{
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;
	
	int level;
};

struct odf_drawing_state
{
	void clear()
	{
		elements_.clear();
		
		svg_x_= boost::none;
		svg_y_= boost::none;
		svg_height_= boost::none;
		svg_width_= boost::none;

		name_ = L"";
		z_order_ = -1;
		
		rotateAngle = boost::none;
		flipH = false;
		flipV = false;	
		tile = false;
	}
	std::vector<odf_element_state> elements_;

	_CP_OPT(length) svg_x_;
	_CP_OPT(length) svg_y_;
	_CP_OPT(length) svg_height_;
	_CP_OPT(length) svg_width_;	

	std::wstring name_;
	int z_order_;

	bool flipH;
	bool flipV;
	bool tile;
	_CP_OPT(double) rotateAngle;
};

class odf_drawing_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		current_drawing_state_.clear();
		styles_context_ = &odf_context_->styles_context();
	} 
	std::vector<odf_drawing_state> drawing_list_;//��� �������� .. ��� �������� ���������� �� "�����"
	
	odf_drawing_state current_drawing_state_;
	
	std::vector<office_element_ptr> current_level_;//��������� ���������� ������ ������� ������������

	odf_style_context * styles_context_;
	odf_conversion_context *odf_context_;
};

////////////////////////////////////////////////////////////////////////////

odf_drawing_context::odf_drawing_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_drawing_context::Impl(odf_context))
{
}

odf_drawing_context::~odf_drawing_context()
{
}

office_element_ptr & odf_drawing_context::get_current_element(){return impl_->current_level_.back();}

void odf_drawing_context::set_styles_context(odf_style_context*  styles_context)
{
	impl_->styles_context_ = styles_context;
}

void odf_drawing_context::start_frame()
{
	office_element_ptr frame_elm;
	create_element(L"draw", L"frame", frame_elm, impl_->odf_context_);
	
	draw_frame* frame = dynamic_cast<draw_frame*>(frame_elm.get());
	if (frame == NULL)return;
//////////////////////////////////////////////////////////////////////////////////////////////
	impl_->styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_frame_elm = impl_->styles_context_->last_state().get_office_element();
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_frame_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
	}
	frame->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_style_name_attlist_.draw_style_name_ = style_ref(style_name);
	
	frame->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
	frame->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;
	frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = impl_->current_drawing_state_.svg_height_;
	frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = impl_->current_drawing_state_.svg_width_;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = impl_->current_level_.size();
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(frame_elm);

	impl_->current_level_.push_back(frame_elm);

	odf_element_state state={frame_elm, style_name, style_frame_elm, level};

	impl_->current_drawing_state_.elements_.push_back(state);

}
void odf_drawing_context::start_drawing()//frame???
{
	impl_->current_drawing_state_.clear();
	//text_context_.clear();
}

void odf_drawing_context::end_frame()
{
	end_element();
}
void odf_drawing_context::end_drawing()//������ ����������.. ������ �������� ������� ���������
{
//��� �������� ��� �������� - ������� � �������...
	if (impl_->current_drawing_state_.elements_.size() > 0)
	{
		draw_frame* frame = dynamic_cast<draw_frame*>(impl_->current_drawing_state_.elements_[0].elm.get());
		if (frame)
		{
			if (impl_->current_drawing_state_.name_.length() > 0)
				frame->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_name_attlist_.draw_name_ = impl_->current_drawing_state_.name_;
			if (impl_->current_drawing_state_.z_order_ >= 0)
				frame->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_z_index_attlist_.draw_z_index_ = impl_->current_drawing_state_.z_order_;

			std::wstring strTransform;
			if (impl_->current_drawing_state_.rotateAngle)
			{
				strTransform = strTransform + std::wstring(L"rotate(") + boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.rotateAngle.get()) + std::wstring(L")");
			}
			if (strTransform.length()>0)
				frame->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_ = strTransform;

			frame->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
			frame->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;
			frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = impl_->current_drawing_state_.svg_height_;
			frame->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = impl_->current_drawing_state_.svg_width_;
		}
		draw_shape* shape = dynamic_cast<draw_shape*>(impl_->current_drawing_state_.elements_[0].elm.get());
		if (shape)
		{
			if (impl_->current_drawing_state_.name_.length() > 0)
				shape->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_name_attlist_.draw_name_ = impl_->current_drawing_state_.name_;
			if (impl_->current_drawing_state_.z_order_ >= 0)
				shape->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_z_index_attlist_.draw_z_index_ = impl_->current_drawing_state_.z_order_;
			
			std::wstring strTransform;
			if (impl_->current_drawing_state_.rotateAngle!=0)
			{
				strTransform = strTransform + std::wstring(L"rotate(") + boost::lexical_cast<std::wstring>(impl_->current_drawing_state_.rotateAngle.get()) + std::wstring(L")");
			}
			if (strTransform.length()>0)
				frame->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_ = strTransform;
			
			shape->common_draw_attlists_.position_.svg_x_ = impl_->current_drawing_state_.svg_x_;
			shape->common_draw_attlists_.position_.svg_y_ = impl_->current_drawing_state_.svg_y_;
			shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = impl_->current_drawing_state_.svg_height_;
			shape->common_draw_attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = impl_->current_drawing_state_.svg_width_;
		}
		
		style* style_ = dynamic_cast<style*>(impl_->current_drawing_state_.elements_[0].style_elm.get());
		if (style_)
		{
			style_graphic_properties * gr_properties = style_->style_content_.get_style_graphic_properties();
			if (gr_properties)
			{		
				if (impl_->current_drawing_state_.flipH)
					gr_properties->content().style_mirror_ = std::wstring(L"horizontal");
				if (impl_->current_drawing_state_.flipV)
					gr_properties->content().style_mirror_ = std::wstring(L"vertical");

				//fo:clip
				//draw:image-opacity
			}
		}
	}

	impl_->drawing_list_.push_back(impl_->current_drawing_state_);//��� ��� ���������� frame, shape, �����???
	impl_->current_drawing_state_.clear();
}
void odf_drawing_context::set_name(std::wstring  name)
{
	impl_->current_drawing_state_.name_ = name;
}
void odf_drawing_context::set_z_order(int id)
{
	impl_->current_drawing_state_.z_order_ = id;

}
void odf_drawing_context::set_flip_H(bool bVal)
{
	impl_->current_drawing_state_.flipH= true;
}
void odf_drawing_context::set_flip_V(bool bVal)
{
	impl_->current_drawing_state_.flipV= true;
}
void odf_drawing_context::set_tile(bool bVal)
{
	impl_->current_drawing_state_.tile = true;
}
void odf_drawing_context::set_rotate(int iVal)
{
	double dRotate = iVal/60000.;
	impl_->current_drawing_state_.rotateAngle = (360 - dRotate)/180. * 3.14159265358979323846;
}

void odf_drawing_context::start_image(std::wstring & path)
{
	office_element_ptr image_elm;
	create_element(L"draw", L"image", image_elm, impl_->odf_context_);

	draw_image* image = dynamic_cast<draw_image*>(image_elm.get());
	if (image == NULL)return;

    image->common_xlink_attlist_.href_= path;
	image->common_xlink_attlist_.type_= xlink_type::Simple;
	image->common_xlink_attlist_.show_ = xlink_show::Embed;
	image->common_xlink_attlist_.actuate_= xlink_actuate::OnLoad;

	start_element(image_elm);
}
void odf_drawing_context::end_image()
{
	end_element();
}
void odf_drawing_context::start_shape(int type, std::wstring & sub_type)
{
	office_element_ptr & shape_elm = create_shape(type);

	draw_shape* shape = dynamic_cast<draw_shape*>(shape_elm.get());
	if (shape == NULL)return;
//////////	
	impl_->styles_context_->create_style(L"",style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_shape_elm = impl_->styles_context_->last_state().get_office_element();
	std::wstring style_name;

	style* style_ = dynamic_cast<style*>(style_shape_elm.get());
	if (style_)
	{
		style_name = style_->style_name_;
	}

	shape->common_draw_attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_style_name_attlist_.draw_style_name_ = style_ref(style_name);
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int level = impl_->current_level_.size();
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(shape_elm);

	impl_->current_level_.push_back(shape_elm);

	odf_element_state state={shape_elm, style_name, style_shape_elm, level};

	impl_->current_drawing_state_.elements_.push_back(state);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (sub_type.length()>0 && shape_elm)
	{
		office_element_ptr enhanced_elm;
		create_element(L"draw", L"enhanced-geometry", enhanced_elm, impl_->odf_context_);
		draw_enhanced_geometry* enhanced = dynamic_cast<draw_enhanced_geometry*>(enhanced_elm.get());
		if (enhanced)
		{
			enhanced->draw_enhanced_geometry_attlist_.draw_type_ = sub_type;
		}
		start_element(enhanced_elm);
///....

		end_element();
	}
}
void odf_drawing_context::end_shape()
{
	end_element();
}
void odf_drawing_context::start_element(office_element_ptr & elm)
{
	int level = impl_->current_level_.size();

	odf_element_state state={elm, L"", office_element_ptr(), level};

	impl_->current_drawing_state_.elements_.push_back(state);
	
	if (impl_->current_level_.size()>0)
		impl_->current_level_.back()->add_child_element(elm);

	impl_->current_level_.push_back(elm);
}
void odf_drawing_context::end_element()
{
	impl_->current_level_.pop_back();
}
void odf_drawing_context::set_rect(double x_pt, double y_pt, double width_pt, double height_pt)
{
	//������� ��� ��������� ��� ������� � �� (���� ����� � � ������ �������)
	impl_->current_drawing_state_.svg_x_ = length(length(x_pt,length::pt).get_value_unit(length::cm),length::cm);
	impl_->current_drawing_state_.svg_y_ = length(length(y_pt,length::pt).get_value_unit(length::cm),length::cm);

	impl_->current_drawing_state_.svg_height_ = length(length(height_pt,length::pt).get_value_unit(length::cm),length::cm);	
	impl_->current_drawing_state_.svg_width_ = length(length(width_pt,length::pt).get_value_unit(length::cm),length::cm);	
	
}
bool odf_drawing_context::is_exist_content()
{
	return (impl_->drawing_list_.size()>0 ? true : false);
}

void odf_drawing_context::finalize(office_element_ptr & root_elm)
{
	for (int i=0; i< impl_->drawing_list_.size()>0; i++)
	{
		if (impl_->drawing_list_[i].elements_.size() > 0)
		{
			root_elm->add_child_element(impl_->drawing_list_[i].elements_[0].elm);
		}
	}
}
office_element_ptr odf_drawing_context::create_shape(int type)
{
	office_element_ptr element;
	switch(type)
	{
	case 1:
		create_element(L"draw", L"caption", element, impl_->odf_context_);
		break;
	case 2:
		create_element(L"draw", L"rect", element, impl_->odf_context_);
		break;
	case 3:
		create_element(L"draw", L"ellipse", element, impl_->odf_context_);
		break;
	case 4:
		create_element(L"draw", L"circle", element, impl_->odf_context_);
		break;
	case 5:
		create_element(L"draw", L"line", element, impl_->odf_context_);
		break;
	case 6:
		create_element(L"draw", L"path", element, impl_->odf_context_);
		break;
	case 7:
		create_element(L"draw", L"custom-shape", element, impl_->odf_context_);
		break;
	case 8:
		create_element(L"draw", L"polygon", element, impl_->odf_context_);
		break;
	case 9:
		create_element(L"draw", L"connector", element, impl_->odf_context_);
		break;
	
	}

	return element;
}

}
}