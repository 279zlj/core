#pragma once

#include "BiffStructure.h"

#include "DXFNum.h"
#include "DXFFntD.h"
#include "DXFALC.h"
#include "DXFBdr.h"
#include "DXFPat.h"
#include "DXFProt.h"

#include <simple_xml_writer.h>

namespace XLS
{

class CFRecord;

class DXFN : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFN)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFN;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int serialize(std::wostream & stream);

	bool alchNinch;
	bool alcvNinch;
	
	bool wrapNinch;
	bool trotNinch;
	bool kintoNinch;
	bool cIndentNinch;
	bool fShrinkNinch;
	bool fMergeCellNinch;
	
	bool lockedNinch;
	bool hiddenNinch;
	
	bool glLeftNinch;
	bool glRightNinch;
	bool glTopNinch;
	bool glBottomNinch;
	bool glDiagDownNinch;
	bool glDiagUpNinch;
	
	bool flsNinch;
	bool icvFNinch;
	bool icvBNinch;
	bool ifmtNinch;
	bool fIfntNinch;
	
	bool ibitAtrNum;
	bool ibitAtrFnt;
	bool ibitAtrAlc;
	bool ibitAtrBdr;
	bool ibitAtrPat;
	bool ibitAtrProt;
	
	bool iReadingOrderNinch;
	bool fIfmtUser;
	bool fNewBorder;
	bool fZeroInited;

	DXFNum	dxfnum;
	DXFFntD dxffntd;
	DXFALC	dxfalc;
	DXFBdr	dxfbdr;
	DXFPat	dxfpat;
	DXFProt dxfprot;
};

typedef boost::shared_ptr<DXFN> DXFNPtr;

} // namespace XLS

