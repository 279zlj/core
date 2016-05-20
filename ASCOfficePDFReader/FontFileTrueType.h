#ifndef _FONT_FILE_TRUETYPE_H
#define _FONT_FILE_TRUETYPE_H

#include "FontFileBase.h"

class StringExt;
class CHash;
struct TrueTypeTable;
struct TrueTypeCmap;

//------------------------------------------------------------------------
// CFontFileTrueType
//------------------------------------------------------------------------

class CFontFileTrueType: public CFontFileBase 
{
public:

	// ������� ������ TTF �� ������.
	static CFontFileTrueType *LoadFromBuffer(char *sBuffer, int lenA);

	// ������� ������ TTF �� �����.
	static CFontFileTrueType *LoadFromFile(wchar_t *wsFileName);

	virtual ~CFontFileTrueType();

	// TRUE, ���� ������ OpenType ���� ��������� ������ ������� CFF.
	// FALSE,���� ��� TrueType ���� ( ��� OpenType ���� � ������� � ������� TrueType).
	BOOL IsOpenTypeCFF() 
	{ 
		return m_bOpenTypeCFF; 
	}

	int  GetCmapsCount();

	int  GetCmapPlatform(int nIndex);

	int  GetCmapEncoding(int nIndex);

	int  FindCmap(int nPlatform, int nEncoding);

	// ���������� GID, ��������������� ������� <nChar> � <nIndex>��� CMap.
	unsigned short MapCodeToGID(int nCMapIndex, int nChar);

	// ���������� GID, ��������������� <sName> � ������� post. ���������� 0,
	// ���� ������� � ����� ������ �� �����, ��� ������� post ���.
	int MapNameToGID(char *sName);

	// ���������� ����� CIDs � GIDs, � ���������� ���������� ���������
	// CIDs � *<pnCIDs>.  ������ ��� CID ������( OpenType CFF )
	unsigned short *GetCIDToGIDMap(int *pnCIDs);

	// ������������ ����������� �� ��������� �����( � ������������ ��
	// ������������� True Type):

	// * 4: ������� OS/2 �� ������� ��� �����������
	// * 3: ��������� �������������
	// * 2: ��������� �������������
	// * 1: ��������� ������������� � ��������
	// * 0: ���������� ���������
	int GetEmbeddingRestrictions();

	// Convert to a Type 42 font, suitable for embedding in a PostScript
	// file.  <psName> will be used as the PostScript font name (so we
	// don't need to depend on the 'name' table in the font).  The
	// <encoding> array specifies the mapping from char codes to names.
	// If <encoding> is NULL, the encoding is unknown or undefined.  The
	// <codeToGID> array specifies the mapping from char codes to GIDs.
	// (Not useful for OpenType CFF fonts.)
	void ToType42(char *sPSName, char **ppEncoding, unsigned short *pCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream );

	// Convert to a Type 1 font, suitable for embedding in a PostScript
	// file.  This is only useful with 8-bit fonts.  If <newEncoding> is
	// not NULL, it will be used in place of the encoding in the Type 1C
	// font.  If <ascii> is true the eexec section will be hex-encoded,
	// otherwise it will be left as binary data.  If <psName> is
	// non-NULL, it will be used as the PostScript font name.  (Only
	// useful for OpenType CFF fonts.)
	void ToType1(char *sPSName, char **ppNewEncoding, BOOL bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream );

	// Convert to a Type 2 CIDFont, suitable for embedding in a
	// PostScript file.  <psName> will be used as the PostScript font
	// name (so we don't need to depend on the 'name' table in the
	// font).  The <cidMap> array maps CIDs to GIDs; it has <nCIDs>
	// entries.  (Not useful for OpenType CFF fonts.)
	void ToCIDType2(char *sPSName, unsigned short *pCIDMap, int nCIDCount, BOOL bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 CIDFont, suitable for embedding in a
	// PostScript file.  <psName> will be used as the PostScript font
	// name.  (Only useful for OpenType CFF fonts.)
	void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 (but non-CID) composite font, suitable for
	// embedding in a PostScript file.  <psName> will be used as the
	// PostScript font name (so we don't need to depend on the 'name'
	// table in the font).  The <cidMap> array maps CIDs to GIDs; it has
	// <nCIDs> entries.  (Not useful for OpenType CFF fonts.)
	void ToType0(char *sPSName, unsigned short *pCIDMap, int nCIDCount, BOOL bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Convert to a Type 0 (but non-CID) composite font, suitable for
	// embedding in a PostScript file.  <psName> will be used as the
	// PostScript font name.  (Only useful for OpenType CFF fonts.)
	void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	// Write a clean TTF file, filling in missing tables and correcting
	// various other errors.  If <name> is non-NULL, the font is renamed
	// to <name>.  If <codeToGID> is non-NULL, the font is re-encoded,
	// using a Windows Unicode cmap.  If <name> is NULL and the font is
	// complete and correct, it will be written unmodified.  (Not useful
	// for OpenType CFF fonts.)
	void WriteTTF(FontFileOutputFunc pOutputFunc, void *pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL);

private:

	CFontFileTrueType(char *sFileName, int nLen, BOOL bFreeFileData);

	void ConvertEncoding   (char **ppEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	void ConvertCharStrings(char **ppEncoding, unsigned short *pnCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	void ConvertSfnts      (FontFileOutputFunc pOutputFunc, void *pOutputStream, StringExt *seName, BOOL bNeedVerticalMetrics);
	void DumpString(unsigned char *sString, int nLength, FontFileOutputFunc pOutputFunc, void *pOutputStream);
	unsigned int ComputeTableChecksum(unsigned char *sData, int nLength);
	void Parse();
	void ReadPostTable();
	int  SeekTable(char *sTag);

private:

	TrueTypeTable   *m_pTables;
	int              m_nTablesCount;
	TrueTypeCmap    *m_pCMaps;
	int              m_nCMapsCount;
	int              m_nGlyphs;
	int              m_nLocaFormat;
	int              m_arrBBox[4];
	CHash           *m_pNameToGID;
	BOOL             m_bOpenTypeCFF;

	BOOL             m_bSuccess;
};

#endif /* _FONT_FILE_TRUETYPE_H */
