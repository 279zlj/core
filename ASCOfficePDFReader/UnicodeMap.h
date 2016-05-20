#ifndef _UNICODE_MAP_H
#define _UNICODE_MAP_H

#include "CharTypes.h"
#include "TemporaryCS.h"

class StringExt;

//-------------------------------------------------------------------------------------------------------------------------------

enum UnicodeMapType
{
	unicodeMapUser,      // ������ �� �����
	unicodeMapResident,  // ����������� ������
	unicodeMapFunc       // ��������� �� �������
};

typedef int (*UnicodeMapFunc)(Unicode nUnicode, char *sBuffer, int nBufferSize);

struct UnicodeMapRange 
{
	Unicode      nStart;      // �������� ��������� ��������
	Unicode      nEnd;        // 
	unsigned int unCode;      // ������ ���
	unsigned int nBytesCount; //
};

struct UnicodeMapExt;

//-------------------------------------------------------------------------------------------------------------------------------
// UnicodeMap
//-------------------------------------------------------------------------------------------------------------------------------

class UnicodeMap 
{
public:

	// ������� UnicodeMap �� ����� <seEncodingName>. ������������� ������� ������ ������ 1.
	static UnicodeMap *Parse(StringExt *seEncodingName, GlobalParams *pGlobalParams);

	// ������� Resident UnicodeMap.
	UnicodeMap(char *sEncodingName, BOOL bUnicodeOut, UnicodeMapRange *pRanges, int nLen);

	// ������� Resident UnicodeMap, ������� ���������� ������� ������ ��������� ��������.
	UnicodeMap(char *sEncodingName, BOOL bUnicodeOut, UnicodeMapFunc pFunction);

	~UnicodeMap();

	// ������� ������
	void AddRef();
	void Release();

	StringExt *GetEncodingName() 
	{ 
		return m_seEncodingName; 
	}

	BOOL IsUnicode() 
	{ 
		return m_bUnicodeOut; 
	}


	// ��������� �� �������� ���������?
	BOOL Match(StringExt *seEncodingName);

	// ������� ����� �������� ���������� �������� � ���������. ��������� <sBuffer> ����������� � ����������
	// ���������� ������������ ����. ������� ��������� ������������ (���� \0) �� �����. 
	int MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize);

private:

	UnicodeMap(StringExt *seEncodingName);

private:

	StringExt     *m_seEncodingName;
	UnicodeMapType m_eType;
	BOOL           m_bUnicodeOut;
	union 
	{
		UnicodeMapRange *m_pRanges;	  // ��� �����: User, Resident
		UnicodeMapFunc   m_pFunction; // ��� ���� Func
	};
	int            m_nLen;      // User, Resident
	UnicodeMapExt *m_pEMaps;    // User
	int            m_nEMapsLen; // User

	int            m_nRefCount;

	CRITICAL_SECTION m_oCS;

};

//-------------------------------------------------------------------------------------------------------------------------------
// UnicodeMapCache
//-------------------------------------------------------------------------------------------------------------------------------

#define unicodeMapCacheSize 4

class UnicodeMapCache 
{
public:

	UnicodeMapCache();
	~UnicodeMapCache();

	UnicodeMap *GetUnicodeMap(StringExt *seEncodingName, GlobalParams *pGlobalParams);

private:

	UnicodeMap *m_ppCache[unicodeMapCacheSize];
};

#endif /* _UNICODE_MAP_H */
