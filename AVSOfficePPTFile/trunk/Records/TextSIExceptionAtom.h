#pragma once
#include "..\Reader\Records.h"

class CRecordTextSIExceptionAtom : public CUnknownRecord
{
public:
	CTextSIRun m_oSIRun;
	
	CRecordTextSIExceptionAtom()
	{
	}

	~CRecordTextSIExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		NSStreamReader::Read(pStream, m_oSIRun, false);

		// ��� �� ������ ������...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("TextPFExceptionAtom"));

		oWriter.WriteString(m_oSIRun.ToString());

		oWriter.WriteNodeEnd(_T("TextPFExceptionAtom"));
		return oWriter.GetXmlString();
	}
};