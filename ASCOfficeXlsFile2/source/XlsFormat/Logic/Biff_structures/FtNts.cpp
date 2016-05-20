
#include "FtNts.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{


BiffStructurePtr FtNts::clone()
{
	return BiffStructurePtr(new FtNts(*this));
}


void FtNts::store(CFRecord& record)
{
	unsigned short ft = 0x0D; // reserved
	unsigned short cb = 0x16; // reserved
	record << ft << cb;

	_GUID_ guid_num;
	if(!STR::bstr2guid(guid, guid_num))
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"FtNts", guid);
	}
	record << guid_num << fSharedNote;
}


void FtNts::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	_GUID_ guid_num;
	record >> guid_num >> fSharedNote;
	
	guid = STR::guid2bstr(guid_num);
	
	record.skipNunBytes(4);
}


} // namespace XLS

