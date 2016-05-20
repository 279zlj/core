#ifndef _PDF_READER_UNICODE_MAP_H
#define _PDF_READER_UNICODE_MAP_H

#include "CharTypes.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
	class StringExt;

	//-------------------------------------------------------------------------------------------------------------------------------

	enum UnicodeMapType
	{
		unicodeMapUser,      // ������ �� �����
		unicodeMapResident,  // ����������� ������
		unicodeMapFunc       // ��������� �� �������
	};

	typedef int(*UnicodeMapFunc)(Unicode nUnicode, char *sBuffer, int nBufferSize);

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
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapRange *pRanges, int nLen);

		// ������� Resident UnicodeMap, ������� ���������� ������� ������ ��������� ��������.
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapFunc pFunction);

		~UnicodeMap();

		// ������� ������
		void AddRef();
		void Release();

		StringExt *GetEncodingName()
		{
			return m_seEncodingName;
		}

		bool IsUnicode()
		{
			return m_bUnicodeOut;
		}


		// ��������� �� �������� ���������?
		bool Match(StringExt *seEncodingName);

		// ������� ����� �������� ���������� �������� � ���������. ��������� <sBuffer> ����������� � ����������
		// ���������� ������������ ����. ������� ��������� ������������ (���� \0) �� �����. 
		int MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize);

	private:

		UnicodeMap(StringExt *seEncodingName);

	private:

		StringExt     *m_seEncodingName;
		UnicodeMapType m_eType;
		bool           m_bUnicodeOut;
		union
		{
			UnicodeMapRange *m_pRanges;	  // ��� �����: User, Resident
			UnicodeMapFunc   m_pFunction; // ��� ���� Func
		};
		int            m_nLen;      // User, Resident
		UnicodeMapExt *m_pEMaps;    // User
		int            m_nEMapsLen; // User

		int            m_nRefCount;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
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
}

#endif // _PDF_READER_UNICODE_MAP_H
