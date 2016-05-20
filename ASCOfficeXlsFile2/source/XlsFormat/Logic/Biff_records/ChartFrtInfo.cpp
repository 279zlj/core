
#include "ChartFrtInfo.h"
#include <Logic/Biff_structures/FrtHeaderOld.h>

namespace XLS
{

ChartFrtInfo::ChartFrtInfo()
{
}


ChartFrtInfo::~ChartFrtInfo()
{
}


BaseObjectPtr ChartFrtInfo::clone()
{
	return BaseObjectPtr(new ChartFrtInfo(*this));
}


void ChartFrtInfo::writeFields(CFRecord& record)
{

}


void ChartFrtInfo::readFields(CFRecord& record)
{
	FrtHeaderOld frtHeaderOld;
	record >> frtHeaderOld >> verOriginator >> verWriter;
	unsigned short cCFRTID;
	record >> cCFRTID;


}

} // namespace XLS

