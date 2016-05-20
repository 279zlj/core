#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtFRIT.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtFSP : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFSP)
public:
	OfficeArtFSP();
	XLS::BiffStructurePtr clone();

	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtFSP;

	const unsigned short GetInstanceToStore();

	_UINT16	shape_id;
	_UINT32	spid;

	bool fGroup;
	bool fChild;
	bool fPatriarch;
	bool fDeleted;
	bool fOleShape;
	bool fHaveMaster;
	bool fFlipH;
	bool fFlipV;
	bool fConnector;
	bool fHaveAnchor;
	bool fBackground;
	bool fHaveSpt;
};

typedef boost::shared_ptr<OfficeArtFSP> OfficeArtFSPPtr;


} // namespace XLS
