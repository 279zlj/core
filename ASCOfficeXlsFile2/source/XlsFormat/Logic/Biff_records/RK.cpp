#include "RK.h"

namespace XLS
{

RK::RK()
{
}


RK::~RK()
{
}


BaseObjectPtr RK::clone()
{
	return BaseObjectPtr(new RK(*this));
}


void RK::writeFields(CFRecord& record)
{
#pragma message("####################### RK record is not implemented")
	Log::error("RK record is not implemented.");
	//record << some_value;
}


void RK::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	Rw rw;
	Col col;
	record >> rw >> col >> rkrec;
	cell = Cell(rw, col, rkrec.get_ixfe());
}

const CellRef RK::getLocation() const
{
	return cell.getLocation();
}

int RK::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe > global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << rkrec.RK_.value();
			}
		}			
	}
	return 0;
}
} // namespace XLS

