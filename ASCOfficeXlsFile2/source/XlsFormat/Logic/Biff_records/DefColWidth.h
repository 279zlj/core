#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DefColWidth record in BIFF8
class DefColWidth: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DefColWidth)
	BASE_OBJECT_DEFINE_CLASS_NAME(DefColWidth)
public:
	DefColWidth();
	~DefColWidth();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDefColWidth;

//-----------------------------
	_UINT16 cchdefColWidth;
};

} // namespace XLS

