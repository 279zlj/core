#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Radar record in BIFF8
class Radar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Radar)
	BASE_OBJECT_DEFINE_CLASS_NAME(Radar)
public:
	Radar();
	~Radar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRadar;

	int serialize(std::wostream & _stream);

//-----------------------------
	BIFF_BOOL fRdrAxLab;
	BIFF_BOOL fHasShadow;
};

} // namespace XLS

