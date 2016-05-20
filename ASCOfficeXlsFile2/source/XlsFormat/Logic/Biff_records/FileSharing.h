#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of FileSharing record in BIFF8
class FileSharing: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FileSharing)
	BASE_OBJECT_DEFINE_CLASS_NAME(FileSharing)
public:
	FileSharing();
	~FileSharing();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFileSharing;

//-----------------------------
	Boolean<unsigned short> fReadOnlyRec;
	unsigned short			wResPassNum;
	BIFF_BSTR				wResPass;
	_UINT16					iNoResPass;
	XLUnicodeString			stUNUsername;

};

} // namespace XLS

