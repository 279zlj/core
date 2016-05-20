#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SHAREDSTRINGS union of records 
class SHAREDSTRINGS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SHAREDSTRINGS)
public:
	SHAREDSTRINGS(const unsigned short code_page);
	~SHAREDSTRINGS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSHAREDSTRINGS;
	
	int serialize(std::wostream & stream);

	unsigned short	code_page_;
	unsigned int	size_;
};

} // namespace XLS

