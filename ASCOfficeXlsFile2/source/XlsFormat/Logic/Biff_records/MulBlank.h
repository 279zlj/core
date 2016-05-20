#pragma once

#include "BiffRecord.h"

namespace XLS
{


class IXFCellMulBlankSpecial : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IXFCellMulBlankSpecial)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record, const size_t num_cells);
//-----------------------------
	virtual void load(CFRecord& record) {};
	virtual void store(CFRecord& record){};

	static const ElementType	type = typeIXFCellMulBlankSpecial;
//-----------------------------
	unsigned short common_ixfe;
	std::vector<unsigned short> rgixfe;
};



// Logical representation of MulBlank record in BIFF8
class MulBlank: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MulBlank)
	BASE_OBJECT_DEFINE_CLASS_NAME(MulBlank)
public:
	MulBlank();
	~MulBlank();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	
    const int GetRow() const;

	int serialize(std::wostream & stream);

//-----------------------------
	GlobalWorkbookInfoPtr	global_info_;
	Rw						rw;
	Col						colFirst;
	IXFCellMulBlankSpecial	rgixfe;
	Col						colLast;
};



} // namespace XLS

