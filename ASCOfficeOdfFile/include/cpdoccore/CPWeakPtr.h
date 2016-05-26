#pragma once

#include <boost/weak_ptr.hpp>

namespace cpdoccore {

template <class T>
	struct weak_ptr 
	{
		typedef ::boost::weak_ptr<T> Type;
	};

}
