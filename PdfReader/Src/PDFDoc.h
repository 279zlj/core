#ifndef _PDF_READER_PDF_DOC_H
#define _PDF_READER_PDF_DOC_H

#include <stdio.h>
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "ErrorConstants.h"

namespace PdfReader
{
	class StringExt;
	class BaseStream;
	class OutputDev;
	class Links;
	class LinkAction;
	class LinkDest;
	class Outline;
	//-------------------------------------------------------------------------------------------------------------------------------
	// PDFDoc
	//-------------------------------------------------------------------------------------------------------------------------------

	class PDFDoc
	{
	public:

		PDFDoc(GlobalParams *pGlobalParams, const wchar_t* wsFileName, StringExt *seOwnerPassword = NULL, StringExt *seUserPassword = NULL);
		~PDFDoc();

		// ��������� �� �������� PDF ����?
		bool CheckValidation()
		{
			return m_bValid;
		}


		// �������� ��� ������, ���� PDF ���� �� ��������.
		EError GetErrorCode()
		{
			return m_eErrorCode;
		}


		// ��������� ��� �����.
		std::wstring GetFileName()
		{
			return m_wsFileName;
		}


		// ��������� ������� Xref.
		XRef *GetXRef()
		{
			return m_pXref;
		}


		// ��������� ������ �atalog.
		Catalog *GetCatalog()
		{
			return m_pCatalog;
		}


		// ��������� �����.
		BaseStream *GetBaseStream()
		{
			return m_pStream;
		}


		// C�������� ��������� ��������.
		double GetPageMediaWidth(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetMediaWidth();
		}
		double GetPageMediaHeight(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetMediaHeight();
		}
		double GetPageCropWidth(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetCropWidth();
		}
		double GetPageCropHeight(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetCropHeight();
		}
		int    GetPageRotate(int nPageIndex)
		{
			return m_pCatalog->GetPage(nPageIndex)->GetRotate();
		}


		// ��������� ���������� �������.
		int GetPagesCount()
		{
			return m_pCatalog->GetPagesCount();
		}


		// ���������� ���������� ������ ���������� ������� Catalog.
		StringExt *ReadMetadata()
		{
			return m_pCatalog->ReadMetadata();
		}


		// ���������� ������ StructureTreeRoot.
		Object *GetStructTreeRoot()
		{
			return m_pCatalog->GetStructTreeRoot();
		}


		// ������� ������ �������� �� ���������� pOut.
		void DisplayPage(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// ������� ����� ��������� �������.
		void DisplayPages(OutputDev *pOut, int nFirstPageIndex, int nLastPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// ������� ����� ��������.
		void DisplayPageSlice(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool(*pAbortCheckCbk)(void *pData) = NULL, void *pAbortCheckCbkData = NULL);
		// ���� �������� �� �� ��������� �������. ���������� 0, ���� �������� �� �������.
		int FindPage(int nNum, int nGen)
		{
			return m_pCatalog->FindPage(nNum, nGen);
		}


		// ���������� Links ��� ������� ��������.
		Links *GetLinks(int nPageIndex);
		// ���� Destination �� �����. ���������� Link destination ��� NULL, ���� <seName> �� ������ Destination.
		LinkDestination *FindDest(StringExt *seName)
		{
			return m_pCatalog->FindDest(seName);
		}


		// ������������ Links ��� ������ ��������.
		void ProcessLinks(OutputDev *pOut, int nPageIndex);
		// ���������� ������ �utline.
		Outline *GetOutline()
		{
			return m_pOutline;
		}
		// ���������� �� ����?
		bool IsEncrypted()
		{
			return m_pXref->CheckEncrypted();
		}

		// ��������� ��������� �����������.
		bool CheckPrint(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckPrint(bIgnoreOwnerPassword);
		}
		bool CheckChange(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckChange(bIgnoreOwnerPassword);
		}
		bool CheckCopy(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckCopy(bIgnoreOwnerPassword);
		}
		bool CheckAddNotes(bool bIgnoreOwnerPassword = false)
		{
			return m_pXref->CheckAddNotes(bIgnoreOwnerPassword);
		}

		// ������ PDF ���� �������� ����������������� (��. Linearized PDF)
		bool IsLinearized();
		// ����������� ����� Info.
		Object *GetDocInfo(Object *pObject)
		{
			return m_pXref->GetDocInfo(pObject);
		}
		Object *GetDocInfoCopy(Object *pObject)
		{
			return m_pXref->GetDocInfoCopy(pObject);
		}

		// ���������� ������ PDF �����.
		double GetPDFVersion()
		{
			return m_dPDFVersion;
		}

	private:

		bool Setup(StringExt *seOwnerPassword, StringExt *seUserPassword);
		void CheckHeader();
		bool CheckEncryption(StringExt *seOwnerPassword, StringExt *seUserPassword);

	private:

		std::wstring   m_wsFileName;  // ��� ��������� �����.
		FILE*          m_pFile;       // ��������� �� �������� �����
		unsigned char* m_pFileBuffer; // ����� ��� �����

		BaseStream*    m_pStream;     // �����, � ������� ������ ���� ����
		double         m_dPDFVersion; // ������� PDF ����� (1.1 - 1.7)
		XRef*          m_pXref;       // ������� Xref 
		Catalog*       m_pCatalog;    // ��������� �� ������ Catalog
		Outline*       m_pOutline;    // ��������� �� Outline (����)
					   
		bool           m_bValid;      // ��������� �� �������� PDF ����
		EError         m_eErrorCode;  // ��� ������
					   
		GlobalParams*  m_pGlobalParams;
	};
}
#endif // _PDF_READER_PDF_DOC_H
