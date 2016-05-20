#ifndef _PDF_READER_CATALOG_H
#define _PDF_READER_CATALOG_H

#include "GlobalParams.h"

namespace PdfReader
{
	class XRef;
	class Object;
	class Page;
	class PageAttrs;
	struct Ref;
	class LinkDestination;

	//------------------------------------------------------------------------
	// Catalog
	//------------------------------------------------------------------------

	class Catalog
	{
	public:

		Catalog(GlobalParams *pGlobalParams, XRef *pXref);

		~Catalog();

		bool CheckValidation()
		{
			return m_bValid;
		}

		int GetPagesCount()
		{
			return m_nPagesCount;
		}

		Page *GetPage(int nIndex)
		{
			return m_ppPages[nIndex - 1];
		}

		Ref *GetPageRef(int nIndex)
		{
			return &m_pPageRefs[nIndex - 1];
		}

		StringExt *GetBaseURI()
		{
			return m_seBaseURI;
		}

		StringExt *ReadMetadata();

		Object *GetStructTreeRoot()
		{
			return &m_oStructTreeRoot;
		}

		// ���� ����� �������� � ������, �� �� ��������� �������.
		int FindPage(int nNum, int nGen);

		// ���� Named destination.
		LinkDestination *FindDest(StringExt *seName);

		Object *GetDests()
		{
			return &m_oDests;
		}

		Object *GetNameTree()
		{
			return &m_oNameTree;
		}

		Object *GetOutline()
		{
			return &m_oOutline;
		}

		Object *GetAcroForm()
		{
			return &m_oAcroForm;
		}

	private:

		int ReadPageTree(Dict *pPages, PageAttrs *pAttrs, int nStart, char *sAlreadyRead);
		Object *FindDestInTree(Object *pTree, StringExt *seName, Object *pObject);

	private:

		XRef      *m_pXref;           // ������� xref ��� ������� PDF �����
		Page     **m_ppPages;         // ������ �������
		Ref       *m_pPageRefs;       // ��������� ������ ���� �������
		int        m_nPagesCount;     // ����� �������
		int        m_nPagesSize;      // ������ ������� �������(� ������)
		Object     m_oDests;          // Destination dictionary
		Object     m_oNameTree;       // Name tree
		StringExt *m_seBaseURI;       // ��� ������ URI
		Object     m_oMetadata;       // Metadata
		Object     m_oStructTreeRoot; // ��������� �������
		Object     m_oOutline;        // Outline
		Object     m_oAcroForm;       // AcroForm

		bool       m_bValid;          // True, ���� �atalog - ���������

		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDF_READER_CATALOG_H
