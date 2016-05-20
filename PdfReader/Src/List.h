#ifndef _PDF_READER_LIST_H
#define _PDF_READER_LIST_H

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CList
	//------------------------------------------------------------------------

	class CList
	{

	public:

		// ������� ������ ������.
		CList();

		// ������� ������ ������ � � ���������� ������� ��� <nSize> ���������.
		CList(int nSize);

		// ����������.
		~CList();

		int GetLength()
		{
			return m_nCount;
		}

		// ���������� <nIndex>�� �������.
		// ���� 0 <= nIndex < m_nCount, ���������� NULL.
		void *GetByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nCount)
				return NULL;
			return m_ppData[nIndex];
		}

		// ��������� ������� � ����� ������.
		void Append(void *pItem);

		// ��������� ������ ������ � ����� �������.
		void Append(CList *pList);

		// ��������� ������� �� ����� <nIndex>.
		// ���� !(0 <= nIndex <= m_nCount), ������ �� ������.
		void Insert(int nIndex, void *pItem);

		// ������� �� ������ � ���������� ������ �� �������.
		// ���� !(0 <= nIndex <= m_nCount), ������ �� ������.
		void *Delete(int nIndex);

		// ��������� ������, � ������������ � ������ ��������
		// ���������.
		void Sort(int(*CompareFunc)(const void *pItem1, const void *pItem2));

		// ���� m_nIncreament > 0, ����� ��� ���������� ������ �����
		// m_nIncreament ��������� ����� �����������. ���� m_nIncreament <= 0,
		// ����� ������ ����� ��������� ��� ����������.
		void SetAllocationIncreament(int nIncreament)
		{
			m_nIncreament = nIncreament;
		}

	private:

		void Expand();
		void Shrink();

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
        delete (T*)_list->GetByIndex(_i);           \
      }                                             \
      delete _list;                                 \
    }                                               \
  } while (0)

}
#endif // _PDF_READER_LIST_H
