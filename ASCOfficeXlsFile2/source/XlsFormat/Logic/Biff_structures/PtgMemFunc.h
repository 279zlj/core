#pragma once

#include "Ptg.h"

namespace XLS
{

class CFRecord;

class PtgMemFunc : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgMemFunc)
public:
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

private:
	unsigned short cce;
};

} // namespace XLS

