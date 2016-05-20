#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{


// Logical representation of SXTH record in BIFF8
class SXTH: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXTH)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXTH)
public:
	SXTH();
	~SXTH();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXTH;


};

} // namespace XLS

