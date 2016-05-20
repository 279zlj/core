#ifndef _PDF_READER_STREAM_H
#define _PDF_READER_STREAM_H

#include <stdio.h>
#include "Object.h"

#include "zlib.h"

//#pragma comment(lib, "zlib.lib")

namespace PdfReader
{
	class BaseStream;

	//------------------------------------------------------------------------

	enum StreamType
	{
		strFile,
		strASCIIHex,
		strASCII85,
		strLZW,
		strRunLength,
		strCCITTFax,
		strDCT,
		strFlate,
		strJBIG2,
		strJPX,
		strWeird			// internal-use stream types
	};

	enum StreamColorSpaceMode
	{
		streamCSNone,
		streamCSDeviceGray,
		streamCSDeviceRGB,
		streamCSDeviceCMYK
	};

	//------------------------------------------------------------------------

	enum CryptAlgorithm
	{
		cryptRC4,
		cryptAES
	};

	//------------------------------------------------------------------------
	// Stream (�������� �����)
	//------------------------------------------------------------------------

	class Stream
	{
	public:

		Stream();

		virtual ~Stream();

		// ������� ������.
		int AddRef()
		{
			return ++m_nRef;
		}
		int Release()
		{
			return --m_nRef;
		}

		// ��� ������.
		virtual StreamType GetType() = 0;

		// ���������� ��� ���������(� ���������).
		virtual void Reset() = 0;

		// ��������� �����.
		virtual void Close();

		// ������������� � ���������� ������� � ������.
		virtual int GetChar() = 0;

		// ������� �� ��������� ������ � ������.
		virtual int LookChar() = 0;

		// GetChar ��� ������������� Predictor.
		// ������������ ������ � StreamPredictor.
		virtual int GetRawChar();

		// ��������� ������ � ������.
		virtual char *GetLine(char *sBuffer, int nSize);

		// ������� ������� � ������.
		virtual int GetPos() = 0;

		// ������������� ������� ������� � ������. ���� nDirection - 
		// ������������, ����� ������� ���������� �� ����� ������, � 
		// ���� ������������, ����� �� ������.
		virtual void SetPos(unsigned int unPos, int nDirection = 0) = 0;

		// PostScript'�� �������� �������.
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);

		// ����� �� ����� ��������� ������������ �������?
		virtual bool IsBinary(bool bLast = true) = 0;

		virtual BaseStream *GetBaseStream() = 0;

		// ����� ����� ���������� ������������� (��� ����� ���� BaseStream 
		// ��� DecryptStream).
		virtual Stream *GetUndecodedStream() = 0;

		// ������� ��������� � ������ �������.
		virtual Dict *GetDict() = 0;

		// Encoding filter?
		virtual bool IsEncoder()
		{
			return false;
		}

		// �������� ��������� �����������, ������������� ���������� ������� ������.
		virtual void GetImageParams(int *pnBitsPerComponent, StreamColorSpaceMode *peCSMode)
		{
		}

		// ����������� ��������� ����� �� �����.
		virtual Stream *GetNextStream()
		{
			return NULL;
		}

		// ��������� ������� � ������� ������, ��������� � ���������� �������.
		// ������������ ����� �����.
		Stream *AddFilters(Object *pDict);

	private:

		Stream *ApplyFilter(char *sName, Stream *pStream, Object *pParams);

	private:

