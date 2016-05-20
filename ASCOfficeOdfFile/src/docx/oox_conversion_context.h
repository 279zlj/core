#pragma once

#include <sstream>
#include <iosfwd>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore { 

namespace odf_reader{
	class style_instance;
};

class styles_context : boost::noncopyable
{
public:
    void start();
   
	std::wstringstream & text_style();
    std::wstringstream & paragraph_nodes();
    std::wstringstream & paragraph_attr();
    std::wstringstream & table_style();
    std::wstringstream & list_style();
	std::wstringstream & math_text_style();

    void docx_serialize_text_style(std::wostream & strm, std::wstring parenStyleId);
    void docx_serialize_table_style(std::wostream & strm);
  
	void pptx_serialize_table_style(std::wostream & strm);

	std::wstring & extern_node(){return extern_node_;}
	std::wstring & hlinkClick(){return hlinkClick_;}

	const odf_reader::style_instance * get_current_processed_style() const { return current_processed_style_; }
    void start_process_style(const odf_reader::style_instance * Instance);
    void end_process_style();

private:
	const odf_reader::style_instance *	current_processed_style_;
	
	std::wstring				extern_node_;
	std::wstring				hlinkClick_;

    std::wstringstream			list_style_;
    std::wstringstream			text_style_;
    std::wstringstream			paragraph_nodes_;
    std::wstringstream			paragraph_attr_;
    std::wstringstream			table_style_;
	std::wstringstream			math_text_style_;
};


namespace oox {
	

}
}

