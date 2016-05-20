#ifndef _LIST_H
#define _LIST_H

//------------------------------------------------------------------------
// CList
//------------------------------------------------------------------------

class CList 
{

public:

	// ������� ������ ������.
	CList() 
	{
		m_nItemSize = 8;
		m_ppData = (void **)::malloc( m_nItemSize * sizeof(void*) );
		m_nCount = 0;
		m_nIncreament = 0;
	}

	// ������� ������ ������ � � ���������� ������� ��� <nSize> ���������.
	CList(int nSize) 
	{
		m_nItemSize = nSize;
		m_ppData = (void **)::malloc( m_nItemSize * sizeof(void*) );
		m_nCount = 0;
		m_nIncreament = 0;
	}


	// ����������.
    ~CList() 
	{
		if ( m_ppData )
			::free( m_ppData );
	}

	int GetLength() 
	{ 
		return m_nCount; 
	}

	// ���������� <nIndex>�� �������.
	// ���� 0 <= nIndex < m_nCount, ���������� NULL.
	void *GetByIndex(int nIndex) 
	{
		if ( nIndex < 0 || nIndex >= m_nCount )
			return NULL;
		return m_ppData[ nIndex ]; 
	}

	// ��������� ������� � ����� ������.
	void Append(void *pItem) 
	{
		if ( m_nCount >= m_nItemSize )
			Expand();
		m_ppData[m_nCount++] = pItem;
	}


	// ��������� ������ ������ � ����� �������.
	void Append(CList *pList) 
	{
		while ( m_nCount + pList->m_nCount > m_nItemSize )
			Expand();
		for (int nIndex = 0; nIndex < pList->m_nCount; ++nIndex )
			m_ppData[m_nCount++] = pList->m_ppData[ nIndex ];
	}


	// ��������� ������� �� ����� <nIndex>.
	// ���� !(0 <= nIndex <= m_nCount), ������ �� ������.
	void Insert(int nIndex, void *pItem) 
	{
		if ( 0 > nIndex  || nIndex > m_nCount )
			return;
		if ( m_nCount >= m_nItemSize ) 
			Expand();
		if ( nIndex < m_nCount ) 
			memmove( m_ppData + nIndex + 1, m_ppData + nIndex, ( m_nCount - nIndex ) * sizeof(void *));
		m_ppData[ nIndex ] = pItem;
		++m_nCount;
	}

	// ������� �� ������ � ���������� ������ �� �������.
	// ���� !(0 <= nIndex <= m_nCount), ������ �� ������.
	void *Delete(int nIndex) 
	{
		void *pItem = m_ppData[ nIndex ];
		if ( nIndex < m_nCount - 1 ) 
			memmove( m_ppData + nIndex, m_ppData + nIndex + 1, (m_nCount - nIndex - 1) * sizeof(void *));
		--m_nCount;
		if ( m_nItemSize - m_nCount >= ((m_nIncreament > 0) ? m_nIncreament : m_nItemSize / 2 ) )
			Shrink();
		return pItem;
	}

	// ��������� ������, � ������������ � ������ ��������
	// ���������.
	void Sort(int (*CompareFunc)(const void *pItem1, const void *pItem2 ) ) 
	{
		qsort( m_ppData, m_nCount, sizeof(void *), CompareFunc);
	}

	// ���� m_nIncreament > 0, ����� ��� ���������� ������ �����
	// m_nIncreament ��������� ����� �����������. ���� m_nIncreament <= 0,
	// ����� ������ ����� ��������� ��� ����������.
	void SetAllocationIncreament(int nIncreament) 
	{ 
		m_nIncreament = nIncreament; 
	}

private:

	void Expand() 
	{
		m_nItemSize += ( m_nIncreament > 0 ) ? m_nIncreament : m_nItemSize;
		m_ppData = (void **)::realloc( m_ppData, m_nItemSize * sizeof(void*) );
	}

	void Shrink() 
	{
		m_nItemSize -= ( m_nIncreament > 0 ) ? m_nIncreament : m_nItemSize / 2;
		m_ppData = (void **)::realloc( m_ppData, m_nItemSize * sizeof(void*) );
	}


private:

  void **m_ppData;      // ������ ���������
  int    m_nItemSize;   // ������ ������ � �������
  int    m_nCount;      // ���������� ��������� � ������
  int    m_nIncreament; // �� ������� ����� ����������� ������
};

#define DeleteCList(list, T)                        \
  do {                                              \
    CList *_list = (list);                          \
    {                                               \
      int _i;                                       \
      for (_i = 0; _i < _list->GetLength(); ++_i) { \
        delete (T*)_list->GetByIndex(_i);                  \
      }                                             \
      delete _list;                                 \
    }                                               \
  } while (0)

#endif /* _LIST_H */