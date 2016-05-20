#ifndef _PDF_READER_DECRYPT_H
#define _PDF_READER_DECRYPT_H

#include "StringExt.h"
#include "Object.h"
#include "Stream.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Decrypt
	//-------------------------------------------------------------------------------------------------------------------------------

	class Decrypt
	{
	public:

		// ������ FileKey. �������� <sFileKey> ������ ����� ����� ��� ������� ��� 16 ����. ��������� <seOwnerPassword> � <seUserPassword>
		// � ���������� true, ���� �����-������ ���������. ����� <pbOwnerPasswordValide> = true, ���� <seOwnerPassword> ��� ����������.  
		// ���� �����-������ ��� ��� ������ ����� ���� NULL, ��� �� ����� �������� ��� ������ ������.
		static bool MakeFileKey(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seOwnerPassword, StringExt *seUserPassword, unsigned char *sFileKey, bool bEncryptMetadata, bool *pbOwnerPasswordValid);

	private:

		static bool MakeFileKey2(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seUserPassword, unsigned char *sFileKey, bool bEncryptMetadata);
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// DecryptStream
	//-------------------------------------------------------------------------------------------------------------------------------

	struct DecryptRC4State
	{
		unsigned char sState[256];
		unsigned char unX;
		unsigned char unY;
		int           nBuffer;
	};

	struct DecryptAESState
	{
		unsigned int  arrW[44];
		unsigned char sState[16];
		unsigned char sCBC[16];
		unsigned char sBuffer[16];
		int           nBufferIndex;
	};

	class DecryptStream : public FilterStream
	{

	public:

		DecryptStream(Stream *pStream, unsigned char *sFileKey, CryptAlgorithm eType, int nKeyLength, int nObjectNum, int nObjectGen);
		virtual ~DecryptStream();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual bool IsBinary(bool bLast);
		virtual Stream *getUndecodedStream() { return this; }

	private:

		CryptAlgorithm m_eCryptType;
		int            m_nObjectKeyLength;
		unsigned char  m_sObjectKey[16 + 9];

		union
		{
			DecryptRC4State oRC4;
			DecryptAESState oAES;
		} m_oState;
	};
}

#endif // _PDF_READER_DECRYPT_H
