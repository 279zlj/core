#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ObjectLink record in BIFF8
class ObjectLink: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ObjectLink)
	BASE_OBJECT_DEFINE_CLASS_NAME(ObjectLink)
public:
	ObjectLink();
	~ObjectLink();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeObjectLink;

	BIFF_WORD wLinkObj;
	BIFF_WORD wLinkVar1;
	BIFF_WORD wLinkVar2;

//-------------------------------

	int testLinkObj;
	int testLinkVar1;
	int testLinkVar2;

};

} // namespace XLS

