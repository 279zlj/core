#ifndef _STRING_EXT_H
#define _STRING_EXT_H

#include <stdarg.h>

//---------------------------------------------------------------------------------------
// ����� StringExt - ������ CString
//---------------------------------------------------------------------------------------

class StringExt 
{

public:

	// ������� ������ ������.
	StringExt();

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
	static StringExt *Format (char *sFormat, ...);
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

	char GetAt(int nIndex) 
	{ 
		return m_sData[ nIndex ]; 
	}
	void SetAt(int nIndex, char nChar) 
	{ 
		m_sData[ nIndex ] = nChar; 
	}

	// ������� ������.
	StringExt *Clear();

	// ��������� ������ ��� ������.
	StringExt *Append(char nChar);
	StringExt *Append(StringExt *seString);
	StringExt *Append(const char *sString);
	StringExt *Append(const char *sString, int nLength);

	// ��������� ��������������� ������.
	StringExt *AppendFormat (char *sFormat, ...);
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
	int Compare (StringExt *seString);
	int CompareN(StringExt *seString, int nCount);
	int Compare (const char *sString);
	int CompareN(const char *sString, int nCount);

private:

	void Resize(int nLength);

	static void FormatInt   (         long nValue, char *sBuffer, int nBufferSize, BOOL bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);	
	static void FormatUInt  (unsigned long nValue, char *sBuffer, int nBufferSize, BOOL bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
	static void FormatDouble(       double nValue, char *sBuffer, int nBufferSize, int nPrecision, BOOL bTrim,            char **ppData, int *nLen);

private:

	int   m_nLength;
	char *m_sData;

};

#endif /* _STRING_EXT_H */
