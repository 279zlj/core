

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include <boost/foreach.hpp>

#include "docx_content_type.h"
#include "namespaces.h"

namespace cpdoccore { 
namespace oox {

const wchar_t * default_content_type::ns = L"";
const wchar_t * default_content_type::name = L"Default";

::std::wostream & default_content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Default")
        {
            CP_XML_ATTR(L"Extension", extension());
            CP_XML_ATTR(L"ContentType", content_type());        
        }    
    }
    return _Wostream;
}

const wchar_t * override_content_type::ns = L"";
const wchar_t * override_content_type::name = L"Override";

::std::wostream & override_content_type::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Override")
        {
            CP_XML_ATTR(L"PartName", part_name());
            CP_XML_ATTR(L"ContentType", content_type());
        }
    }
    return _Wostream;
}

const wchar_t * content_type_content::ns = L"";
const wchar_t * content_type_content::name = L"Types";

::std::wostream & content_type_content::xml_to_stream(::std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"Types")
        {
            CP_XML_ATTR(L"xmlns", xmlns::types.value);

            
            BOOST_FOREACH(const xml::element_wc & elm, default_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }

            BOOST_FOREACH(const xml::element_wc & elm, override_)
            {
                elm.xml_to_stream(CP_XML_STREAM());
            }            
        }
    }

    return _Wostream;
}

}
}
