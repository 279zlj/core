
#include "CatSerRange.h"

namespace XLS
{

CatSerRange::CatSerRange()
{
}


CatSerRange::~CatSerRange()
{
}


BaseObjectPtr CatSerRange::clone()
{
	return BaseObjectPtr(new CatSerRange(*this));
}


void CatSerRange::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fBetween);
	SETBIT(flags, 1, fMaxCross);
	SETBIT(flags, 2, fReversed);

	record << catCross << catLabel << catMark << flags;
}


void CatSerRange::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> catCross >> catLabel >> catMark >> flags;

	fBetween = GETBIT(flags, 0);
	fMaxCross = GETBIT(flags, 1);
	fReversed = GETBIT(flags, 2);
}

} // namespace XLS

