#ifndef _LIST_H
#define _LIST_H

#include "MemoryManager.h"
#include "Utils.h"
#include "Consts.h"
#include "Errors.h"

typedef struct TListRec  *List;

typedef struct TListRec 
{
      MMgr         oMMgr;
      Error        oError;
      unsigned int nBlockSize;
      unsigned int nItemsPerBlock;
      unsigned int nCount;
      void       **ppObj;
} ListRec;


/*
 *  Resize
 *
 *  pList  : ��������� �� ������ List.
 *  nCount : ������ ������� ����������.
 *
 *  return:  success, returns OK.
 *           otherwise, returns error-code, ������� ������� � MMgr �������.
 *
 */

unsigned long Resize(List pList, unsigned int nCount)
{
    void **ppNewObj;

	if ( pList->nCount >= nCount) 
	{
		if ( pList->nCount == nCount )
            return OK;
        else
            return AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER;
    }

	ppNewObj = (void **)GetMem( pList->oMMgr, nCount * sizeof(void *) );

    if ( !ppNewObj )
		return ErrorGetCode( pList->oError );

	if ( pList->ppObj )
		UtilsMemCpy( (BYTE *)ppNewObj, (BYTE *)pList->ppObj, pList->nBlockSize * sizeof(void *));

	pList->nBlockSize = nCount;
    
	if ( pList->ppObj )
		FreeMem( pList->oMMgr, pList->ppObj );
    
	pList->ppObj = ppNewObj;

    return OK;
}



/*
 *  ListNew
 *
 *  oMMgr          :  ��������� ������� MMgr.
 *  nItemsPerBlock :  ���������� ����������.
 *
 *  return:  success, ���������� handle �� ����� List ������
 *           otherwise, ���������� NULL.
 *
 */

List ListNew(MMgr oMMgr, unsigned int nItemsPerBlock)
{
    List pList;

    if ( NULL == oMMgr )
        return NULL;

    pList = (List)GetMem( oMMgr, sizeof(ListRec) );
    if ( pList ) 
	{
		pList->oMMgr  = oMMgr;
		pList->oError = oMMgr->oError;
		pList->nBlockSize = 0;
		pList->nItemsPerBlock = (nItemsPerBlock <= 0 ? DEF_ITEMS_PER_BLOCK : nItemsPerBlock);
		pList->nCount = 0;
		pList->ppObj = NULL;
    }

    return pList;
}

/*
 *  ListAdd
 *
 *  pList :  ��������� �� ������ List.
 *  pItem :  ��������� �� ������, ������� ����� ��������.
 *
 *  return:  success, returns OK.
 *           ���������� AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, ����� ���������� ������� List ����������
 *
 */

unsigned long ListAdd(List pList, void *pItem)
{

	if ( pList->nCount >= pList->nBlockSize)
	{
		unsigned long nRet = Resize( pList, pList->nBlockSize + pList->nItemsPerBlock);

        if ( OK != nRet ) 
		{
            return nRet;
        }
    }

	pList->ppObj[pList->nCount++] = pItem;
    return OK;
}

/*
 *  ListFind
 *
 *  pList  : ��������� �� ������ List.
 *  nCount : ������ ������� ����������.
 *
 *  return:  success, ��������� ����� �������.
 *           otherwise, ���������� ������������� ��������.
 *
 */

int ListFind  (List pList, void *pItem)
{
    unsigned int nIndex;

	for ( nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
		if ( pList->ppObj[nIndex] == pItem )
            return nIndex;
    }

    return -1;
}


/*
 *  ListInsert
 *
 *  pList   :  ��������� �� ������ List.
 *  pTarget :  ��������� �� ������, ������� �������.
 *  pItem   :  ��������� �� ������, ������� ��������� � ������.
 *
 *  ��������� ����� ������ pItem ����� pTarget � ������.
 *
 *  return:  success, return OK.
 *           ���������� AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM, ����� ���������� ������� List ����������
 *           ���������� AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND, ����� ������ ������� ������� �� ������
 *
 */

