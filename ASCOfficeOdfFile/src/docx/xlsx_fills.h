#pragma once

#include <string>
#include <cpdoccore/CPOptional.h>
#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>
#include "xlsx_fill.h"

namespace cpdoccore { 
namespace odf_reader {

    class text_format_properties_content;
    class paragraph_format_properties;
    class style_table_cell_properties_attlist;
}
}

namespace cpdoccore {
namespace oox {

    class xlsx_fills
    {
    public:
        xlsx_fills();
        ~xlsx_fills();
    public:
        size_t size() const;
       
		size_t fillId(	const odf_reader::text_format_properties_content		* textProp,
						const odf_reader::paragraph_format_properties			* parProp,
						const odf_reader::style_table_cell_properties_attlist	* cellProp, bool  default_set);

        size_t fillId(	const odf_reader::text_format_properties_content		* textProp,
						const odf_reader::paragraph_format_properties			* parProp,
						const odf_reader::style_table_cell_properties_attlist	* cellProp, bool default_set,bool & is_default);

        void serialize(std::wostream & _Wostream) const;

    private:
        class Impl;
        _CP_SCOPED_PTR(Impl) impl_;

    };
}
}
