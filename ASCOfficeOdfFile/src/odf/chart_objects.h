#pragma once

#include <cpdoccore/xml/attributes.h>
#include "../docx/oox_drawing_fills.h"
//�������� ������ ����������� �������� ( ��������� ���������) ������� ������������ � xlsx & odf_reader - 

namespace cpdoccore { 
namespace odf_reader {

class text_format_properties_content;

namespace chart {

	enum oox_typeconvert {docx,xlsx,pptx};

    enum class_type 
    {
        chart_unknown,
        chart_line, 
        chart_area,
        chart_circle,
        chart_ring,
        chart_scatter,
        chart_radar,
        chart_bar,
        chart_stock,
        chart_bubble,
        chart_surface,
        chart_gantt,
		chart_filled_radar
    };

	struct title
	{
		title() : bEnabled(false), pos_x(0), pos_y(0) {}
		bool					bEnabled;
		
		std::wstring			content_;
		std::vector<_property>	text_properties_;
		
		double					pos_x;
		double					pos_y;
		
	} ; 
	struct simple
	{
		simple() : bEnabled(false) {}
		bool					bEnabled;
		std::vector<_property>	properties_; 
 		std::vector<_property>	text_properties_;
		std::vector<_property>	graphic_properties_;
		oox::_oox_fill			fill_;
	};
	struct treadline
	{
		bool					bEquation;
		bool					bREquation;
		std::vector<_property>	line_properties_;
		simple					equation_properties_;

		treadline(){bEquation = false; bREquation = false;}

	};
	struct plot_area : public simple
    {
		std::wstring cell_range_address_;
		//series_in_column_or_row
		//uses_first_as_label
	};
	struct axis: public simple
    {
		axis() : bCategories_(false), type_(3) {}
        struct grid
        {
            enum grid_type {major, minor};

            grid_type				type_;
            std::wstring			style_name_;
 			
			std::vector<_property>	graphic_properties_;
       };
		bool				bCategories_;
		
		title title_;
        
		std::wstring		dimension_;
        std::wstring		chart_name_;
        std::wstring		style_name_;
        std::vector<grid>	grids_;
		int					type_;
		
  };
    struct series : public simple
    {
		struct point : public simple
        {
			point(unsigned int rep/*, std::wstring const & styleName*/) : repeated_(rep)/*, style_name_(styleName)*/{}

            unsigned int	repeated_;
        };

        std::wstring		name_;
		std::wstring		cell_range_address_;

        std::wstring		label_cell_address_;
        class_type			class_;
        std::wstring		attached_axis_;
        std::wstring		style_name_;  
        std::vector<point>	points_;
		
		simple				mean_value_;
		simple				error_indicator_;
		treadline			regression_curve_;

		series(){}
        series(
				std::wstring const & rangeAddress,
				std::wstring const & labelCell,
				class_type classType,
				std::wstring const & attachedAxis,
				std::wstring const & styleName                        
            ) : 
				cell_range_address_(rangeAddress),
                label_cell_address_(labelCell),
                class_(classType),
                attached_axis_(attachedAxis),
                style_name_(styleName)
			{}

    };
}
}
}