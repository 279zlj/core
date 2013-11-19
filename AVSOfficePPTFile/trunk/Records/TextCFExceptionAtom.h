#pragma once
#include "..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oCFRun.LoadFromStream(pStream, false);

		// ��� �� ������ ������...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("TextCFExceptionAtom"));

		oWriter.WriteString(m_oCFRun.m_oRun.ToString(0));

		oWriter.WriteNodeEnd(_T("TextCFExceptionAtom"));
		return oWriter.GetXmlString();
	}
};