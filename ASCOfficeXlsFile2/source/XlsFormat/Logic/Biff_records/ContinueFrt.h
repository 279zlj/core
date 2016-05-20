#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ContinueFrt record in BIFF8
class ContinueFrt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ContinueFrt)
	BASE_OBJECT_DEFINE_CLASS_NAME(ContinueFrt)
public:
	ContinueFrt();
	~ContinueFrt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeContinueFrt;

};

} // namespace XLS

