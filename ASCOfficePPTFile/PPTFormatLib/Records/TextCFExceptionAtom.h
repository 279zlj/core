#pragma once
#include "../Reader/Records.h"

class CRecordTextCFExceptionAtom : public CUnknownRecord
{
public:
	CTextCFRun_ppt m_oCFRun;
	
	CRecordTextCFExceptionAtom()
	{
	}

	~CRecordTextCFExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oCFRun.LoadFromStream(pStream, false);

		// ��� �� ������ ������...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}

};