		int m_nRef; // ������� ������

	};

	//------------------------------------------------------------------------
	// BaseStream
	//
	// ��� �������� ����� ��� ���� ������� � ������ �� �����.
	//------------------------------------------------------------------------

	class BaseStream : public Stream
	{
	public:

		BaseStream(Object *pDict);
		virtual ~BaseStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict) = 0;
		virtual void SetPos(unsigned int unPos, int nDirection = 0) = 0;
		virtual bool IsBinary(bool bLast = true)
		{
			return bLast;
		}
		virtual BaseStream *GetBaseStream()
		{
			return this;
		}
		virtual Stream *GetUndecodedStream()
		{
			return this;
		}
		virtual Dict *GetDict()
		{
			return m_pDict.GetDict();
		}
		virtual StringExt *GetFileName()
		{
			return NULL;
		}

		// �����������/������������� ������� ������� ����� � ������ �����.
		virtual unsigned int GetStartPos() = 0;
		virtual void         SetStartPos(int nDelta) = 0;

	private:

		Object m_pDict;
	};

	//------------------------------------------------------------------------
	// FilterStream
	//
	// ��� �������� ����� ��� ���� �������, � ������� ������� ������.
	//------------------------------------------------------------------------

	class FilterStream : public Stream
	{
	public:

		FilterStream(Stream *pStream);
		virtual ~FilterStream();
		virtual void Close();
		virtual int GetPos()
		{
			return m_pStream->GetPos();
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual BaseStream *GetBaseStream()
		{
			return m_pStream->GetBaseStream();
		}
		virtual Stream *GetUndecodedStream()
		{
			return m_pStream->GetUndecodedStream();
		}
		virtual Dict *GetDict()
		{
			return m_pStream->GetDict();
		}
		virtual Stream *GetNextStream()
		{
			return m_pStream;
		}

	protected:

		Stream *m_pStream;
	};

	//------------------------------------------------------------------------
	// ImageStream
	//------------------------------------------------------------------------

	class ImageStream
	{
	public:

		// ������� Image object �� �������� � ��������� �����������( ��� ������� ����� 
		// �������� ��������� ��������, � �� �� ��� ����� ������ � Predictor).
		ImageStream(Stream *pStream, int nWidth, int nComponents, int nBitsPerComponent);

		~ImageStream();

		void Reset();

		// ��������� ��������� ������ �� ������. pPixel ������ ��������� �� ����� �����������,
		// ��� �������� ��� ������� m_nComponentsPerPixel ���������.
		bool GetPixel(unsigned char *pPixel);

		unsigned char *GetNextLine();

		void SkipLine();

	private:

		Stream        *m_pStream;             // �������� �����
		int            m_nWidth;              // ���������� �������� � ������
		int            m_nComponentsPerPixel; // ���������� ��������� �� ���� ������
		int            m_nBitsPerComponent;   // ���������� ��� � ����������
		int            m_nComponentsPerLine;  // ���������� ��������� � ����� ������
		unsigned char *m_pLineBuffer;         // ������ ��� ����� ������ �����������
		int            m_nLinePos;            // ������� ������� � m_pLineBuffer
	};

	//------------------------------------------------------------------------
	// StreamPredictor (Predictor - �������������� ������ ��� ��������)
	//------------------------------------------------------------------------

	class StreamPredictor
	{
	public:

		// ������� Predictor object. ���������, ������� �������� �����, �����  
		// �� ��������� � ��������� ����������� ��������.
		StreamPredictor(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);

		~StreamPredictor();

		bool CheckValidate()
		{
			return m_bSuccess;
		}


		int LookChar();
		int GetChar();

	private:

		bool GetNextLine();

	private:

		Stream *m_pStream;             // �������� �����
		int     m_nPredictor;          // Predictor
		int     m_nWidth;              // ���������� �������� � ������
		int     m_nComponentsPerPixel; // ���������� ��������� �� ���� ������
		int     m_nBitsPerComponent;   // ���������� ��� � ����������
		int     m_nComponentsPerLine;  // ���������� ��������� � ����� ������
		int     m_nBytesPerPixel;      // ���������� ���� � ����� �������
		int     m_nBytesPerLine;       // ���������� ���� � ������
		unsigned char *m_pLineBuffer;  // ������ ��� ����� ������ �����������
		int     m_nLinePos;            // ������� ������� � m_pLineBuffer

		bool    m_bSuccess;
	};

	//------------------------------------------------------------------------
	// FileStream
	//------------------------------------------------------------------------

#define FileStreamBufferSize 1024

	class FileStream : public BaseStream
	{
	public:

		FileStream(FILE *pFile, unsigned int unStart, bool bLimited, unsigned int nLength, Object *pDict);
		virtual ~FileStream();

		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int nLength, Object *pDict);
		virtual StreamType GetType()
		{
			return strFile;
		}
		virtual void Reset();
		virtual void Close();
		inline virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		inline virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		inline virtual int GetPos()
		{
			return m_unBufferPos + (m_pBufferPointer - m_sBuffer);
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos()
		{
			return m_unStart;
		}
		virtual void SetStartPos(int nDelta);

	private:

		bool FillBuffer();

	private:

		FILE        *m_pFile;
		unsigned int m_unStart;
		bool         m_bLimited;
		unsigned int m_unLength;
		char         m_sBuffer[FileStreamBufferSize];
		char        *m_pBufferPointer;
		char        *m_pBufferEnd;
		unsigned int m_unBufferPos;
		int          m_nSavePos;
		bool         m_bSaved;
	};
	//------------------------------------------------------------------------
	// MemoryStream
	//------------------------------------------------------------------------

	class MemoryStream : public BaseStream
	{
	public:

		MemoryStream(char *sBuffer, unsigned int unStart, unsigned int unLength, Object *pDict);
		virtual ~MemoryStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict);
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual void Close();
		virtual int GetChar()
		{
			return (m_pBufferPointer < m_pBufferEnd) ? (*m_pBufferPointer++ & 0xff) : EOF;
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer < m_pBufferEnd) ? (*m_pBufferPointer & 0xff) : EOF;
		}
		virtual int GetPos()
		{
			return (int)(m_pBufferPointer - m_sBuffer);
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos()
		{
			return m_unStart;
		}
		virtual void SetStartPos(int delta);

	private:

		char        *m_sBuffer;
		unsigned int m_unStart;
		unsigned int m_unLength;
		char        *m_pBufferEnd;
		char        *m_pBufferPointer;
		bool         m_bNeedFree;
	};

	//------------------------------------------------------------------------
	// EmbedStream
	//
	// ����������� ��� ������, ������������ ��� �����������(����������) �������
	// (Inline images). ������ �������� �� Base stream -- ����� �������� 
	// EmbedStream, ������ �� Base stream ����� ����������� ���  ���������� �����
	// ������. ��� ���������� ���������, ��� ������ ������� ����� �����
	// FileStream (��������� MakeSubStream).
	//------------------------------------------------------------------------

	class EmbedStream : public BaseStream
	{
	public:

		EmbedStream(Stream *pStream, Object *pDict, bool bLimited, unsigned int unLength);
		virtual ~EmbedStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict);
		virtual StreamType GetType()
		{
			return m_pStream->GetType();
		}
		virtual void Reset()
		{
		}
		virtual void Close(){};
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetPos()
		{
			return m_pStream->GetPos();
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos();
		virtual void SetStartPos(int nDelta);

	private:

		Stream      *m_pStream;
		bool         m_bLimited;
		unsigned int m_unLength;
	};

	//------------------------------------------------------------------------
	// ASCIIHexStream
	//------------------------------------------------------------------------

	class ASCIIHexStream : public FilterStream
	{
	public:

		ASCIIHexStream(Stream *pStream);
		virtual ~ASCIIHexStream();
		virtual StreamType GetType()
		{
			return strASCIIHex;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			m_nBuffer = EOF;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		int  m_nBuffer;
		bool m_bEOF;
	};

	//------------------------------------------------------------------------
	// ASCII85Stream
	//------------------------------------------------------------------------

	class ASCII85Stream : public FilterStream
	{
	public:

		ASCII85Stream(Stream *pStream);
		virtual ~ASCII85Stream();
		virtual StreamType GetType()
		{
			return strASCII85;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			++m_nIndex;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		int  m_arrC[5]; // ����� �������� C � B ����� �� ������������
		int  m_arrB[4];
		int  m_nIndex;
		int  m_nCount;
		bool m_bEOF;
	};

	//------------------------------------------------------------------------
	// LZWStream
	//------------------------------------------------------------------------

	class LZWStream : public FilterStream
	{
	public:

		LZWStream(Stream *pStream, int nPredictor, int nColumns, int nColors, int nBitPerPixel, int nEarlyChange);
		virtual ~LZWStream();
		virtual StreamType GetType()
		{
			return strLZW;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		bool ProcessNextCode();
		void ClearTable();
		int  GetCode();

	private:

		StreamPredictor *m_pPredictor;         // Predictor (��� ���� �������������� ������)
		int              m_nEarlyChange;       // �������� � ������� ��� LZW �������
		bool             m_bEOF;               // ����� ������?
		int              m_nInputBuffer;       // Input buffer
		int              m_nInputBits;         // ���������� ��� � �������
		struct
		{
			int           nLength;
			int           nHead;
			unsigned char unTail;
		}  m_pTable[4097];       // ������� ��� �������������
		int              m_nNextCode;          // ��������� ���
		int              m_nNextBits;          // ����� ��� � ��������� ����
		int              m_nPrevCode;          // ���������� ��� � ������
		int              m_nNewChar;           // ����� ������, ������� �� ������� � �������
		unsigned char    m_arrCurBuffer[4097]; // ������ ��� ������� ������������������
		int              m_nCurLength;         // ����� ������� ������������������
		int              m_nCurPos;            // ������� � ������� ������������������
		bool             m_bFirst;             // �������� �� ������ ��� ������ ����� �������� �������
	};

	//------------------------------------------------------------------------
	// RunLengthStream
	//------------------------------------------------------------------------

	class RunLengthStream : public FilterStream
	{
	public:

		RunLengthStream(Stream *pStream);
		virtual ~RunLengthStream();
		virtual StreamType GetType()
		{
			return strRunLength;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pEndOfBuffer && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pEndOfBuffer && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[128];   // �����
		char *m_pBufferPointer; // ��������� �� ��������� ������ � ������
		char *m_pEndOfBuffer;   // ��������� �� ����� ������
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// CCITTFaxStream
	//------------------------------------------------------------------------

	struct CCITTCodeTable;

	class CCITTFaxStream : public FilterStream
	{
	public:

		CCITTFaxStream(Stream *pStream, int nK, bool bEndOfLine, bool bByteAlign, int nColumns, int nRows, bool bEndOfBlock, bool bBlackIs1);
		virtual ~CCITTFaxStream();
		virtual StreamType GetType()
		{
			return strCCITTFax;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			m_nCharBuffer = EOF;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		short Get2DCode();
		short GetWhiteCode();
		short GetBlackCode();
		short LookBits(int nCount);
		void  SkipBits(int nCount)
		{
			if ((m_nInputBits -= nCount) < 0)
				m_nInputBits = 0;
		}

	private:

		int    m_nK;           // �������� 'K', ������������ ��� ���������
		bool   m_bEndOfLine;   // �������� 'EndOfLine'
		bool   m_bByteAlign;   // �������� 'EncodedByteAlign'
		int    m_nColumns;     // �������� 'Columns'
		int    m_nRows;        // �������� 'Rows'
		bool   m_bEndOfBlock;  // �������� 'EndOfBlock'
		bool   m_bBlackIs1;    // �������� 'BlackIs1'

		bool   m_bEOF;         // 
		bool   m_bNextLine2D;  // True, ���� ��������� ������ ���������� ��������� 2D
		int    m_mCurRow;      // ������� ������
		int    m_nInputBuffer; // Input buffer
		int    m_nInputBits;   // ���������� ��� � Input buffer
		short *m_pRefLine;     // ��������� ������, �������� ��������
		int    m_nCurPosRL;    // ������� ������� � m_pRefLine
		short *m_pCodingLine;  // ���������� ������, �������� ��������
		int    m_nCurPosCL;    // ������� ������� � m_pCodingLine 
		int    m_nOutputBits;  //
		int    m_nCharBuffer;  // 
	};

	//------------------------------------------------------------------------
	// DCTStream
	//------------------------------------------------------------------------

	struct DCTCompInfo
	{
		int nID;            // ID ������� ��������
		int nXResolution;   // ���������� �� �����������
		int nYResolution;   // ���������� �� ���������
		int nQuantTableNum; // ����� ������� �����������
		int nPrevDC;        // 
	};

	struct DCTScanInfo
	{
		bool arrbComponent[4];  // arrbComponent[i] = true, ���� i-�� ���������� �������� � ������ ���������
		int  nComponentsCount;  // ���������� ��������� � ������ ���������
		int  arrDCHuffTable[4];	// ������ ������ DC Huffman
		int  arrACHuffTable[4]; // ������ ������ AC Huffman
		int  nFirstKoef;        // ������ DCT �����������
		int  nLastKoef;         // ��������� DCT �����������
		int  nApproxH;
		int  nApproxL;
	};

	struct DCTHuffTable
	{
		unsigned char  arrunFirstSymbol[17]; // ������ ������ � ������ ������
		unsigned short arrunFirstCode[17];   // ������ ��� � ������ ������
		unsigned short arrunCodesCount[17];  // ���������� ����� � ������ ������
		unsigned char  arrunSymbols[256];    // ���� �������
	};

	class DCTStream : public FilterStream
	{
	public:

		DCTStream(Stream *pStream, int nColorTransform);
		virtual ~DCTStream();
		virtual StreamType GetType()
		{
			return strDCT;
		}
		virtual void Reset();
		virtual void Close();
		virtual int GetChar();
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);
		Stream *GetRawStream()
		{
			return m_pStream;
		}

	private:

		void Restart();
		bool ReadMCURow();
		void ReadScan();
		bool ReadDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64]);
		bool ReadProgressiveDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64]);
		void DecodeImage();
		void TransformDataUnit(unsigned short *pQuantTable, int arrDataIn[64], unsigned char arrDataOut[64]);
		int  ReadHuffSymbol(DCTHuffTable *pTable);
		int  ReadAmp(int nSize);
		int  ReadBit();
		bool ReadHeader();
		bool ReadBaselineSOF();
		bool ReadProgressiveSOF();
		bool ReadScanInfo();
		bool ReadQuantTables();
		bool ReadHuffmanTables();
		bool ReadRestartInterval();
		bool ReadJFIFMarker();
		bool ReadAdobeMarker();
		bool ReadTrailer();
		int  ReadMarker();
		int  Read16();

	private:

		bool           m_bProgressive;          // True, ���� ��� progressive
		bool           m_bInterleaved;          // True, ���� ��� interleaved
		int            m_nWidth;                // ������ �����������
		int            m_nHeight;               // ������ �����������
		int            m_nMCUWidth;             // ������ min coding unit(MCU)
		int            m_nMCUHeight;            // ������ min coding unit(MCU)
		int            m_nBufferWidth;          // ������ FrameBuffer
		int            m_nBufferHeight;         // ������ FrameBuffer
		DCTCompInfo    m_arrCompInfo[4];	      // ��������� Info ��� ������ ����������
		DCTScanInfo    m_oCurScanInfo;          // ��������� Info ��� �������� Scan
		int            m_nComponentsCount;      // ���������� ��������� �����������
		int            m_nColorTransform;       // �������������� ������������ ������:
		// -1 = ������������
		//  0 = ��� ��������������
		//  1 = YUV/YUVK -> RGB/CMYK
		bool           m_bJFIFMarker;           // True, ���� ���� ������ APP0 JFIF 
		bool           m_bAdobeMarker;          // True, ���� ���� ������ APP14 Adobe 
		int            m_nRestartInterval;      // � MCU
		unsigned short m_arrQuantTables[4][64]; // ������� �����������
		int            m_nQuantTablesCount;     // ���������� ������� �����������
		DCTHuffTable   m_arrDCHuffTables[4];    // ������� DC Huffman
		DCTHuffTable   m_arrACHuffTables[4];    // ������� AC Huffman
		int            m_nDCHuffTablesCount;    // ���������� ������ DC Huffman
		int            m_nACHuffTablesCount;    // ���������� ������ AC Huffman
		unsigned char *m_pppRowBuffer[4][32];   // ����� ��� MCU (non-progressive mode)
		int           *m_ppFrameBuffer[4];      // ����� ��� ������ (progressive mode)

		int            m_nCurComponent;         // ������� ��������� ��� �������/MCU
		int            m_nX;                    //
		int            m_nY;                    //
		int            m_nDY;                   //

		int            m_nRestartCtr;           // ���������� ���������� MCU �� ��������
		int            m_nRestartMarker;        //
		int            m_nEOBRun;               // ���������� ���������� EOB(end-of-block) � ������� ������

		int            m_nInputBuffer;          // ����� ��� ����� ������ �����
		int            m_nInputBits;            // ����� ���������� ��� � Input buffer

	};

	//------------------------------------------------------------------------
	// FlateZlibStream
	//------------------------------------------------------------------------

#define flateZlibWindow             32768   // ������ ������
#define flateZlibMask               32767   // (flateZlibWindow - 1)

	class FlateZlibStream : public FilterStream
	{
	public:

		FlateZlibStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);
		virtual ~FlateZlibStream();
		virtual StreamType GetType()
		{
			return strFlate;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		void ReadSome();

	private:

		z_stream          m_oZStream;                     // Zlib ����� 
		StreamPredictor  *m_pPredictor;                   // Predictor
		unsigned int      m_nInSize;                      // ������ �������� �������
		unsigned char     m_arrInBuffer[flateZlibWindow]; // ������� ������
		unsigned char     m_arrBuffer[flateZlibWindow];   // ����� ��� ���������� �������������
		int               m_nBufferCurPos;                // ������� ������� � ������
		int               m_nRemain;                      // ����� ���������� ���� � ������
		bool              m_bEndOfBlock;                  // True, ����� �������� ����� �����
		bool              m_bEOF;                         // True, ����� �������� ����� ������
	};


	//------------------------------------------------------------------------
	// FlateStream
	//------------------------------------------------------------------------

#define flateWindow             32768   // ������ ������
#define flateMask               (flateWindow-1)
#define flateMaxHuffman         15      // ����������� ����� ���� Huffman
#define flateMaxCodeLenCodes    19      // max # code length codes
#define flateMaxLitCodes        288     // max # literal codes
#define flateMaxDistCodes       30      // max # distance codes

	struct FlateHuffmanCode
	{
		unsigned short nLength;   // ����� ���� � �����
		unsigned short nValue;    // �������� ������� ����
	};

	struct FlateHuffmanTable
	{
		FlateHuffmanCode *pCodes;  // ������� �����
		int               nMaxLen; // ����������� ����� ����
	};

	struct FlateDecode
	{
		int nExtraBitsCount; // ���������� �������������� ���
		int nFirst;          // ������ �����
	};

	class FlateStream : public FilterStream
	{
	public:

		FlateStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);
		virtual ~FlateStream();
		virtual StreamType GetType()
		{
			return strFlate;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		void ReadSome();
		bool StartBlock();
		void LoadFixedCodes();
		bool ReadDynamicCodes();
		void ConvertHuffmanCodes(int *pLengths, int nCount, FlateHuffmanTable *pTable);
		int  GetHuffmanCodeWord(FlateHuffmanTable *pTable);
		int  GetCodeWord(int nBits);

	private:

		StreamPredictor  *m_pPredictor;              // Predictor
		unsigned char     m_arrBuffer[flateWindow];  // ����� ��� ���������� �������������
		int               m_nBufferCurPos;           // ������� ������� � ������
		int               m_nRemain;                 // ����� ���������� ���� � ������
		int               m_nCodeBuffer;             // ����� �������� �����
		int               m_nCodeSize;               // ����� ��� � m_nCodeBuffer
		int				  m_arrCodeLengths[flateMaxLitCodes + flateMaxDistCodes]; // ����� �����(� ��� ����� ���� � ��� ����������)
		FlateHuffmanTable m_oLiteratCodeTable;       // ������� ��������� �����
		FlateHuffmanTable m_oDistanceCodeTable;      // ������� �����-����������
		bool              m_bCompressedBlock;        // True, ���� ������ ������ ����
		int               m_nUncompBlockLen;         // ���������� ����� ��������� �����
		bool              m_bEndOfBlock;             // True, ����� �������� ����� �����
		bool              m_bEOF;                    // True, ����� �������� ����� ������

		static int               m_arrCodeLenCodeMap[flateMaxCodeLenCodes]; // 
		static FlateDecode       m_arrLengthDecode[flateMaxLitCodes - 257];   // 
		static FlateDecode       m_arrDistanceDecode[flateMaxDistCodes];    // 
		static FlateHuffmanTable m_oFixedLiteralCodeTable;   // 
		static FlateHuffmanTable m_oFixedDistanceCodeTable;  // 
	};

	//------------------------------------------------------------------------
	// EOFStream
	//------------------------------------------------------------------------

	class EOFStream : public FilterStream
	{
	public:

		EOFStream(Stream *pStream);
		virtual ~EOFStream();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset()
		{
		}
		virtual int GetChar()
		{
			return EOF;
		}
		virtual int LookChar()
		{
			return EOF;
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
	};

	//------------------------------------------------------------------------
	// FixedLengthEncoder
	//------------------------------------------------------------------------

	class FixedLengthEncoder : public FilterStream
	{
	public:

		FixedLengthEncoder(Stream *pStream, int nLength);
		~FixedLengthEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true);
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		int m_nLength;
		int m_nCount;
	};

	//------------------------------------------------------------------------
	// ASCIIHexEncoder
	//------------------------------------------------------------------------

	class ASCIIHexEncoder : public FilterStream
	{
	public:

		ASCIIHexEncoder(Stream *pStream);
		virtual ~ASCIIHexEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[4];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		int   m_nLineLength;
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// ASCII85Encoder
	//------------------------------------------------------------------------

	class ASCII85Encoder : public FilterStream
	{
	public:

		ASCII85Encoder(Stream *pStream);
		virtual ~ASCII85Encoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
		virtual bool IsEncoder()
		{
			return true;
		}
	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[8];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		int   m_nLineLength;
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// RunLengthEncoder
	//------------------------------------------------------------------------

	class RunLengthEncoder : public FilterStream
	{
	public:

		RunLengthEncoder(Stream *pStream);
		virtual ~RunLengthEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return true;
		}
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[131];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		char *m_pNextEnd;
		bool  m_bEOF;
	};
}

#endif // _PDF_READER_STREAM_H