unsigned long ListInsert(List pList, void *pTarget, void *pItem)
{
    int  nTargetIndex = ListFind( pList, pTarget );
	void *pLastItem = pList->ppObj[pList->nCount - 1];
    
    if ( nTargetIndex < 0 )
        return AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND;

    // ������� ������� ������ ����� �� �������
	for ( int nIndex  = pList->nCount - 2; nIndex >= nTargetIndex; nIndex-- )
		pList->ppObj[nIndex + 1] = pList->ppObj[nIndex];

	pList->ppObj[ nTargetIndex ] = pItem;

    return ListAdd( pList, pLastItem);
}

/*
 *  ListRemoveByIndex
 *
 *  ������� ������ �� ������, ������������ ������� � ������.
 *
 *  pList  :  ��������� �� ������ List.
 *  nIndex :  ����� �������, ������� ����� �������.
 *
 *  return:  success, ���������� ��������� �� ��������� �� ������ ������.
 *           ���������� NULL, ���� ������ �� ������ � ������.
 *
 */

void* ListRemoveByIndex( List pList, unsigned int nIndex )
{
    void *pTmp = NULL;

	if ( pList->nCount <= nIndex)
        return NULL;

	pTmp = pList->ppObj[nIndex];

	while ( nIndex < pList->nCount - 1)
	{
		pList->ppObj[nIndex] = pList->ppObj[nIndex + 1];
        nIndex++;
    }

	pList->nCount--;

    return pTmp;
}


/*
 *  ListRemove
 *
 *  ������� ������ �� ������, ������������ ���������� pItem.
 *  ������� ������, ���������� ��������, �� �������������.
 *
 *  pList :  ��������� �� ������ List.
 *  pItem :  ��������� �� ������, ������� �������.
 *
 *  return:  success, return OK.
 *           ���������� AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND, ���� ������ �� ������ � ������.
 *
 */

unsigned long ListRemove( List pList, void *pItem )
{
	void **ppObj = pList->ppObj;

	for (unsigned int nIndex = 0; nIndex < pList->nCount; nIndex++) 
	{
        if ( *ppObj == pItem) 
		{
            ListRemoveByIndex( pList, nIndex );
            return OK;
        } 
		else
            ppObj++;
    }

    return AVS_OFFICEPDFWRITER_ERROR_ITEM_NOT_FOUND;
}

/*
 *  ListItemAt
 *
 *  pList  :  ��������� �� ������ List.
 *  pIndex :  ����� �������.
 *
 *  return:  success, ���������� ��������� �� ������.
 *           otherwise, ���������� NULL.
 *
 */

__forceinline void* ListItemAt( List pList, unsigned int nIndex)
{
	return ( pList->nCount <= nIndex ) ? NULL : pList->ppObj[nIndex];
}


/*
 *  ListClear
 *
 *  pList :  ��������� �� ������ List.
 *
 */

void ListClear( List pList )
{
    if ( pList->ppObj )
        FreeMem( pList->oMMgr, pList->ppObj );

    pList->nBlockSize = 0;
    pList->nCount = 0;
    pList->ppObj = NULL;
}

/*
 * Listfree
 * 
 * ������� ��� ������ � ��� ������� ��� �������������.
 *
 * pList :  ��������� �� ������ List.
 *
 */

void ListFree( List pList )
{
    if ( !pList )
        return ;

    ListClear( pList );
	FreeMem( pList->oMMgr, pList );
}


/*
 * Listfree2
 * 
 * ������� ��� ������, �� ��� ������� ��� �������������
 * ��������� � ������.
 *
 * pList :  ��������� �� ������ List.
 *
 */

void ListFree2( List pList )
{
    if ( !pList )
        return ;

	FreeMem( pList->oMMgr, pList );
}



#endif /* _LIST_H */

