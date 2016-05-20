#pragma once

#include "BiffStructure.h"
#include "DXFN12.h"
#include "CFExTemplateParams.h"

namespace XLS
{

class CFRecord;

class CFExNonCF12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFExNonCF12)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeCFExNonCF12;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short	icf;
	unsigned char	cp;
	unsigned char	icfTemplate;
	unsigned short	ipriority_;

	bool			fActive;
	bool			fStopIfTrue;

	unsigned char	fHasDXF;

	DXFN12				dxf;
	CFExTemplateParams	rgbTemplateParms;
};

} // namespace XLS

