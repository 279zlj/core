#pragma once

#include <iosfwd>
#include <iostream>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/attributes.h>

#include "mediaitems.h"
#include "oox_drawing_fills.h"

#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

static const int _odf_to_oox_ShapeType[]=
{ 4,4,4,34,};

static const std::wstring _ooxShapeType[]=
{
	L"rect", //frame
	L"rect", //text box
	L"rect", //shape
	L"ellipse",
	L"ellipse", 
	L"line", 
	L"path",
	L"custGeom",//uses sub-sub type,
	L"polygon", 
};

namespace cpdoccore {
namespace oox {

    struct _hlink_desc
    {
        std::wstring hId;
        std::wstring hRef;
        bool object;
    };
    struct _oox_drawing
    {
        _oox_drawing() : type(mediaitems::typeUnknown), id(0), x(0), y(0), cx(0), cy(0), sub_type(0), inGroup(false), name(L"object")
        {
        }
        mediaitems::Type type;

 		bool inGroup;
		size_t id;

        std::wstring name;

        _INT32 x, y;
        _INT32 cx, cy;

        _oox_fill fill;

        int sub_type; //odf
        std::wstring	chartId;

        std::vector<_hlink_desc> hlinks;

        std::vector<odf_reader::_property> additional;

    };
    void oox_serialize_xfrm		(std::wostream & strm, _oox_drawing & val, const std::wstring namespace_ = L"a");
    void oox_serialize_shape	(std::wostream & strm, _oox_drawing & val);
    void oox_serialize_ln		(std::wostream & strm, const std::vector<odf_reader::_property> & val, bool always_draw = false);
    void oox_serialize_hlink	(std::wostream & strm, const std::vector<_hlink_desc> & val);
	void oox_serialize_bodyPr	(std::wostream & strm, _oox_drawing & val, const std::wstring & namespace_ = L"a");
	void oox_serialize_aLst		(std::wostream & strm, const std::vector<odf_reader::_property> & val);
}
}
