#ifndef _PDF_READER_FONT_FILE_TYPE1C_H
#define _PDF_READER_FONT_FILE_TYPE1C_H

#include "FontFileBase.h"

namespace PdfReader
{
	class StringExt;

	//------------------------------------------------------------------------

	struct Type1CIndex
	{
		int nPos;         // ������� � ����� �� ������ �����
		int nCount;       // ���������� ���������
		int nOffsetSize;  // Offset size
		int nStartPos;    // ��������� ������� index data - 1
		int nEndPos;      // ������� ���������� ����� ����� Type1CIndex
	};

	struct Type1CIndexVal
	{
		int nPos;         // ������� � ����� �� ������ �����
		int nLen;         // ����� � ������
	};

	struct Type1CTopDict
	{
		int    nFirstOperator;

		int    nVersionSID;
		int    nNoticeSID;
		int    nCopyrightSID;
		int    nFullNameSID;
		int    nFamilyNameSID;
		int    nWeightSID;

		int    nIsFixedPitch;
		double dItalicAngle;
		double dUnderlinePosition;
		double dUnderlineThickness;

		int    nPaintType;
		int    nCharStringType;
		double arrdFontMatrix[6];
		bool   bHasFontMatrix;	// � CID ������ �������� ������� ����� ����� � FD, � �� � ������� �������
		int    nUniqueID;
		double arrdFontBBox[4];
		double dStrokeWidth;
		int    nCharsetOffset;
		int    nEncodingOffset;
		int    nCharStringsOffset;
		int    nPrivateSize;
		int    nPrivateOffset;

		// CIDFont entries
		int    nRegistrySID;
		int    nOrderingSID;
		int    nSupplement;
		int    nFDArrayOffset;
		int    nFDSelectOffset;
	};



#define type1CMaxBlueValues 14
#define type1CMaxOtherBlues 10
#define type1CMaxStemSnap   12

	struct Type1CPrivateDict
	{
		double arrdFontMatrix[6];
		bool   bHasFontMatrix;
		int    arrnBlueValues[type1CMaxBlueValues];
		int    nBlueValues;
		int    arrnOtherBlues[type1CMaxOtherBlues];
		int    nOtherBlues;
		int    arrnFamilyBlues[type1CMaxBlueValues];
		int    nFamilyBlues;
		int    arrnFamilyOtherBlues[type1CMaxOtherBlues];
		int    nFamilyOtherBlues;
		double dBlueScale;
		int    nBlueShift;
		int    nBlueFuzz;
		double dStdHW;
		bool   bHasStdHW;
		double dStdVW;
		bool   bHasStdVW;
		double arrdStemSnapH[type1CMaxStemSnap];
		int    nStemSnapH;
		double arrdStemSnapV[type1CMaxStemSnap];
		int    nStemSnapV;
		bool   bForceBold;
		bool   bHasForceBold;
		double dForceBoldThreshold;
		int    nLanguageGroup;
		double dExpansionFactor;
		int    nInitialRandomSeed;
		int    nSubrsOffset;
		double dDefaultWidthX;
		bool   bDefaultWidthXFP;
		double dNominalWidthX;
		bool   bNominalWidthXFP;
	};

	struct Type1COperator
	{
		bool bIsNumber;   // true -> number, false -> operator
		bool bIsFloat;    // true -> floating point number, false -> int
		union
		{
			double dNumber;
			int    nOperator;
		};
	};

	struct Type1CEexecBuf
	{
		FontFileOutputFunc pOutputFunc;
		void              *pOutputStream;
		bool               bASKII;          // ASCII ���������?
		unsigned short     unEncryptionKey; // eexec encryption key
		int                nLine;           // ���������� eexec-��������, ���������� �� ������� ������
	};

	//------------------------------------------------------------------------
	// CFontFileType1C
	//------------------------------------------------------------------------

	class CFontFileType1C : public CFontFileBase
	{

	public:

		static CFontFileType1C *LoadFromBuffer(char *sBuffer, int nLen);

		static CFontFileType1C *LoadFromFile(wchar_t *wsFileName);

		virtual ~CFontFileType1C();

		char *GetName();

		// ���������� ���������, ��� ������ 256 ���� (��������� ����� ���� 
		// NULL). ������������ ������ ��� 8-������ ������.
		char **GetEncoding();

		unsigned short *GetCIDToGIDMap(int *arrCIDs);

		// Convert to a Type 1 font, suitable for embedding in a PostScript
		// file.  This is only useful with 8-bit fonts.  If <newEncoding> is
		// not NULL, it will be used in place of the encoding in the Type 1C
		// font.  If <ascii> is true the eexec section will be hex-encoded,
		// otherwise it will be left as binary data.  If <psName> is non-NULL,
		// it will be used as the PostScript font name.
		void ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 0 CIDFont, suitable for embedding in a
		// PostScript file.  <psName> will be used as the PostScript font
		// name.
		void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 0 (but non-CID) composite font, suitable for
		// embedding in a PostScript file.  <psName> will be used as the
		// PostScript font name.
		void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	private:

		CFontFileType1C(char *sBuffer, int nLen, bool bFreeData);
		void EexecConvertGlyph(Type1CEexecBuf *pEexecBuf, char *sGlyphName, int nOffset, int nBytes, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict);
		void ConvertGlyph(int nOffset, int nBytes, StringExt *seCharBuffer, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict, bool bTop);
		void ConvertGlyphWidth(bool bUseOperation, StringExt *seCharBuffer, Type1CPrivateDict *pDict);
		void ConvertNum(double dValue, bool bIsFloat, StringExt *seCharBuffer);
		void EexecWrite(Type1CEexecBuf *pEexecBuf, char *sBuffer);
		void EexecWriteCharString(Type1CEexecBuf *pEexecBuf, unsigned char *sBuffer, int nLen);
		bool Parse();
		void ReadTopDict();
		void ReadFD(int nOffset, int nLength, Type1CPrivateDict *pDict);
		void ReadPrivateDict(int nOffset, int nLength, Type1CPrivateDict *pDict);
		void ReadFDSelect();
		void BuildEncoding();
		bool ReadCharset();
		int GetOperator(int nPos, bool bCharString, bool *pbSuccess);
		int GetDeltaIntArray(int *pArray, int nMaxLen);
		int GetDeltaDoubleArray(double *pArray, int nMaxLen);
		void GetIndex(int nPos, Type1CIndex *pIndex, bool *pbSuccess);
		void GetIndexVal(Type1CIndex *pIndex, int nIndex, Type1CIndexVal *pIndexVal, bool *bSuccess);
		char *GetString(int nSID, char *sBuffer, bool *pbSuccess);

	private:

		StringExt         *m_seName;
		char             **m_arrEncoding;

		Type1CIndex        m_oNameIndex;
		Type1CIndex        m_oTopDictIndex;
		Type1CIndex        m_oStringIndex;
		Type1CIndex        m_oGsubrIndex;
		Type1CIndex        m_oCharStringsIndex;

		Type1CTopDict      m_oTopDict;
		Type1CPrivateDict *m_pPrivateDicts;

		int                m_nGlyphsCount;
		int                m_nFDsCount;
		unsigned char     *m_pnFDSelect;
		unsigned short    *m_pnCharset;
		int                m_nGsubrBias;

		bool               m_bSuccessParsed;

		Type1COperator     m_arrOperators[49];
		int                m_nOperatorsCount;
		int                m_nHints;           // ��� �������� �������
		bool               m_bFirstOperator;
		bool               m_bOpenPath;		   // true, ���� ���� ���������� ���
	};
}

#endif // _PDF_READER_FONT_FILE_TYPE1C_H
