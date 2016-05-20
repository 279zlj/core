#pragma once

#include <boost/optional.hpp>

namespace cpdoccore
{
    /// ������� ���������� ��� Optional, ���������� boost::optional

    template <class T>
    struct optional
    {
        typedef T Base;
        typedef boost::optional<T> Type;
    };


    #define _CP_OPT(V) optional<V>::Type

}
