#pragma once

#include "../../../ASCOfficePPTFile/PPTFormatLib/Records/Drawing/ArtBlip.h"

#include "../../../../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "Enums.h"

// ��� �����, �������������� ��� �������� ������� ��������,
// �������� - ��������� �� ��������... (�� PID'�)

class CProperty
{
public:
	NSOfficeDrawing::PropertyId		m_ePID;
	bool							m_bIsBlip;
	bool							m_bComplex;
	DWORD							m_lValue;
	BYTE*							m_pOptions;
	// ����� �� ������� ������ - ��� value, ����� m_bComplex == true
	bool							m_bIsTruncated;

public:
	CProperty()
	{
		m_ePID = NSOfficeDrawing::left;
		m_bIsBlip = false;
		m_bComplex = false;
		m_lValue = 0;
		m_pOptions = NULL;

		m_bIsTruncated = false;
	}
	~CProperty()
	{
		RELEASEARRAYOBJECTS(m_pOptions);
	}
	void FromStream(POLE::Stream* pStream)
	{
		// ������ �� ������...
		// ������ ���� ��� ����� bComplex
		// �.�. Complex - ����������� � ����������, �������� ��� ��������
		USHORT lMem = StreamUtils::ReadWORD(pStream);
		m_ePID = (NSOfficeDrawing::PropertyId)(lMem & 0x3FFF);
		
		m_bIsBlip = ((lMem & 0x4000) == 0x4000);
		m_bComplex = ((lMem & 0x8000) == 0x8000);

		m_lValue = StreamUtils::ReadDWORD(pStream);
	}

	void ComplexFromStream(POLE::Stream* pStream)
	{
		if (m_bComplex && m_lValue > 0)
		{
			if (NSOfficeDrawing::dgmConstrainBounds		== m_ePID ||
				NSOfficeDrawing::fillShadeColors		== m_ePID ||
				NSOfficeDrawing::lineDashStyle			== m_ePID ||
				NSOfficeDrawing::pAdjustHandles			== m_ePID ||
				NSOfficeDrawing::pConnectionSites		== m_ePID ||
				NSOfficeDrawing::pConnectionSitesDir	== m_ePID ||
				NSOfficeDrawing::pInscribe				== m_ePID ||
				NSOfficeDrawing::pSegmentInfo			== m_ePID ||
				NSOfficeDrawing::pVertices				== m_ePID ||
				NSOfficeDrawing::pGuides				== m_ePID ||
				NSOfficeDrawing::pWrapPolygonVertices	== m_ePID ||
				NSOfficeDrawing::pRelationTbl			== m_ePID ||
				NSOfficeDrawing::tableRowProperties		== m_ePID ||
				NSOfficeDrawing::lineLeftDashStyle		== m_ePID ||
				NSOfficeDrawing::lineTopDashStyle		== m_ePID ||
				NSOfficeDrawing::lineRightDashStyle		== m_ePID ||
				NSOfficeDrawing::lineBottomDashStyle	== m_ePID)
			{
				WORD nElems			= StreamUtils::ReadWORD(pStream);
				WORD nElemsAlloc	= StreamUtils::ReadWORD(pStream);
				WORD nElemSize		= StreamUtils::ReadWORD(pStream);

				if (0xFFF0 == nElemSize)
				{
					nElemSize = 4;
					m_bIsTruncated = TRUE;
				}

				LONG dwSize = nElems * nElemSize;			

				if (m_lValue != (dwSize + 6))
				{
                   bool b = false;
				}

				m_lValue = dwSize;
			}

			switch( m_ePID )
			{
			case NSOfficeDrawing::fillBlip:
				{
					SRecordHeader oHeader;
					if (oHeader.ReadFromStream(pStream) == false )
					{
						return;
					}
					switch (oHeader.RecType)
					{
						case RECORD_TYPE_ESCHER_BLIP_EMF:
						case RECORD_TYPE_ESCHER_BLIP_WMF:
						case RECORD_TYPE_ESCHER_BLIP_PICT:
						case RECORD_TYPE_ESCHER_BLIP_JPEG:
						case RECORD_TYPE_ESCHER_BLIP_PNG:
						case RECORD_TYPE_ESCHER_BLIP_DIB:
						case RECORD_TYPE_ESCHER_BLIP_TIFF:
						{
							CRecordOfficeArtBlip art_blip;
							art_blip.ReadFromStream(oHeader, pStream);
						}
					}

				}break;
			default:
				{
					if (0 == m_lValue)
					{
						return;
					}				
					
					RELEASEARRAYOBJECTS(m_pOptions);
					m_pOptions = new BYTE[m_lValue];

					ULONG lReadBytes = 0;
					lReadBytes = pStream->read(m_pOptions, m_lValue);
					if (lReadBytes != m_lValue)
					{
						return;
					}
				}
			}
		}
	}

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("%d,%d,%d,%d"), (long)m_ePID, (long)m_bIsBlip, (long)m_bComplex, (long)m_lValue);
		// �� ����� ������ ������ - � �� xml - �� ������������
		/*if (m_bComplex)
		{
			CString strProp = CDirectory::BYTEArrayToString(m_pOptions, m_lValue);
			if (NSOfficeDrawing::pibName == m_ePID)
			{
				strProp = (CString)CDirectory::BYTEArrayToStringW(m_pOptions, m_lValue);
			}
			str += _T(" options=") + strProp;
		}*/
		return _T("<Property command='") + str + _T("'/>");
	}
};

// � ��� ��������� ���������
class CProperties
{
public:
    std::vector<CProperty> m_arProperties;
	// �� ���� - ��� instance, �� ��� ��� �������,
	// ��� ����� ��� ����� - �� ��������� � RecordHeader
	long m_lCount;

public:
	CProperties() : m_arProperties()
	{
	}
	~CProperties()
	{
		m_lCount = 0;
        m_arProperties.clear();
	}

	void FromStream(POLE::Stream* pStream, long lCount)
	{
		m_lCount = lCount;
		for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
		{
            CProperty elem;
            m_arProperties.push_back(elem);
			m_arProperties[lIndex].FromStream(pStream);
		}
		// ������ ������ �������������� ���������� 
		// ����������� �� pid'�� (�� � ��� ���� ������ ��-�������)
		for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
		{
			m_arProperties[lIndex].ComplexFromStream(pStream);
		}
	}

	CString ToString()
	{
		CString str = _T("");
        for (size_t nIndex = 0; nIndex < m_arProperties.size(); ++nIndex)
		{
			str += m_arProperties[nIndex].ToString();
		}
		return _T("<Properties>") + str + _T("</Properties>");
	}

	DWORD GetLen()
	{
		DWORD dwLen = 6 * m_lCount;
		for (long nIndex = 0; nIndex < m_lCount; ++nIndex)
		{
			if (m_arProperties[nIndex].m_bComplex)
			{
				dwLen += m_arProperties[nIndex].m_lValue;
			}
		}
		return dwLen;
	}
};
