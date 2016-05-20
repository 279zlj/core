#pragma once
#include "../Reader/Records.h"

class CRecordTextSpecInfoAtom : public CUnknownRecord
{
public:
	// ����� ��� ������� ������� � ������, 
	// ��������� �� ������
	LONG											m_lOffsetInStream;
	DWORD											m_lCount; 

	std::vector<NSPresentationEditor::CTextSIRun>	m_arrSIs;

	
	CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
		m_lOffsetInStream = 0;
	}

	~CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
	}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

		DWORD lMemCount = 0;
		while (true)
		{
			NSPresentationEditor::CTextSIRun elm;
			m_arrSIs.push_back(elm);
			
			NSStreamReader::Read(pStream, m_arrSIs.back());
			lMemCount += m_arrSIs.back().lCount;

			int sz = pStream->tell() - m_lOffsetInStream;

			if (sz >= m_oHeader.RecLen)
				break;
		}

		// �� ������ ������...
		// ����� ����� ����� ������� ������ (�.�. �������� - placeholder � �������) - 
		// �� � ��� ����������� ���������� m_lCount... �� ����� m_lCount ������ .. ����� �������� si ����� ���������
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
	}

    void ApplyProperties(CTextAttributesEx* pText)
	{
		if (m_arrSIs.size() < 1) return;

		int pos_text = 0, pos_si = 0;
		int ind = 0;
		for (int i = 0; i < pText->m_arParagraphs.size(); i++)
		{
			if (ind >= m_arrSIs.size()) break;
			
			for (int j = 0 ; j < pText->m_arParagraphs[i].m_arSpans.size(); j++)
			{
				if (pos_text + pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() > pos_si + m_arrSIs[ind].lCount )
				{
					pos_si += m_arrSIs[ind].lCount;
					ind++;
				}
				if (ind >= m_arrSIs.size()) break;
				if (m_arrSIs[ind].bLang)
					pText->m_arParagraphs[i].m_arSpans[j].m_oRun.Language = m_arrSIs[ind].Lang;
				pos_text += pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() ;
			}

		}
	}

	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		for (size_t nIndexSI = 0; nIndexSI < m_arrSIs.size(); ++nIndexSI)
		{
			oWriter.WriteString(m_arrSIs[nIndexSI].ToString());
		}

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};
