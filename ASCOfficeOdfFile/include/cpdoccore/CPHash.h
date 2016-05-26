#pragma once

#include "../../include/cpdoccore/CPOptional.h"

namespace boost 
{
	template< class T > struct hash;
}

namespace cpdoccore {

template <class T>
std::size_t hash_value(typename optional<T>::Type const & val)
{
    if (val)
    {
        boost::hash<T>(*val);        
    }
    else
        return 0;
}

}

