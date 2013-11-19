#ifndef _GFONT_H
#define _GFONT_H

#include "StringExt.h"
#include "Object.h"
#include "CharTypes.h"
#include "GlobalParams.h"

class Dict;
class CMap;
class CharCodeToUnicode;
class CFontFileTrueType;
struct GrFontCIDWidths;

//------------------------------------------------------------------------
// GrFontType
//------------------------------------------------------------------------

enum GrFontType 
{
	//----- Gr8BitFont
	fontUnknownType,
	fontType1,
	fontType1C,
	fontType1COT,
	fontType3,
	fontTrueType,
	fontTrueTypeOT,
	//----- GrCIDFont
	fontCIDType0,
	fontCIDType0C,
	fontCIDType0COT,
	fontCIDType2,
	fontCIDType2OT
};

//------------------------------------------------------------------------
// GrFontCIDWidths
//------------------------------------------------------------------------

struct GrFontCIDWidthException
{
	CID    nFirst;     // ������ ������ ����������� � 
	CID    nLast;      // CID: <nFirst>...<nLast>
	double dWidth;     // ����� ������ ��������
};

struct GrFontCIDWidthExceptionV 
{
	CID    nFirst;    // ������ ������ ����������� � 
	CID    nLast;     // CID: <nFirst>...<nLast>
	double dHeight;   // ����� ������ ��������
	double dVx;       // ���������� ������� V: origin0->origin1
	double dVy;       // ��. ������ 40 � ������������ PDF 1.7
};

struct GrFontCIDWidths 
{
	double                   dDefaultWidth;     // ����������� �������� ������ �������
	double                   dDefaultHeight;    // ����������� �������� ������ �������
	double                   dDefaultV;         // ����������� �������� ������� V
	GrFontCIDWidthException *pExceptions;       // ���������� ��� �������������� ������
	int                      nExceptionsCount;  // ����� ����������
	GrFontCIDWidthExceptionV*pExceptionsV;      // ���������� ��� ������������ ������
	int                      nExceptionsVCount; // ����� ����������
};

//------------------------------------------------------------------------
// GrFont
//------------------------------------------------------------------------

#define fontFixedWidth (1 << 0)
#define fontSerif      (1 << 1)
#define fontSymbolic   (1 << 2)
#define fontItalic     (1 << 6)
#define fontBold       (1 << 18)

class GrFont 
{
public:

	static GrFont *MakeFont(XRef *pXref, char *sTag, Ref oID, Dict *pFontDict, GlobalParams *pGlobalParams);

	GrFont(char *sTag, Ref oID, StringExt *seName, GlobalParams *pGlobalParams);

	virtual ~GrFont();

	BOOL CheckValidate() 
	{ 
		return m_bValid; 
	}

	StringExt *GetTag()
	{ 
		return m_seTag; 
	}

	Ref *GetID() 
	{ 
		return &m_oID; 
	}

	BOOL CheckTag(char *sTag) 
	{ 
		return !m_seTag->Compare(sTag); 
	}

	StringExt *GetBaseName() 
	{ 
		return m_seName; 
	}

	StringExt *GetOriginalName() 
	{ 
		return m_seOriginalName; 
	}

	GrFontType GetType() 
	{ 
		return m_eType; 
	}
	virtual BOOL IsCIDFont() 
	{ 
		return FALSE; 
	}

	// ���������� �����.
	BOOL       GetEmbeddedFontFileRef(Ref *pEmbRef)
    { 
		*pEmbRef = m_oEmbFontFileRef; 
		return m_oEmbFontFileRef.nNum >= 0; 
	}

	StringExt *GetEmbeddedFontName() 
	{ 
		return m_seEmbeddedFontName; 
	}

	CStringW GetExternalFontFilePath() 
	{ 
		return m_wsExternalFontFilePath; 
	}

	//
	int GetFlags() 
	{ 
		return m_nFlags; 
	}
	BOOL IsFixedWidth() 
	{ 
		return m_nFlags & fontFixedWidth; 
	}
	BOOL IsSerif() 
	{ 
		return m_nFlags & fontSerif; 
	}
	BOOL IsSymbolic() 
	{ 
		return m_nFlags & fontSymbolic; 
	}
	BOOL IsItalic() 
	{ 
		return m_nFlags & fontItalic; 
	}
	BOOL IsBold() 
	{ 
		return m_nFlags & fontBold; 
	}

	double *GetFontMatrix() 
	{ 
		return m_arrFontMatrix; 
	}

	double *GetFontBBox() 
	{ 
		return m_arrFontBBox; 
	}

	double  GetAscent() 
	{ 
		return m_dAscent; 
	}
	double  GetDescent() 
	{ 
		return m_dDescent; 
	}

	// 0 = ����� �� �����������, 1 = �� ���������
	virtual int GetWMode() 
	{ 
		return 0; 
	}

	// ��������� ������� ��� ���������� FontFile � �����.
	char *ReadExternalFontFile(int *pnLength);
	char *ReadEmbeddedFontFile(XRef *pXref, int *pnLegth);

	// ��������� ��������� ������ �� ������ <sText> ����� <nLen> ����, ���������
	// ��� ������� <unCode>, ��� ��������� �������� <unUnicode>, ������ ��������� 
	// (w0 ��� w1, ��. ������������ PDF 1.7 ������ 40) (<dDx>, <dDy>), � ������ V
	// (��. ������ 40) (<Vx>, <Vy>). ���������� ���������� ���� ����������� ��� punCode.
	virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy) = 0;

