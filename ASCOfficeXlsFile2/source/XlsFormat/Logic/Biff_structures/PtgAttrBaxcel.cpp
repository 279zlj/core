
#include "PtgAttrBaxcel.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrBaxcel::clone()
{
	return BiffStructurePtr(new PtgAttrBaxcel(*this));
}

void PtgAttrBaxcel::storeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // unused
}


void PtgAttrBaxcel::loadFields(CFRecord& record)
{
	bitSemi = 0 != (getPtgId() & 0x0100);
	record.skipNunBytes(2); // unused
}


void PtgAttrBaxcel::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// volatile property is not seen in the textual form
}


} // namespace XLS

