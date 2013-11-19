#pragma once
#include "..\AVSOfficeFile\OfficeEventsTemplate.h"

template <class T, class Reciever>
class COfficeHtmlEvent : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeHtmlEvent(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgress( LONG nID, LONG nPercent )
	{
		m_pReciever->OnProgressHtml( nID, nPercent );
		return S_OK;
	}
	HRESULT OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel )
	{
		m_pReciever->OnProgressHtmlEx( nID, nPercent, Cancel );
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		VARIANTARG varg2;
		switch(dispIdMember) {
		case 1:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			OnProgress( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		case 2:
			VariantInit(&varg0);
			VariantInit(&varg1);
			VariantInit(&varg2);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			hr = DispGetParam(pDispParams,2,VT_I2,&varg2,puArgErr);		
			OnProgressEx( varg0.lVal, varg1.lVal, &varg2.iVal );
			if (pDispParams->rgvarg[0].vt & VT_BYREF)	// implemented now for last VT_I2 arg only
			{
				SHORT* p = (SHORT*)pDispParams->rgvarg[0].byref;
				if (p) *p = varg2.iVal;
			}
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};