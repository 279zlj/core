#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of CrtMlFrt record in BIFF8
class CrtMlFrt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrtMlFrt)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrtMlFrt)
public:
	CrtMlFrt();
	~CrtMlFrt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrtMlFrt;

//-----------------------------

	_UINT32	cb;

};

} // namespace XLS

