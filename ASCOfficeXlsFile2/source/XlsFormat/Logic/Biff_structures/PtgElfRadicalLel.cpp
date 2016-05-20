
#include "PtgElfRadicalLel.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgElfRadicalLel::clone()
{
	return BiffStructurePtr(new PtgElfRadicalLel(*this));
}

//
//void PtgElfRadicalLel::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ilel", ilel);
//	xml_tag->setAttribute(L"fQuoted", fQuoted);
//}


void PtgElfRadicalLel::storeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fQuoted);
	record << ilel << flags;
}


void PtgElfRadicalLel::loadFields(CFRecord& record)
{
	unsigned short flags;
	record >> ilel >> flags;
	fQuoted = GETBIT(flags, 0);
}


void PtgElfRadicalLel::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// The significant tag follows (PtgArea or PtgAreaErr)
}


} // namespace XLS