protected:

	void ReadFontDescriptor(XRef *pXref, Dict *pFontDict);
	CharCodeToUnicode *ReadToUnicodeCMap(Dict *pFontDict, int nBitsCount, CharCodeToUnicode *pCharToUnicode);
	void FindExternalFontFile();

protected:

	StringExt *m_seTag;                  // ��� pdf-������
	Ref        m_oID;                    // ������ �� ������-����� (���������� � �������� ����������� ��������������)
	StringExt *m_seName;                 // �������� ������
	StringExt *m_seOriginalName;         // �������������� �������� ������
	GrFontType m_eType;                  // ��� ������
	int        m_nFlags;                 // ����� � FontDescriptor
	StringExt *m_seEmbeddedFontName;     // �������� ����������� � PDF ������(�.�. ����� ���� ������ � FontFile)
	Ref        m_oEmbFontFileRef;        // ������(pdf-������) �� FontFile ��� ����������� ������
	CStringW   m_wsExternalFontFilePath; // ���� � FontFile(�.�. font file �� � ����� PDF)

	double     m_arrFontMatrix[6];       // ������� �������������� ��������� ������� � ���������� ������ (������ ��� Type 3)
	double     m_arrFontBBox[4];         // ���������� ������������� �������� ������� ���� �������� (������ ��� Type 3)
	double     m_dMissingWidth;          // ����������� �������� ������ ��� ����������� ��������
	double     m_dAscent;                // Ascent
	double     m_dDescent;               // Descent

	BOOL       m_bValid;

	GlobalParams *m_pGlobalParams;
};

//------------------------------------------------------------------------
// Gr8BitFont
//------------------------------------------------------------------------

class Gr8BitFont: public GrFont 
{
public:

	Gr8BitFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, GrFontType eType, Dict *pFontDict, GlobalParams *pGlobalParams);

	virtual ~Gr8BitFont();

	virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy);

	char **GetEncoding() 
	{ 
		return m_ppEncoding; 
	}

	CharCodeToUnicode *GetToUnicode();

	char *GetCharName(int nCode) 
	{ 
		return m_ppEncoding[nCode]; 
	}

	BOOL GetHasEncoding() 
	{ 
		return m_bHasEncoding; 
	}

	BOOL GetUsesMacRomanEncoding() 
	{ 
		return m_bUsesMacRomanEncoding; 
	}

	double GetWidth(unsigned char unChar) 
	{ 
		return m_arrWidths[unChar]; 
	}
	// ������ ��� TrueType
	unsigned short *GetCodeToGIDMap(CFontFileTrueType *pTTF);

	// ��������� ��� ������� ������������ ������ ��� Type 3
	Dict   *GetCharProcs();
	Object *GetCharProc(int nCode, Object *pProc);
	Dict   *GetResources();

private:

	char              *m_ppEncoding[256];       // ���������( �har code --> �har name)
	BOOL               m_arrEncFree[256];       // ��������� �������� ��� ����� ������� �������: true, ���� ��� ������ �������� ������
	CharCodeToUnicode *m_pCharToUnicode;        // �har code --> Unicode
	BOOL               m_bHasEncoding;          // ������� �� ��������� ������?
	BOOL               m_bUsesMacRomanEncoding; // ��������� MacRomanEncoding?
	double             m_arrWidths[256];        // ������ ��������
	Object             m_oCharProcs;            // ���������� ������ ��� Type 3, ������� CharProcs
	Object             m_oResources;            // ���������� ������ ��� Type 3, ������� Resources 
};

//------------------------------------------------------------------------
// GrCIDFont
//------------------------------------------------------------------------

class GrCIDFont: public GrFont
{
public:

	GrCIDFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, Dict *pFontDict, GlobalParams *pGlobalParams);

	virtual ~GrCIDFont();

	virtual BOOL IsCIDFont() 
	{ 
		return TRUE; 
	}

	virtual int GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy);

	virtual int GetWMode();

	CharCodeToUnicode *GetToUnicode();

	// �ollection name (<registry>-<ordering>).
	StringExt *GetCollection();

	// CID-to-GID. ������������ ������ ���� ��� = fontCIDType2.
	unsigned short *GetCIDToGID() 
	{ 
		return m_pCidToGID; 
	}
	int GetCIDToGIDLen() 
	{ 
		return m_nCidToGIDLen; 
	}

	CMap *GetCMap()
	{
		return m_pCMap;
	}

private:

	CMap              *m_pCMap;           // char code --> CID
	CharCodeToUnicode *m_pCharToUnicode;  // CID --> Unicode
	GrFontCIDWidths    m_oWidths;         // character widths
	unsigned short    *m_pCidToGID;       // CID --> GID mapping (for embedded TrueType fonts)
	int                m_nCidToGIDLen;
};

//------------------------------------------------------------------------
// GrFontDict
//------------------------------------------------------------------------

class GrFontDict 
{
public:

	GrFontDict(XRef *pXref, Ref *pFontDictRef, Dict *pFontDict, GlobalParams *pGlobalParams);

	~GrFontDict();

	GrFont *Search(char *sTag);

	int GetFontsCount() 
	{ 
		return m_nFontsCount; 
	}
	GrFont *GetFont(int nIndex) 
	{ 
		return m_ppFonts[nIndex]; 
	}

private:

	GrFont      **m_ppFonts;     // ������ �������
	int           m_nFontsCount; // ���������� �������
	GlobalParams *m_pGlobalParams;
};

#endif /* _GFONT_H */
