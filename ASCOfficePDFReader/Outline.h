#ifndef _OUTLINE_H
#define _OUTLINE_H

#include "Object.h"
#include "CharTypes.h"

class StringExt;
class GList;
class XRef;
class LinkAction;

//-------------------------------------------------------------------------------------------------------------------------------
// Outline
//-------------------------------------------------------------------------------------------------------------------------------

class Outline 
{
public:

	Outline(Object *pOutlineObject, XRef *pXref);
	~Outline();

	CList *GetItems() 
	{ 
		return m_pItems; 
	}

private:

	CList *m_pItems; // NULL, ���� � ��������� ��� Outline
					 // [OutlineItem]
};

//-------------------------------------------------------------------------------------------------------------------------------
// OutlineItem
//-------------------------------------------------------------------------------------------------------------------------------

class OutlineItem 
{
public:

	OutlineItem(Dict *pDict, XRef *pXref);
	~OutlineItem();

	static CList *ReadItemList(Object *pFirstItemRef, Object *pLastItemRef, XRef *pXref);

	void Open();
	void Close();

	Unicode *GetTitle() 
	{ 
		return m_pTitle; 
	}
	int GetTitleLength() 
	{ 
		return m_nTitleLen; 
	}
	LinkAction *GetAction() 
	{ 
		return m_pAction; 
	}
	BOOL IsOpen() 
	{ 
		return m_bOpen; 
	}
	BOOL HasKids() 
	{ 
		return m_oFirstRef.IsRef(); 
	}
	CList *GetKids() 
	{ 
		return m_pKids; 
	}

private:

	XRef       *m_pXref;          // ������� Xref ������� PDF-���������
	Unicode    *m_pTitle;         // �������� ������ ��������
	int         m_nTitleLen;      // ����� ��������
	LinkAction *m_pAction;        // LinkAction
	
	Object      m_oFirstRef;      // ������(Ref) �� First
	Object      m_oLastRef;       // ������(Ref) �� Last
	Object      m_oNextRef;       // ������(Ref) �� Next

	BOOL        m_bOpen;          // ����� �� ������� �������� ��� �������� ���������
	CList      *m_pKids;          // NULL, ���� ������ ����� �� ������ [OutlineItem]
};

#endif /* _OUTLINE_H */
