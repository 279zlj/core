#pragma once
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
// Byte input-output (Stream IO)
//-------------------------------------------------------------------------------------------------------------------------------

#define STREAM_READ	 0x0001 // ����� ������ ��� ������
#define STREAM_WRITE 0x0002 // ����� ������ ��� ������

#include "AtlFile.h"

typedef struct TByteIO 
{
	//PCommon        pInfo;   // ���������� � ������
	int            nMode;   // ������ ��� ������ ( STREAM_READ ��� OPJ_STREAM_WRITE )
	unsigned char *pBuffer; // ��������� �� ������ ������
	int            nLength; // ������ ������ � ������

	unsigned char *pStart;  // ��������� �� ������ ������
	unsigned char *pEnd;    // ��������� �� ����� ������
	unsigned char *pCurPos; // ��������� �� ������� ������� � ������
} ByteIO;


class CReader
{
public:
	virtual ~CReader(void){}

	virtual bool Open(void* sStream,long lSizeStream){return 0;}
	virtual void Close(){}
	
	virtual void  Read(BYTE* Data,long Size){}
	virtual DWORD Read(long size){return 0;}
	virtual void  Skip(long size){}
	virtual void  Seek(__int64 position){}
	virtual unsigned int  Write(unsigned int nValue, int nLen){return 0;}
	virtual int Tell(){return 0;}
	virtual int GetLeftSize(){return 0;}
	
	virtual void* GetOwner(){return 0;}
};

class CReaderStream:CReader
{
public:
	CReaderStream(void);	
	CReaderStream(void* sStream,long lSizeStream);
	virtual ~CReaderStream(void);

	bool Open(void* sStream,long lSizeStream);
	void Close();
	
	DWORD Read(long size);
	void  Skip(long size);
	void  Seek(__int64 position);
	unsigned int  Write(unsigned int nValue, int nLen);
	int Tell   ();
	int GetLeftSize();
	void* GetOwner();
	void  Read(BYTE* Data,long Size);

protected:
	ByteIO *m_oStream;	
	__int32 m_llPosition;
	long m_lSkiped;

	__int32 m_llSize;
	
};
#define SWAP_INT(x) ((x<<8) | (x>>8))
#define SWAP_INT1(x) {WORD y=((x<<8) | (x>>8));x=y;}
#define SWAP_LONG(x) ((x>>24) | (((x>>16)& 0xff)<<8) | (((x>>8)& 0xff)<<16) | ((x & 0xff)<<24))
#define SWAP_LONG1(x) {DWORD y=((x>>24) | (((x>>16)& 0xff)<<8) | (((x>>8)& 0xff)<<16) | ((x & 0xff)<<24));x=y;}

class CReaderFile:CReader
{
public:
	CReaderFile(void);	
	CReaderFile(CString sFileName,long lMode);
	virtual ~CReaderFile(void);

	bool Open(void* sStream,long lSizeStream);
	void Close();
	
	DWORD Read(long size);
	void  Skip(long size);
	void  Seek(__int64 position);
	unsigned int  Write(unsigned int nValue, int nLen);
	int Tell   ();
	int GetLeftSize();
	void* GetOwner();
	void  Read(BYTE* Data,long Size);

protected:
	CAtlFile m_oFile;	
	__int32 m_llPosition;
	long m_lSkiped;

	__int32 m_llSize;
	
};
