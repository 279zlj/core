#pragma once

/**************************************************************
������ ��� ���������...
������� ��������� � �������
**************************************************************/
class CTextFullSettings;
struct SSlidePersist
{
	DWORD m_nPsrRef;
	DWORD m_nSlideID;

	CAtlArray<CTextFullSettings> m_arTextAttrs;

	SSlidePersist();
	SSlidePersist(const SSlidePersist& oSrc);
	CString ToString();
	SSlidePersist& operator =(const SSlidePersist& oSrc);
}; 