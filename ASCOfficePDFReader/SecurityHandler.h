#ifndef _SECURITY_HANDLER_H
#define _SECURITY_HANDLER_H

#include "Object.h"

class StringExt;
class PDFDoc;

//-------------------------------------------------------------------------------------------------------------------------------
// SecurityHandler
//-------------------------------------------------------------------------------------------------------------------------------

class SecurityHandler 
{
public:

	static SecurityHandler *Make(PDFDoc *pDocument, Object *pEncryptDict);

	SecurityHandler(PDFDoc *pDocument);
	virtual ~SecurityHandler();

	// ��������� ����������� �� ��������. ���� �������� �����������, ����� ������� ������� ������������ <seOwnerPassword> � 
	// <seUserPassword>. ���� ��� ������ �� ��������, ����� ����������� ������ � ������������ (�� ����� 3-� ���). 
	// ������������ ��������: 
	// True, ���� �������� ����� ���� ������( ���� �� ������ �� ����������, ���� ������ ������)
	// False, � ��������� ������ ( �������� ����������, � ������ ��������).
	BOOL CheckEncryption(StringExt *seOwnerPassword, StringExt *seUserPassword);

	// ��� "Batch mode", �.�. ����� ������ ������ � ����������� ��� ������������. 
	virtual void *MakeAuthData(StringExt *ownerPassword, StringExt *userPassword) = 0;

	// ��� ������� ������ �� ������������.
	virtual void *GetAuthData() = 0;

	// ����������� ������ ���������� ��������� MakeAuthData ��� GetAuthData.
	virtual void FreeAuthData(void *pAuthData) = 0;

	// �������� ��������������.
	virtual BOOL Authorize(void *pAuthData) = 0;

	// ��������� ��������� ���������.
	virtual int            GetPermissionFlags()   = 0;
	virtual BOOL           GetOwnerPasswordValid()= 0;
	virtual unsigned char *GetFileKey()           = 0;
	virtual int            GetFileKeyLength()     = 0;
	virtual int            GetEncodingVersion()   = 0;
	virtual CryptAlgorithm GetEncodingAlgorithm() = 0;

protected:

	PDFDoc *m_pDocument;
};

//-------------------------------------------------------------------------------------------------------------------------------
// StandardSecurityHandler
//-------------------------------------------------------------------------------------------------------------------------------

class StandardSecurityHandler: public SecurityHandler 
{
public:

	StandardSecurityHandler(PDFDoc *pDocument, Object *pEncryptDict);
	virtual ~StandardSecurityHandler();

	virtual void *MakeAuthData(StringExt *seOwnerPassword, StringExt *seUserPassword);
	virtual void *GetAuthData();
	virtual void FreeAuthData(void *pAuthData);
	virtual BOOL Authorize(void *pAuthData);
	virtual int GetPermissionFlags() 
	{ 
		return m_nPermissionFlags; 
	}
	virtual BOOL GetOwnerPasswordValid() 
	{ 
		return m_bOwnerPasswordValid; 
	}
	virtual unsigned char *GetFileKey() 
	{
		return m_sFileKey; 
	}
	virtual int GetFileKeyLength() 
	{ 
		return m_nFileKeyLength; 
	}
	virtual int GetEncodingVersion() 
	{ 
		return m_nEncryptVersion; 
	}
	virtual CryptAlgorithm GetEncodingAlgorithm() 
	{ 
		return m_eCryptType; 
	}

private:

	int            m_nPermissionFlags;
	BOOL           m_bOwnerPasswordValid;
	unsigned char  m_sFileKey[16];
	int            m_nFileKeyLength;
	int            m_nEncryptVersion;
	int            m_nEncryptRevision;
	CryptAlgorithm m_eCryptType;
	BOOL           m_bEncryptMetadata;

	StringExt     *m_seOwnerKey;
	StringExt     *m_seUserKey;
	StringExt     *m_seFileID;

	BOOL          m_bValid;
};

#endif /* _SECURITY_HANDLER_H */
