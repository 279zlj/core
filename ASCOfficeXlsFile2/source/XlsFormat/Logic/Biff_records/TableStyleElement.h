#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of TableStyleElement record in BIFF8
class TableStyleElement: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TableStyleElement)
	BASE_OBJECT_DEFINE_CLASS_NAME(TableStyleElement)
public:
	TableStyleElement();
	~TableStyleElement();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTableStyleElement;

//-----------------------------
	_UINT32	tseType;
	_UINT32	size;
	_UINT32	index;

};

} // namespace XLS

