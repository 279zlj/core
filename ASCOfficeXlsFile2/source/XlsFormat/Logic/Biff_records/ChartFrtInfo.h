#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of ChartFrtInfo record in BIFF8
class ChartFrtInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ChartFrtInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartFrtInfo)
public:
	ChartFrtInfo();
	~ChartFrtInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeChartFrtInfo;

//-----------------------------
	unsigned char	verOriginator;
	unsigned char	verWriter;

};

} // namespace XLS

