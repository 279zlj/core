#include <vector>

#include <boost/foreach.hpp>

#include <cpdoccore/xml/utils.h>

#include "xlsx_sharedstrings.h"

namespace cpdoccore {
namespace oox {

class xlsx_shared_strings::Impl
{
public:
    void serialize(std::wostream & _Wostream) const;
    size_t add(const std::wstring & content);

private:
    std::vector<std::wstring> strings_;
};

void xlsx_shared_strings::Impl::serialize(std::wostream & _Wostream) const
{
    _Wostream << L"<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"" << 
        strings_.size() << "\" uniqueCount=\"" << strings_.size() << "\">";
    
    BOOST_FOREACH(const std::wstring & str, strings_)
    {
        _Wostream << L"<si>";
			_Wostream << str;
        _Wostream << L"</si>";
    }

    _Wostream << L"</sst>";
}

size_t xlsx_shared_strings::Impl::add(const std::wstring & content)
{
    strings_.push_back(content);
    return strings_.size() - 1;
}

xlsx_shared_strings::xlsx_shared_strings(): impl_( new xlsx_shared_strings::Impl() ) 
{
}

xlsx_shared_strings::~xlsx_shared_strings()
{
}

void xlsx_shared_strings::serialize(std::wostream & _Wostream) const
{
    return impl_->serialize(_Wostream);
}

size_t xlsx_shared_strings::add(const std::wstring & content)
{
    return impl_->add(content);
}

}
}
