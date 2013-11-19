#pragma once
#include "File.h"
#include "Enums.h"

using namespace NSAttributes;

enum ElementType
{
	etVideo		 = 0,
	etAudio		 = 1,
	etPicture    = 2,
	etShape      = 3,
	etText		 = 4
};

// ��� �����, �������������� ��� �������� ������� ��������,
// �������� - ��������� �� ��������... (�� PID'�)
class CProperty
{
public:
	NSOfficeDrawing::PropertyId m_ePID;
	bool m_bIsBlip;
	bool m_bComplex;

	// ���� ��������
	DWORD m_lValue;

	// ����� �� ������� ������ - ��� value, ����� m_bComplex == true
	BYTE* m_pOptions;

	BOOL m_bIsTruncated;

public:
	CProperty()
	{
		m_ePID = NSOfficeDrawing::left;
		m_bIsBlip = false;
		m_bComplex = false;
		m_lValue = 0;
		m_pOptions = NULL;

		m_bIsTruncated = FALSE;
	}
	~CProperty()
	{
		RELEASEARRAYOBJECTS(m_pOptions);
	}

	void FromStream(IStream* pStream)
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

	void ComplexFromStream(IStream* pStream)
	{
		if (m_bComplex && 0 != m_lValue)
		{
			if (NSOfficeDrawing::dgmConstrainBounds == m_ePID ||
				NSOfficeDrawing::fillShadeColors == m_ePID ||
				NSOfficeDrawing::lineDashStyle == m_ePID ||
				NSOfficeDrawing::pAdjustHandles == m_ePID ||
				NSOfficeDrawing::pConnectionSites == m_ePID ||
				NSOfficeDrawing::pConnectionSitesDir == m_ePID ||
				NSOfficeDrawing::pInscribe == m_ePID ||
				NSOfficeDrawing::pSegmentInfo == m_ePID ||
				NSOfficeDrawing::pVertices == m_ePID ||
				NSOfficeDrawing::pGuides == m_ePID ||
				NSOfficeDrawing::pWrapPolygonVertices == m_ePID ||
				NSOfficeDrawing::pRelationTbl == m_ePID ||
				NSOfficeDrawing::tableRowProperties == m_ePID ||
				NSOfficeDrawing::lineLeftDashStyle == m_ePID ||
				NSOfficeDrawing::lineTopDashStyle == m_ePID ||
				NSOfficeDrawing::lineRightDashStyle == m_ePID ||
				NSOfficeDrawing::lineBottomDashStyle == m_ePID)
			{
				WORD nElems = StreamUtils::ReadWORD(pStream);
				WORD nElemsAlloc = StreamUtils::ReadWORD(pStream);
				WORD nElemSize = StreamUtils::ReadWORD(pStream);

				if (0xFFF0 == nElemSize)
				{
					nElemSize = 4;
					m_bIsTruncated = TRUE;
				}

				LONG dwSize = nElems * nElemSize;			

				if (m_lValue != (dwSize + 6))
				{
					BOOL b = FALSE;
				}

				m_lValue = dwSize;
			}
			
			if (0 == m_lValue)
			{
				return;
			}

			RELEASEARRAYOBJECTS(m_pOptions);
			m_pOptions = new BYTE[m_lValue];

			ULONG lReadBytes = 0;
			pStream->Read(m_pOptions, m_lValue, &lReadBytes);
			if (lReadBytes != m_lValue)
			{
				return;
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
	CAtlArray<CProperty> m_arProperties;
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
		m_arProperties.RemoveAll();
	}
	
	void FromStream(IStream* pStream, long lCount)
	{
		m_lCount = lCount;
		for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
		{
			m_arProperties.Add();
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
		for (size_t nIndex = 0; nIndex < m_arProperties.GetCount(); ++nIndex)
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

// �����, ������� ��� �� �����������
class CInteractiveInfo
{
public:
	class CTextRange
	{
	public:
		LONG m_lStart;
		LONG m_lEnd;
	
	public:
		CTextRange()
		{
			m_lStart	= 0;
			m_lEnd		= 0;
		}
		CTextRange(const CTextRange& oSrc)
		{
			*this = oSrc;
		}
		CTextRange& operator=(const CTextRange& oSrc)
		{
			m_lStart	= oSrc.m_lStart;
			m_lEnd		= oSrc.m_lEnd;

			return *this;
		}
	};

private:
	long m_lType;
	
public:
	bool m_bPresent;

	CAtlArray<CTextRange> m_arRanges;

public:
	CInteractiveInfo()
	{
		m_bPresent = false;
	}
	~CInteractiveInfo()
	{
	}
};

class CAnimationInfo
{
private:
	long m_lType;

public:
	CAnimationInfo()
	{
	}
	~CAnimationInfo()
	{
	}
};