#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ForceFullCalculation record in BIFF8
class ForceFullCalculation: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ForceFullCalculation)
	BASE_OBJECT_DEFINE_CLASS_NAME(ForceFullCalculation)
public:
	ForceFullCalculation();
	~ForceFullCalculation();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeForceFullCalculation;

//-----------------------------
	bool fNoDeps;

};

} // namespace XLS

