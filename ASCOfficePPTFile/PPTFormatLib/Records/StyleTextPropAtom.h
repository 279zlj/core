#pragma once
#include "../Reader/Records.h"

class CRecordStyleTextPropAtom : public CUnknownRecord
{
public:
	// ����� ��� ������� ������� � ������, 
	// ��������� �� ������
	LONG m_lOffsetInStream;
	
	// ������ ������. ����� ����������� �� ���� ��������
	// Text.Bytes/Chars.Atom
	DWORD m_lCount; 

public:
	std::vector<CTextPFRun_ppt> m_arrPFs;
	std::vector<CTextCFRun_ppt> m_arrCFs;

public:
	
	CRecordStyleTextPropAtom()
	{
		m_lCount = 0;
		m_lOffsetInStream = 0;
	}

	~CRecordStyleTextPropAtom()
	{
		m_lCount = 0;
	}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

		DWORD lMemCount = 0;
		DWORD lCountItems = 0;

		if (0 == m_lCount)
		{
			StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
			return;
		}

		while (lMemCount < m_lCount + 1)
		{
			if (pStream->tell() >= m_lOffsetInStream + m_oHeader.RecLen) break;

			CTextPFRun_ppt elm;
			m_arrPFs.push_back(elm);
			m_arrPFs[lCountItems].LoadFromStream(pStream);
			lMemCount += m_arrPFs[lCountItems].m_lCount;

			++lCountItems;
		}

		lMemCount = 0;
		lCountItems = 0;
		while (lMemCount < m_lCount + 1)
		{
			if (pStream->tell() >= m_lOffsetInStream + m_oHeader.RecLen) break;
			
			CTextCFRun_ppt elm;
			m_arrCFs.push_back(elm);

			m_arrCFs[lCountItems].LoadFromStream(pStream);
			lMemCount += m_arrCFs[lCountItems].m_lCount;

			++lCountItems;
		}

		// �� ������ ������...
		// ����� ����� ����� ������� ������ (�.�. �������� - placeholder � �������) - 
		// �� � ��� ����������� ���������� m_lCount...
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
	}

};
