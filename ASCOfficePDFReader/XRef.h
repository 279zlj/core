#ifndef _XREF_H
#define _XREF_H

#include "Object.h"

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

	BOOL CheckValidate() 
	{ 
		return m_bValidXref; 
	}

	// ���� CheckValidate ������ FALSE
	int GetErrorCode() 
	{ 
		return m_nErrorCode; 
	}

	void SetEncryption(int nPermissionFlags, BOOL bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm);

	BOOL CheckEncrypted() 
	{ 
		return m_bEncrypted; 
	}

	// ��������� �����������.
	BOOL CheckPrint(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckChange(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckCopy(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckAddNotes(BOOL bIgnoreOwnerPassword = FALSE);

	Object *GetCatalog(Object *pObject) 
	{ 
		return Fetch( m_nRootNum, m_nRootGen, pObject); 
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
	// ���������� FALSE, ���� ������� ���������� ��� ���� �� ���������.
	BOOL GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd);

	int GetSize() 
	{ 
		return m_nEntrySize; 
	}
	XRefEntry *GetEntry(int nIndex) 
	{ 
		return &m_arrEntries[ nIndex ]; 
	}
	Object *GetTrailerDict() 
	{ 
		return &m_oTrailerDict; 
	}

private:

	unsigned int GetStartXref();
	BOOL ReadXRef(unsigned int *punPos);
	BOOL ReadXRefTable(Parser *pParser, unsigned int *punPos);
	BOOL ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount);
	BOOL ReadXRefStream(Stream *pXrefStream, unsigned int *punPos);
	BOOL ConstructXRef();
	unsigned int StrintToUInt(char *sString);

private:

	BaseStream    *m_pStream;           // �������� �����
	unsigned int   m_nStart;            // ����� � ������ 

	XRefEntry     *m_arrEntries;        // �������� ������� Xref
	int            m_nEntrySize;        // ������ �������� � ������ m_arrEntries
	int            m_nRootNum;          // ����� ������� Root (Catalog)
	int            m_nRootGen;          // ����� ������ ������� Root (Catalog)

	BOOL           m_bValidXref;        // ��������� ������������ ������� Xref
	int            m_nErrorCode;        // ����� ������, ���� m_bValidXref = FALSE

	Object         m_oTrailerDict;      // ������� Trailer

	unsigned int   m_unLastXRefOffset;  // ����� ��������� ������� Xref
	unsigned int  *m_punStreamEnds;     // ������� ����� ������ - ������������ ������ ��� ������������ ������

	int            m_nStreamEndsCount;  // ���������� ���������� ��������� � m_punStreamEnds
	ObjectStream  *m_pObjectStream;     // Object Stream
	BOOL           m_bEncrypted;        // ����� ���������� ��� ���?
	int            m_nPermissionFlags;  // ��������� �����������
	BOOL           m_bOwnerPassword;    // ��������� ���������� �� ��� ������ ������ ��������� �����
	unsigned char  m_arrDecryptKey[16]; // ���� ��� �����������
	int            m_nKeyLength;        // ������ ����� � ������
	int            m_nEncryptVersion;   // ������ ��������
	CryptAlgorithm m_eEncryptAlgorithm;	// �������� ����������

	CRITICAL_SECTION m_oCS;
};

#endif /* _XREF_H */
