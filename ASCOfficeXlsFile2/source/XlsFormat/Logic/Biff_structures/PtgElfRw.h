#pragma once

#include "OperandPtg.h"

namespace XLS
{

class CFRecord;

class PtgElfRw: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgElfRw)
public:
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

private:
	RgceElfLoc loc;
};

} // namespace XLS

