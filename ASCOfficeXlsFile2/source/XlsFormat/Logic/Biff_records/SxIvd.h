#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{


// Logical representation of SxIvd record in BIFF8
class SxIvd: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxIvd)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxIvd)
public:
	SxIvd();
	~SxIvd();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSxIvd;

};

} // namespace XLS

