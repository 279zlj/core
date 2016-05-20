#pragma once

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

/*
	�� �������� (����� ������������� � ������������ CGdiPlusInit()) ������� 
	�� ����������, ��������� ����������� ���������� �� InitInstance ����������
	� GDI+ ������� ���� �����-�� ���� ���������, � DDE ��������� ������� ����
	������ ���� ����� ��������� ���� �������� MainFrame. � ����� ��� ���� 
	��������� ������ �� ���������� ������� ������, � �� ������ ��������� 
	���������� ��� �� ���� ���������, ��������� ���� � ���� �� ��������.
	������� �������� �������� ��������� Init() � �������� �� ����� ���� ���
	MainFrame ��� ������.

	�����!!!! �� ���� ������������ �� �������, ���� ���� ������������ 3 ����.
*/

class CGdiPlusInit  
{
public:

	CGdiPlusInit()
	{
		m_bPresent = FALSE;
		m_nToken = 0;
	}
	virtual ~CGdiPlusInit()
	{
		if (m_bPresent) 
			Gdiplus::GdiplusShutdown(m_nToken);
	}
	BOOL Init()
	{
		Gdiplus::GdiplusStartupInput pInput;

		try
		{
			Gdiplus::GdiplusStartup(&m_nToken, &pInput, 0);

			m_bPresent = TRUE;
		}
		catch (...)
		{
			m_bPresent = FALSE;
		}

		return m_bPresent;
	}
	BOOL Good()
	{
		return m_bPresent;
	}
	BOOL IsValid()
	{
		return m_bPresent;
	}

private:

	BOOL m_bPresent;
	ULONG_PTR m_nToken;
};
