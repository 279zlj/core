#pragma once
#include "../Reader/Records.h"

class CRecordAnimationInfoAtom : public CUnknownRecord
{
	// ����...

public:
	
	CRecordAnimationInfoAtom()
	{
	}

	~CRecordAnimationInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};