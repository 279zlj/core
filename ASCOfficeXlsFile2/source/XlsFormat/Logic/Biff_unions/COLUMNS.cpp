
#include "COLUMNS.h"
#include <Logic/Biff_records/DefColWidth.h>
#include <Logic/Biff_records/ColInfo.h>

namespace XLS
{


COLUMNS::COLUMNS()
{
}


COLUMNS::~COLUMNS()
{
}


BaseObjectPtr COLUMNS::clone()
{
	return BaseObjectPtr(new COLUMNS(*this));
}


// COLUMNS = DefColWidth *255ColInfo
const bool COLUMNS::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();

	bool def_ok = proc.optional<DefColWidth>(); 
			// OpenOffice Calc stored files workaround (DefColWidth is mandatory according to [MS-XLS])

	if (def_ok)
	{
		m_DefColWidth = elements_.back();
		elements_.pop_back();
	}
	bool col_ok = proc.repeated<ColInfo>(0, 255);

	int last_add = 0;

	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
	{
		ColInfo* column_info = dynamic_cast<ColInfo*>(it->get());

		for (int i = column_info->colFirst; i <= column_info->colLast; i++)
		{
			if (column_info->coldx > 0)
			{
				global_info_->customColumnsWidth.insert(std::pair<int, double>(i,column_info->coldx / 256.));
			}
		}

	}

	return def_ok || col_ok;
}

int COLUMNS::serialize(std::wostream & stream)
{
	if (elements_.size() < 1) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cols")
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
			{
				ColInfo* column_info = dynamic_cast<ColInfo*>(it->get());

				CP_XML_NODE(L"col")
				{
					if (column_info->coldx > 0)
					{
						CP_XML_ATTR(L"width", column_info->coldx / 256.);
						CP_XML_ATTR(L"customWidth", true);
					}

					CP_XML_ATTR(L"min", column_info->colFirst + 1);/// from 0 
					CP_XML_ATTR(L"max", column_info->colLast + 1);

					if (column_info->ixfe > global_info_->cellStyleXfs_count)
					{
						CP_XML_ATTR(L"style", column_info->ixfe - global_info_->cellStyleXfs_count);
					}

					if (column_info->fBestFit)
					{
						CP_XML_ATTR(L"bestFit", column_info->fBestFit);
					}
					if (column_info->fHidden)
					{
						CP_XML_ATTR(L"hidden", column_info->fHidden);
					}
					if (column_info->iOutLevel>0)
					{
						CP_XML_ATTR(L"outlineLevel", column_info->iOutLevel);
					}
				}	
			}
		}
	}
	return 0;
}


} // namespace XLS

