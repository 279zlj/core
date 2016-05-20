#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/MDir.h>

namespace XLS
{


// Logical representation of MDB record in BIFF8
class MDB: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDB)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDB)
public:
	MDB();
	~MDB();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BiffStructurePtrVector rgmdir;
};

} // namespace XLS

