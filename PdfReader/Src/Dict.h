#ifndef _PDF_READER_DICT_H
#define _PDF_READER_DICT_H

#include "Object.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Dict
	//------------------------------------------------------------------------
	struct DictEntry
	{
		char  *sKey;
		Object oValue;
	};

	class Dict
	{
	public:

		Dict(XRef *pXref);

		~Dict();

		// ������� ������.
		int AddRef()
		{
			return ++m_nRef;
		}
		int Release()
		{
			return --m_nRef;
		}


		int GetEntryCount()
		{
			return m_nEntriesCount;
		}

		// ��������� ������� � �������. ��� ���� ��� �� ����������.
		void AddItem(char *sKey, Object *pValue);

		// ��������� ��� ������� �� ����� sType.
		bool CheckType(char *sType);

		// ���� ������� �������. ������������ ��������� �� ������.
		Object *Search(char *sKey, Object *pObject);
		// ���� ������� �������. ������������ ����� �������.
		Object *SearchAndCopy(char *sKey, Object *pObject);


		char   *GetKey(int nIndex);
		Object *GetValue(int nIndex, Object *pObj);
		Object *GetValueCopy(int nIndex, Object *pObj);

		// ������������� ��������� �� ������� Xref. ������ ������� ������������
		// ������ � ����� ������: ������� Trailer �������� �� ������� Xref, �������
		// ��� ������� ������� ���� ��������� ����� �������������.
		void SetXRef(XRef *pXref)
		{
			m_pXref = pXref;
		}

	private:

		DictEntry *Find(char *sKey);

	private:

		XRef      *m_pXref;         // ������� Xref �������� PDF �����
		DictEntry *m_arrEntries;    // ��������� �������
		int        m_nEntriesSize;  // ������ �������� � �������
		int        m_nEntriesCount; // ���������� ��������� � �������

		int        m_nRef;          // ������� ������
	};
}

#endif // _PDF_READER_DICT_H
