#pragma once

#include <string>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_drawings.h"

namespace cpdoccore { 

	namespace svg_path
	{
		struct _polyline;
	};
	
namespace oox {

struct drawing_object_description;
struct xlsx_table_position;
class xlsx_table_metrics;
class mediaitems;
class xlsx_drawings;
struct _oox_fill;

typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(mediaitems & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings);
    const std::vector<drawing_elm> & content() const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

//class xlsx_drawings;
//typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle & h);
    ~xlsx_drawing_context();

	void set_odf_packet_path(std::wstring path){odf_packet_path_ = path;}//��� ������� ��������

    void start_shapes();
    void start_drawing(std::wstring const & name);
	
	void set_rect(double width_pt, double height_pt, double x_pt, double y_pt);

	void set_translate(double x_pt, double y_pt);
	void set_scale(double cx_pt, double cy_pt);
	void set_rotate(double angle);

	void set_anchor(std::wstring anchor, double x_pt, double y_pt);
	void set_property(odf_reader::_property p);
	std::vector<odf_reader::_property> & get_properties();
    void set_clipping(const std::wstring & str );
	void set_fill(_oox_fill & fill);

	void end_drawing();
    void end_shapes();

	std::wstring add_hyperlink(std::wstring const & ref, bool object);

    void start_image(std::wstring const & path);
    void end_image();
   
	void start_chart(std::wstring const & path);
    void end_chart();

    void start_shape(int type);
	//...���� ���� ����� �������� ... ������� ��� ������� ������� �������
    void end_shape();

	void start_object_ole();
	void end_object_ole();
	
	bool empty() const;

    void write_drawing(std::wostream & strm);
	std::wstring dump_path(std::vector<svg_path::_polyline> & path, double w,double h);

    xlsx_drawings_ptr get_drawings();

    void process_images(xlsx_table_metrics & table_metrics);
    void process_charts(xlsx_table_metrics & table_metrics);
    void process_shapes(xlsx_table_metrics & table_metrics);

	void process_position_properties(drawing_object_description & pic,xlsx_table_metrics & table_metrics,xlsx_table_position & from,xlsx_table_position & to);
	void process_common_properties(drawing_object_description& pic,_xlsx_drawing & drawing,xlsx_table_metrics & table_metrics);

private:
	void default_set();
	int hlinks_size_;
	std::wstring odf_packet_path_ ;
	float dpi_;
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}
