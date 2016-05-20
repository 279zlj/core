#pragma once

#include "atldefine.h"

//------------------------------------------------------------------------------------------------------
// CBaseThread
//------------------------------------------------------------------------------------------------------
// ����������� �����, ������� ��������� ������ � �������
//------------------------------------------------------------------------------------------------------

class CBaseThread
{
public :

	CBaseThread ( DWORD_PTR dwAffinityMask = 0 )
		: m_bRunThread ( FALSE )
		, m_hThread ( NULL )
		, m_lPercents ( 0 )
		, m_lError ( NO_ERROR )
		, m_sError ( "" )
		, m_dwAffinityMask ( dwAffinityMask )
		, m_bSuspend ( FALSE )
	{
		if ( 0 == m_dwAffinityMask )
		{
			DWORD_PTR dwProcessAffinityMask = 0;
			DWORD_PTR dwSystemAffinityMask = 0;
			if ( GetProcessAffinityMask ( GetCurrentProcess (), &dwProcessAffinityMask, &dwSystemAffinityMask ) )
				m_dwAffinityMask = dwProcessAffinityMask;
		}
	}

	virtual ~CBaseThread ()
	{
		StopWork ();
	}
	virtual void StartWork ( LONG lPriority )
	{
		// ���� ����� ������� - �������
		if ( m_bRunThread )
			return;

		m_lPercents = 0;
		m_lError = NO_ERROR;
		m_sError = "";

		m_bSuspend = FALSE;
		StartThread ();

		SetThreadPriority ( m_hThread, lPriority );
		m_lThreadPriority = lPriority;
	}
	virtual void SuspendWork ()
	{
		m_bSuspend = TRUE;
	}
	virtual void ResumeWork ()
	{
		m_bSuspend = FALSE;
	}
	virtual void StopWork ()
	{
		if ( !m_bRunThread )
			return;

		m_bRunThread = FALSE;
		if ( NULL != m_hThread )
			WaitForSingleObject ( m_hThread, INFINITE );

		RELEASEHANDLE ( m_hThread );
	}
	virtual void WaitThread ()
	{
		if ( !m_bRunThread )
			return;

		if ( NULL != m_hThread )
			WaitForSingleObject ( m_hThread, INFINITE );

		m_bRunThread = FALSE;

		RELEASEHANDLE ( m_hThread );
	}

	BOOL IsSuspended () const
	{		
		return m_bSuspend;
	}
	BOOL IsRunned () const
	{		
		return m_bRunThread;
	}
	LONG GetPercents () const
	{
		return m_lPercents;
	}
	LONG GetError () const
	{
		return m_lError;
	}
	CString GetErrorString () const
	{
		return m_sError;
	}
	HANDLE GetHandle ()
	{
		return m_hThread;
	}
	LONG GetPriority () const
	{
		return GetThreadPriority ( m_hThread );
	}

protected :

	void StartThread ()
	{
		DWORD dwTmp;
		m_bRunThread = TRUE;
		m_hThread = CreateThread ( NULL, 0, &_ThreadProc, ( LPVOID ) this, 0, &dwTmp );
		SetThreadAffinityMask ( m_hThread, m_dwAffinityMask );
	}
	void CheckSuspend ()
	{
		// ���� ����� ������������� � �������, �� �������� �� 10 ��
		while ( m_bSuspend && m_bRunThread )
			Sleep ( 10 );
	}
	static DWORD WINAPI CBaseThread::_ThreadProc ( LPVOID pv )
	{
		CBaseThread *pThis = ( CBaseThread* ) pv;
		return pThis->ThreadProc ();
	}
	virtual DWORD ThreadProc () = 0;				

protected :

	HANDLE m_hThread;									// ��������� �� �����

	BOOL m_bRunThread;									// ������� �� �����
	BOOL m_bSuspend;									// ������������� �� �����

	DWORD_PTR m_dwAffinityMask;							// ����� ������

	LONG	m_lError;									// ������
	CString m_sError;									// �������� ������

	LONG m_lThreadPriority;								// ��������� ������
	LONG m_lPercents;									// ��������
};

