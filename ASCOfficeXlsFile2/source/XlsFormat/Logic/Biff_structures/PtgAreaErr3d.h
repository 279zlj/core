#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{

class CFRecord;

class PtgAreaErr3d: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAreaErr3d)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

	unsigned short ixti;
	GlobalWorkbookInfoPtr global_info;
};

} // namespace XLS

