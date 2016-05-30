#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "style_ref.h"
#include "bool.h"

namespace cpdoccore { 
namespace odf_writer {

/// \brief  table:named-expressions
class table_database_ranges : public office_element_impl<table_database_ranges>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseRanges;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array database_ranges_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_database_ranges);

/// \brief  table:named-range
class table_database_range: public office_element_impl<table_database_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseRange;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring)		table_name_;
    _CP_OPT(std::wstring)		table_target_range_address_;

	_CP_OPT(odf_types::Bool)	table_display_filter_buttons_;


};

CP_REGISTER_OFFICE_ELEMENT2(table_database_range);


}
}
