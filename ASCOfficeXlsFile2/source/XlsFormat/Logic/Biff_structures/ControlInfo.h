#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class ControlInfo : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ControlInfo)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeControlInfo;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	BIFF_BYTE fDefault;
	BIFF_BYTE fHelp;
	BIFF_BYTE fCancel;
	BIFF_BYTE fDismiss;
	_UINT16 accel1;
};

} // namespace XLS

