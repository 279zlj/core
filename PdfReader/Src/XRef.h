#ifndef _PDF_READER_XREF_H
#define _PDF_READER_XREF_H

#include "Object.h"
#include "Stream.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "ErrorConstants.h"

namespace PdfReader
{
	class Dict;
	class Stream;
	class Parser;
	class ObjectStream;

	//------------------------------------------------------------------------
	// XRef
	//------------------------------------------------------------------------

	enum XRefEntryType
	{
		xrefEntryFree,
		xrefEntryUncompressed,
		xrefEntryCompressed
	};

	struct XRefEntry
	{
		unsigned int  unOffset;
		int           nGen;
		XRefEntryType eType;
	};

	class XRef
	{
	public:

		XRef(BaseStream *pStream);

		~XRef();

		bool CheckValidate()
		{
			return m_bValidXref;
		}

		// ���� CheckValidate ������ false
		EError GetErrorCode()
		{
			return m_eErrorCode;
		}

		void SetEncryption(int nPermissionFlags, bool bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm);

		bool CheckEncrypted()
		{
			return m_bEncrypted;
		}

		// ��������� �����������.
		bool CheckPrint(bool bIgnoreOwnerPassword = false);
		bool CheckChange(bool bIgnoreOwnerPassword = false);
		bool CheckCopy(bool bIgnoreOwnerPassword = false);
		bool CheckAddNotes(bool bIgnoreOwnerPassword = false);

		Object *GetCatalog(Object *pObject)
		{
			return Fetch(m_nRootNum, m_nRootGen, pObject);
		}

		// ����������� ��������� ������.
		Object *Fetch(int nNum, int nGen, Object *pObject);

		Object *GetDocInfo(Object *pObject);
		Object *GetDocInfoCopy(Object *pObject);

		int GetObjectsCount()
		{
			return m_nEntrySize;
		}

		unsigned int GetLastXRefPos()
		{
			return m_unLastXRefOffset;
		}


		// ������ Root (Catalog)
		int GetRootNum()
		{
			return m_nRootNum;
		}
		int GetRootGen()
		{
			return m_nRootGen;
		}

		// �������� �������� ������� � ������������ �����.
		// ���������� false, ���� ������� ���������� ��� ���� �� ���������.
		bool GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd);

		int GetSize()
		{
			return m_nEntrySize;
		}
		XRefEntry *GetEntry(int nIndex)
		{
			return &m_arrEntries[nIndex];
		}
		Object *GetTrailerDict()
		{
			return &m_oTrailerDict;
		}

	private:

		unsigned int GetStartXref();
		bool ReadXRef(unsigned int *punPos);
		bool ReadXRefTable(Parser *pParser, unsigned int *punPos);
		bool ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount);
		bool ReadXRefStream(Stream *pXrefStream, unsigned int *punPos);
		bool ConstructXRef();
		unsigned int StrintToUInt(char *sString);

	private:

		BaseStream    *m_pStream;           // �������� �����
		unsigned int   m_nStart;            // ����� � ������ 

		XRefEntry     *m_arrEntries;        // �������� ������� Xref
		int            m_nEntrySize;        // ������ �������� � ������ m_arrEntries
		int            m_nRootNum;          // ����� ������� Root (Catalog)
		int            m_nRootGen;          // ����� ������ ������� Root (Catalog)

		bool           m_bValidXref;        // ��������� ������������ ������� Xref
		EError         m_eErrorCode;        // ����� ������, ���� m_bValidXref = false

		Object         m_oTrailerDict;      // ������� Trailer

		unsigned int   m_unLastXRefOffset;  // ����� ��������� ������� Xref
		unsigned int  *m_punStreamEnds;     // ������� ����� ������ - ������������ ������ ��� ������������ ������

		int            m_nStreamEndsCount;  // ���������� ���������� ��������� � m_punStreamEnds
		ObjectStream  *m_pObjectStream;     // Object Stream
		bool           m_bEncrypted;        // ����� ���������� ��� ���?
		int            m_nPermissionFlags;  // ��������� �����������
		bool           m_bOwnerPassword;    // ��������� ���������� �� ��� ������ ������ ��������� �����
		unsigned char  m_arrDecryptKey[16]; // ���� ��� �����������
		int            m_nKeyLength;        // ������ ����� � ������
		int            m_nEncryptVersion;   // ������ ��������
		CryptAlgorithm m_eEncryptAlgorithm;	// �������� ����������

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};
}

#endif // _PDF_READER_XREF_H
