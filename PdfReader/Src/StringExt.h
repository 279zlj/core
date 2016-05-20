#ifndef _PDF_READER_STRING_EXT_H
#define _PDF_READER_STRING_EXT_H

#include <stdarg.h>
#include <string>
#include "../../DesktopEditor/common/String.h"

namespace PdfReader
{
	//---------------------------------------------------------------------------------------
	// WString - ������ BSTR
	//---------------------------------------------------------------------------------------
	typedef wchar_t* WString;

	static WString AllocWString(std::wstring& wsString)
	{
		int nLen = wsString.length();
		WString wsResult = new wchar_t[nLen + 1];
		wsResult[nLen] = 0x0000;

		for (int nIndex = 0; nIndex < nLen; nIndex++)
		{
			wsResult[nIndex] = (wchar_t)wsString.at(nIndex);
		}

		return wsResult;
	}
	static WString AllocWString(const wchar_t* wsString)
	{
		return AllocWString(std::wstring(wsString));
	}
	static WString AllocWString(WString wsString)
	{
		return AllocWString(std::wstring(wsString));
	}
	static void FreeWString(WString wString)
	{
		if (wString)
			delete[] wString;
	}

	static std::wstring* AStringToPWString(const char* sString)
	{
		return new std::wstring(NSString::CConverter::GetUnicodeFromSingleByteString((unsigned char*)sString, strlen(sString)));
	}
	static std::wstring AStringToWString(const char* sString)
	{
		return std::wstring(NSString::CConverter::GetUnicodeFromSingleByteString((unsigned char*)sString, strlen(sString)));
	}


	//---------------------------------------------------------------------------------------
	// ����� StringExt - ������ CString
	//---------------------------------------------------------------------------------------

	class StringExt
	{

	public:

		// ������� ������ ������.
		StringExt();

		StringExt(std::wstring& wsString);

		StringExt(const wchar_t* wsString);

		// ������� ������ �� ������ ������.
		StringExt(const char *sString);

		// ������� ������ �� <nLength> �������� � <sString>. ������ ������
		// ����� ��������� ������� �������.
		StringExt(const char *sString, int nLength);

		// ������� ������ �� <nLength> ��������, ������� � <nIndex>, ������ <seString>.
		StringExt(StringExt *seString, int nIndex, int nLength);

		// �������� ������.
		StringExt(StringExt *seString);
		StringExt *Copy()
		{
			return new StringExt(this);
		}

		// ��������� ��� ������.
		StringExt(StringExt *seString1, StringExt *seString2);

		// ��������� ����� �������� � ������.
		static StringExt *FromInt(int nValue);

		// ������� ��������������� ������. ������� ������� printf, �� ��� �������
		// � ������������� ������. ������ �������� ��������� �������:
		//     {<nArg>:[<nWidth>][.<precision>]<type>}
		// ���:
		// - <nArg> ����� ��������� (��������� ���������� � 0).
		// -- PS: ���� ��������� ������ ���� �� �������, � ������������ �� �����
		//    ����������� � � ����� �������.
		// - <nWidth> ������ ����, ���� ��� ������������, ����� ���������� �����
		//   ������� �� ���������������, � ������ ����� ����� ��������� ������.
		// - <nPrecision> ���������� ������ ����� �������
		// - <nType> ��� ���� �� ��������:
		//     d, x, o, b -- �����(int) � ����������, �����������������, ������������
		//                   � �������� �������� ����������
		//     ud, ux, uo, ub -- ���� �����, ������ ����������� �����(uint)
		//     ld, lx, lo, lb, uld, ulx, ulo, ulb -- ����������� long � ulong
		//     f, g -- double
		//     c -- char
		//     s -- string (char *)
		//     t -- StringExt *
		//     w -- �������; �������� ��������� �������� ���������� ��������
		// ��� ������ �������� ������ ���� ������������ {{ � }}.
		static StringExt *Format(char *sFormat, ...);
		static StringExt *FormatV(char *sFormat, va_list sArgList);

		// ����������.
		~StringExt();

		int GetLength()
		{
			return m_nLength;
		}

		// ���������� ������ � ���� char*.
		char *GetBuffer()
		{
			return m_sData;
		}

		std::wstring GetWString()
		{
			return NSString::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)m_sData, m_nLength);
		}

		char GetAt(int nIndex)
		{
			return m_sData[nIndex];
		}
		void SetAt(int nIndex, char nChar)
		{
			m_sData[nIndex] = nChar;
		}

		// ������� ������.
		StringExt *Clear();

		// ��������� ������ ��� ������.
		StringExt *Append(char nChar);
		StringExt *Append(StringExt *seString);
		StringExt *Append(const char *sString);
		StringExt *Append(const char *sString, int nLength);

		// ��������� ��������������� ������.
		StringExt *AppendFormat(char *sFormat, ...);
		StringExt *AppendFormatV(char *sFormat, va_list sArgList);

		// ��������� ������ ��� ������.
		StringExt *Insert(int nIndex, char nChar);
		StringExt *Insert(int nIndex, StringExt  *seString);
		StringExt *Insert(int nIndex, const char *sString);
		StringExt *Insert(int nIndex, const char *sString, int nLength);

		// ������� ���� ������ ��� ������ ��������.
		StringExt *Delete(int nIndex, int nCount = 1);

		// ������ � ������ ��� ������� ��������/���������� �������.
		StringExt *MakeUpper();
		StringExt *MakeLower();

		// ��������� ���� �����:  -1:<  0:=  +1:>
		int Compare(StringExt *seString);
		int CompareN(StringExt *seString, int nCount);
		int Compare(const char *sString);
		int CompareN(const char *sString, int nCount);

	private:

		void Resize(int nLength);

		static void FormatInt(long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
		static void FormatUInt(unsigned long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
		static void FormatDouble(double nValue, char *sBuffer, int nBufferSize, int nPrecision, bool bTrim, char **ppData, int *nLen);

	private:

		int   m_nLength;
		char *m_sData;

	};
}

#endif //_PDF_READER_STRING_EXT_H
