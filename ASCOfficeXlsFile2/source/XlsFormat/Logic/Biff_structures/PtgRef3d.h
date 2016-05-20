#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{

class CFRecord;

class PtgRef3d: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgRef3d)
public:
	PtgRef3d(const CellRef& cell_base_ref_init);
	PtgRef3d(const unsigned short ixti, const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();

	void fromString(const std::wstring& str);

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

	static const unsigned short fixed_id = 0x1A;
private:
//biff8
	unsigned short			ixti;
	RgceLocRel				rgce_loc_rel;
	RgceLoc					rgce_loc;
	const CellRef&			cell_base_ref;
//biff5
	_UINT16					ixals;
	_UINT16					itabFirst;
	_UINT16					itabLast;
	_UINT16					grbitRw;
	unsigned char			col;

	GlobalWorkbookInfoPtr	global_info;

};

} // namespace XLS

