
#include "ShapePropsStream.h"

namespace XLS
{

ShapePropsStream::ShapePropsStream()
{
}


ShapePropsStream::~ShapePropsStream()
{
}


BaseObjectPtr ShapePropsStream::clone()
{
	return BaseObjectPtr(new ShapePropsStream(*this));
}


void ShapePropsStream::writeFields(CFRecord& record)
{
#pragma message("####################### ShapePropsStream record is not implemented")
	Log::error("ShapePropsStream record is not implemented.");
}


void ShapePropsStream::readFields(CFRecord& record)
{
	record >> frtHeader;
	record >> wObjContext;

	record.skipNunBytes(2);

	record >> dwChecksum;
	
	_UINT32	cb=0;
	record >> cb;

	if (cb > 0)
	{
		xml_ = std::string( record.getCurData<char>(), cb);
		record.skipNunBytes(cb);
	}
}

} // namespace XLS

