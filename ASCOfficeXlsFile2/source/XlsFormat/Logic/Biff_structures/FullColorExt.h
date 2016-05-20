#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class FullColorExt : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FullColorExt)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeFullColorExt;

	unsigned char	icv;
	unsigned char	xclrType;
	short			nTintShade;
	_UINT32			xclrValue;
};

} // namespace XLS
