#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtRecordHeader : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRecordHeader)
public:
	OfficeArtRecordHeader();
	XLS::BiffStructurePtr clone();
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecordHeader;

	const size_t size() const;

	unsigned char	recVer;
	unsigned short	recInstance;
	unsigned short	recType;
	_UINT32	recLen;
};

typedef boost::shared_ptr<OfficeArtRecordHeader> OfficeArtRecordHeaderPtr;


} // namespace ODRAW
