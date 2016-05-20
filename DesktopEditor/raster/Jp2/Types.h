#pragma once
#include "Reader.h"
#include "../../graphics/Defines.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Constant definitions
	//-------------------------------------------------------------------------------------------------------------------------------

#define MAX_PATH_LEN 4096 // ������������ ����� ��� ���� ������ 

#define J2K_MAXRLVLS 33					    // ������������ ����� ������� ����������
#define J2K_MAXBANDS (3 * J2K_MAXRLVLS - 2)	// ������������ ����� �������, ��������� � �������� ����������

#define J2K_MAXSSFTR 2    // Max SubSampling Factor (�������� ������ 2 ����� �������� � ������)

	void* Malloc(size_t nSize);
	void  Free(void* pMem);
	//-------------------------------------------------------------------------------------------------------------------------------
	// Enum definitions
	//-------------------------------------------------------------------------------------------------------------------------------

	// Progression order
	typedef enum EProgressionOrder
	{
		poUnknown = -1, // ����������� �������
		poLRCP    =  0, // layer-resolution-component-precinct
		poRLCP    =  1, // resolution-layer-component-precinct
		poRPCL    =  2, // resolution-precinct-component-layer
		poPCRL    =  3, // precinct-component-resolution-layer
		poCPRL    =  4  // component-precinct-resolution-layer
	} ProgressionOrder;

	// �������������� �������� ������������
	typedef enum EColorSpace
	{
		csUnknown = -1, // �������� ������������ �������������
		csRGB     =  1, // sRGB
		csGray    =  2, // Grayscale
		csYUV     =  3  // YUV
	} ColorSpace;

	// �������������� �������
	typedef enum ECodecFormat
	{
		codecUnknown = -1, // ����������� �����
		codecJ2K     =  0, // JPEG-2000 codestream : ������/������
		codecJPT     =  1, // JPT-stream (JPEG 2000, JPIP) : ������ ������
		codecJP2     =  2, // JPEG-2000 file format (Jp2-wrapper) : ������/������
		codecMj2     =  3, // Motion JPEG-2000
	} CodecFormat;

	// ����������� �� ������������� Jpeg2000-Codestream
	typedef enum ELimitDecoding
	{
		declimNoLimitation      = 0, // ��� ����������� ��� �������������. ���������� Jpeg2000-codestream ���������
		declimLimitToMainHeader = 1	 // ���������� ������ Main Header
	} LimitDecoding;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Event (��������� ���������)
	//-------------------------------------------------------------------------------------------------------------------------------

#define EVT_ERROR	1 // Error
#define EVT_WARNING	2 // Warning
#define EVT_INFO	4 // Debug event

#define MSG_SIZE  512 // ������������ ������ ���������

#define JP2_ERROR_NO_ERROR           0x0000
#define JP2_ERROR_NOT_ENOUGH_MEMORY  0x0001


	//-------------------------------------------------------------------------------------------------------------------------------
	// ��������� ������ (����������/������������)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Progression order changes (POC) (Annex A.6.6 Table A-32)
	typedef struct TPOC
	{
		int              nRSpoc;         // Resolution index for the start of a progression
		int              nCSpoc;         // Component index for the start of a progression.
		int              nLYEpoc;        // Layer index for the end of a progression
		int              nREpoc;         // Resolution index for the end of a progression
		int              nCEpoc;         // Component index for the end of a progression
		ProgressionOrder ePpoc;          // Progression order.
		int              nTile;
		char             sProgOrder[4];
	} POC;

	// ��������� ����������
	typedef struct TEncoderParams
	{
		bool             bTileSizeOn;          // False: ������� ������ �� �������, true: ������� ������ �������
		int              nXTOsiz;              // XTOsiz
		int              nYTOsiz;              // YTOsiz
		int              nXTsiz;               // XTsiz
		int              nYTsiz;               // YTsiz
		int              nDistoAlloc;          // allocation by rate/distortion
		int              nFixedAlloc;          // allocation by fixed layer
		int              nFixedQuality;        // fixed_quality
		int             *pMatrix;              // fixed layer
		char            *sComment;             // �����������
		int              nCodingStyle;         // CodingStyle
		ProgressionOrder eProgOrder;           // Progression order (default LRCP)
		POC              aoPOC[32];            // Progression order changes
		int              nPOCsCount;           // ���������� progression order changes (POC), default to 0
		int              nLayersCount;         // ���������� �������
		float            afRates[100];         // ������ ����
		float            afDistoratio[100];    // ������ ���������
		int              nResolutionsCount;    // ����� ����������(������� ����������)
		int              nCodeBlockWidthInit;  // �������� ������ ����� (�� ��������� 64)
		int              nCodeBlockHeightInit; // �������� ������ ����� (�� ��������� 64)
		int              nCodeBlockStyle;      // CodeBlockStyle
		int              nTransformId;         // 1: ����������� ������� �������������� ( Irreversible DTW ), 0: ���������� ��� ������ (�� ���������)
		int              nROIComponentIndex;   // Region of interest: ������������� ���������� [0..3], -1 ��������, ��� ROI �����������
		int              nROIShift;            // Region of interest: shift
		int              nPrecinctCount;                     // ���������� ��������
		int              anPrecinctWidthInit[J2K_MAXRLVLS];  // �������� ������ ��������
		int              anPrecinctHeightInit[J2K_MAXRLVLS]; // �������� ������ ��������

		int              nComponentsCount;       // ���������� ��������� � �������� �����
		char             sInFile[MAX_PATH_LEN];  // ��� ��������� �����
		char             sOutfile[MAX_PATH_LEN]; // ��� ��������������� �����
		int              nIndexOn;               // ������� �� ���� � ����������� (�� ��������� 0)
		char             sIndex[MAX_PATH_LEN];   // ���� ����������
		int              nImageOffsetX0;         // ���� �������� �������� �� �
		int              nImageOffsetY0;         // ���� �������� �������� �� Y
		int              nSubSamplingDx;         // SubSampling X         
		int              nSubSamplingDy;         // SubSampling Y
		int              nDecoderFormat;         // ������ ��������� �����. 0: PGX, 1: PxM, 2: BMP
		int              nEncoderFormat;         // ������ ��������� �����. 0: J2K, 1: JP2, 2: JPT
	} EncoderParams;

	// ��������� ������������
	typedef struct TDecoderParams
	{
		int           nReduce;                // ���� != 0, ����� ���������� ��������� ����������� ������� �� 2^(nReduce). ���� == 0, ��� �� ������������, ����� ����������� ������������ � ������ �����������.
		int           nLayer;                 // ������������� ������������ ����� ������, ������� �� ����� ������������.
		LimitDecoding eLimitDecoding;         // ����� ������������ ��������� ����, ��� ������ ���������.

		char          sInFile[MAX_PATH_LEN];  // ��� ��������� �����
		char          sOutfile[MAX_PATH_LEN]; // ��� ��������������� �����
		int           nDecoderFormat;         // ������ ��������� �����. 0: J2k, 1: Jp2, 2: Jpt
		int           nEncoderFormat;         // ������ ��������� �����. 0: PGX, 1: PxM, 2: BMP
	} DecoderParams;

	// ����� ���� ��� JPEG-2000 ���������� � �����������
	//EventManager *pEventManager;   /* ��������� �� ���������� ��������� (events) */\
		//void         *pClientData;     /* */\

#define CommonFields \
	bool          bIsDecompressor; /* ����� ������ ���������� ��� ������������ */\
	ECodecFormat  eCodecFormat;	   /* ��������� ����� */\
	void         *pJ2k;            /* ��������� �� J2K ����� */\
	void         *pJp2;            /* ��������� �� JP2 ����� */\
	void         *pMj2;            /* ��������� �� JP2 ����� */\
	int           nErrorCode       /* ����� ������ */

	// ����, ������� ����������� ��� ��� ������, ��� � ��� ������
	typedef struct TCommonStruct
	{
		CommonFields;
	} CommonStruct;

	typedef CommonStruct* PCommon;

	// ��������� ����������
	typedef struct TCInfo
	{
		CommonFields; // ����� ����
		// ���� ����������� ������ ��� ����������
	} CInfo;

	// ��������� ������������
	typedef struct TDInfo
	{
		CommonFields; // ����� ����
		// ���� ����������� ������ ��� ������������
	} DInfo;


	//-------------------------------------------------------------------------------------------------------------------------------
	// Image
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TImageComponent
	{
		int  nXRsiz;           // XRsiz: �������������� ���������� i-�� ����������
		int  nYRsiz;           // YRsiz: ������������ ���������� i-�� ����������
		int  nWidth;           // ������ ��������
		int  nHeight;          // ������ ��������
		int  nXOsiz;           // ����� ���������� �� � �� ��������� �� ����� �����������
		int  nYOsiz;           // ����� ���������� �� � �� ��������� �� ����� �����������
		int  nPrecision;       // Precision
		int  nBPP;             // Bits Per Pixel
		int  nSigned;          // Signed - 1, unsigned - 0
		int  nDecodedResCount; // ���������� Resolutions � ������������ �����������
		int  nFactorDiv2;      // ���������� ������� �� 2 �������������� ��������, �� ��������� � �������� ��������
		int *pData;            // ������ ����������� ��� i-�� ����������
	} ImageComponent;

	typedef struct TImage
	{
		int             nXOsiz;      // XOsiz: �������������� c���� �� ��������� ����� ���� ����� � ������� � �������� ������������
		int             nYOsiz;      // YOsiz: ������������ ����� �� ��������� ����� ���� ����� � ������� � �������� ������������
		int             nXsiz;       // Xsiz: ������ �����
		int             nYsiz;       // Ysiz: ������ �����
		int             nCsiz;       // Csiz: ���������� ��������� � �����������
		ColorSpace      eColorSpace; // �������� ������������: sRGB, Greyscale ��� YUV
		ImageComponent *pComponents; // ���������� �����������
	} Image;

	typedef struct TImageComponentParams
	{
		int nXRsiz;     // XRsiz: �������������� ���������� i-�� ����������
		int nYRsiz;     // YRsiz: ������������ ���������� i-�� ����������
		int nWidth;     // ������
		int nHeight;    // ������ ��������
		int nXoffset;   // ����� ���������� �� � �� ��������� �� ����� �����������
		int nYoffset;   // ����� ���������� �� � �� ��������� �� ����� �����������
		int nPrecision; // Precision
		int nBPP;       // Bits Per Pixel
		int nSigned;    // Signed - 1, unsigned - 0
	} ImageComponentParams;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Tag-tree coder (TagTree.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Tag node
	typedef struct TTagNode
	{
		struct TTagNode *pParent;
		int              nValue;
		int              nLow;
		int              nKnown;
	} TagNode;

	// Tag tree
	typedef struct TTagTree
	{
		int      nLeafsHorCount;
		int      nLeafsVerCount;
		int      nNodesCount;
		TagNode *pNodes;
	} TagTree;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  JPEG2000 codestream (J2k.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define J2K_CP_CSTY_PRT 0x01 // CodingParams - CodingStyle - Partition (Tables A-20, A-23)
#define J2K_CP_CSTY_SOP 0x02
#define J2K_CP_CSTY_EPH 0x04

#define J2K_CCP_CSTY_PRT 0x01

#define J2K_CCP_CBLKSTY_LAZY    0x01
#define J2K_CCP_CBLKSTY_RESET   0x02
#define J2K_CCP_CBLKSTY_TERMALL 0x04
#define J2K_CCP_CBLKSTY_VSC     0x08
#define J2K_CCP_CBLKSTY_PTERM   0x10
#define J2K_CCP_CBLKSTY_SEGSYM  0x20

#define J2K_CCP_QNTSTY_NOQNT 0 // ComponentCodingParams - Quantization style - No quantization (Tables A-28, A-29)
#define J2K_CCP_QNTSTY_SIQNT 1 // ComponentCodingParams - Quantization style - Scalar implicit (Tables A-28, A-30)
#define J2K_CCP_QNTSTY_SEQNT 2 // ComponentCodingParams - Quantization style - Scalar explicit (Tables A-28, A-30)

	//-------------------------------------------------------------------------------------------------------------------------------

	// Marker Segments (Annex A.2 Table A-2)
#define J2K_MS_SOC 0xff4f	// SOC
#define J2K_MS_SOT 0xff90	// SOT
#define J2K_MS_SOD 0xff93	// SOD
#define J2K_MS_EOC 0xffd9	// EOC
#define J2K_MS_SIZ 0xff51	// SIZ
#define J2K_MS_COD 0xff52	// COD
#define J2K_MS_COC 0xff53	// COC
#define J2K_MS_RGN 0xff5e	// RGN
#define J2K_MS_QCD 0xff5c	// QCD
#define J2K_MS_QCC 0xff5d	// QCC
#define J2K_MS_POC 0xff5f	// POC
#define J2K_MS_TLM 0xff55	// TLM
#define J2K_MS_PLM 0xff57	// PLM
#define J2K_MS_PLT 0xff58	// PLT
#define J2K_MS_PPM 0xff60	// PPM
#define J2K_MS_PPT 0xff61	// PPT
#define J2K_MS_SOP 0xff91	// SOP
#define J2K_MS_EPH 0xff92	// EPH
#define J2K_MS_CRG 0xff63	// CRG
#define J2K_MS_COM 0xff64	// COM
	//////////////////////////////////////////////////
#define J2K_MS_EPC 0xff68	/**< EPC marker value (Part 11: JPEG 2000 for Wireless) */
#define J2K_MS_EPB 0xff66	/**< EPB marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_ESD 0xff67	/**< ESD marker value (Part 11: JPEG 2000 for Wireless) */ 
#define J2K_MS_RED 0xff69	/**< RED marker value (Part 11: JPEG 2000 for Wireless) */
#define J2K_MS_SEC 0xff65    /**< SEC marker value (Part 8: Secure JPEG 2000) */
#define J2K_MS_INSEC 0xff94  /**< INSEC marker value (Part 8: Secure JPEG 2000) */

	//-------------------------------------------------------------------------------------------------------------------------------

	// �������� ������������ ������ �������������(����� ������������ Main Header)
	typedef enum EJ2kStatus
	{
		j2kstateMHSOC  = 0x0001, // SOC ������
		j2kstateMHSIZ  = 0x0002, // SIZ ������
		j2kstateMH     = 0x0004, // ������� ������������� ��������� � Main Header
		j2kstateTPHSOT = 0x0008, // ������� ������������� ��������� � Tile Part Header, � ������ �������� ������ SOT
		j2kstateTPH    = 0x0010, // ������� ������������� ��������� � Tile Part Header
		j2kstateMT     = 0x0020, // ������ ��� ��� �������� ������ EOC
		j2kstateNEOC   = 0x0040  // ��� ������������� �� ������������ ������ EOC, ������ ��� �odestream �����������
	} J2kStatus;

	//-------------------------------------------------------------------------------------------------------------------------------

	// ������ ���� ����������� 
	typedef struct TQuantStepSize
	{
		int nExponent;
		int nMantissa;
	} QuantStepSize;

	// Tile-component coding parameters (Annex A.6.2 Table A-23)
	typedef struct TTileCompCodingParams
	{
		int           nCodingStyle;                   // Coding style
		int           nResolutionsCount;              // Decomposition levels
		int           nCodeBlockWidth;                // Code-block size width
		int           nCodeBlockHeight;               // Code-block size height
		int           nCodeBlockStyle;                // Code-block context
		int           nTransformID;                   // Wavelet-�������������
		int           nQuantStyle;                    // ��� �����������
		QuantStepSize aoStepSizes[J2K_MAXBANDS];      // ������ ����, ������������ ��� �����������
		int           nGuardBitsCount;                // Number of guard bits
		int           nROIShift;                      // SPrgn: Region Of Interest shift (Table A-25)
		int           anPrecinctWidth[J2K_MAXRLVLS];  // Precinct width
		int           anPrecinctHeight[J2K_MAXRLVLS]; // Precinct height	
	} TileCompCodingParams;

	// Tile coding parameters : ������ ��������� ������������ ��� �������� ���������� �����������/������������� ���������� ��� ���� 
	// �����(tiles) ( ������� ���������� COD, COC � Main Header )
	typedef struct TTileCodingParams
	{
		int                   nFirst;            // ����. 1 : ������ part-tile ������� tile
		int                   nCodingStyle;      // Scod: Coding style (Table A-12)
		ProgressionOrder      eProgOrder;        // SPcod, Ppod: Progression order
		int                   nLayersCount;      // SPcod: Number of layers
		int                   nMCT;              // SPcod: Multiple component transform
		float                 afRates[100];      // ������� layers
		int                   nPOCsCount;        // ���������� POC ( progression order changes )
		int                   nUsePOC;           // ����; ���������� �� ������ POC. 0 - ���, 1 - ��.
		POC                   aoPOC[32];         // Progression order changes
		unsigned char        *pPPTData;          // ������ ��������� �� Packet header, ����� ������������ ��� � Tire2 �������������
		unsigned char        *pPPTDataFirst;     // ��������� �� ������ ������� ������ � PacketHeader, ���� PPT ������������
		int                   nPPT;              // ����. ���� ��� �������� Tile ����� ������ PPT, ����� ���� ����� 1.
		int                   nPPTStore;         // ����������, ���� ����� ��������� �������� PPT (������ �� ����� ����� � ������ �����)
		int                   nPPTLength;        // PPM bug 1
		float                 afDistoRatio[100]; // ��������� Fixed quality
		TileCompCodingParams *pTCCP;             // Tile-component coding parameters
	} TileCodingParams;

	// Coding parameters
	typedef struct TCodingParams
	{
		int               nDistoAlloc;    // allocation by rate/distortion
		int               nFixedAlloc;    // allocation by fixed layer
		int               nFixedQuality;  // ��������� Fixed quality
		int               nReduceFactor;  // ���� ������ �������� ������� �� 0, ����� �������� ���������� ����� �� 2^(nReduceFactor); ���� ����� 0, ��� �� ������������, ���������� ����������� � ������ �����������
		int               nLayerFactor;   // ���� ������ �������� ������� �� 0, ����� ���������� ������ ������ "layer"; ���� ����� 0, ��� �� ������������, ���������� ��� "layers"
		LimitDecoding     eLimitDecoding; // ����������� �� �������������.
		int               nIndexOn;       // 0 - ��� ���������, 1 - ����.
		int               nXTOsiz;        // XTOsiz: ����� �� ����������� ������� Tile
		int               nYTOsiz;        // YTOsiz: ����� �� ��������� ������� Tile
		int               nXTsiz;         // XTsiz: ������ ������ Tile �� ��������� � ����� �����
		int               nYTsiz;         // YTsiz: ������ ������ Tile �� ��������� � ����� �����
		char             *sComment;       // �����������
		int               nXTilesCount;   // ���������� Tiles �� �����������
		int               nYTilesCount;   // ���������� Tiles �� ���������
		int              *pTileIndex;     // ���������� ������ Tiles � codestream
		int               nTileIndexSize; // ������ ������� pTileIndex
		unsigned char    *pPPMData;       // ������ ��������� �� Packet header, ����� ������������ ��� � Tire2 �������������
		unsigned char    *pPPMDataFirst;  // ��������� �� ������ ������� ������ � PacketHeader, ���� PPM ������������
		int               nPPM;           // ����. ���� ��� �������� Tile ����� ������ PPM, ����� ���� ����� 1.
		int               nPPMStore;      // ����������, ���� ����� ��������� �������� PPM (������ �� ����� ����� � ������ �����)
		int               nPPMPrevious;   // ����������, ���� ����� ��������� �������� PPM (������, ���� ���������� ������ PPM ��� ��� �� ��������)
		int               nPPMLength;     // PPM bug 1
		TileCodingParams *pTCP;           // Tile coding parameters
		int              *pMatrix;        // Fixed layer
	} CodingParams;

	// ���������� ������������ ������ � Tile
	typedef struct TPacketInfo
	{
		int    nStartPos; // ��������� �������
		int    nEndPos;   // �������� �������
		double dDisto;
	} PacketInfo;

	// ����������, ����������� � Tiles ������ �����������
	typedef struct TTileInfo
	{
		double     *pdThreshold;          // �������� ������ ��� ������� layer
		int         nTileCount;           // ����� ���������� Tiles
		int         nStartPos;            // ��������� �������
		int         nEndHeader;           // �������� ������� ��� ���������
		int         nEndPos;              // �������� �������
		int         anXPrecinctCount[33]; // ���������� �������� (precinct) �� � ��� ������� ������ ���������� (resolution level)
		int         anYPrecinctCount[33]; // ���������� �������� (precinct) �� Y ��� ������� ������ ���������� (resolution level)
		int         anPrecinctWidth[33];  // ������ ������� (� ������� 2), ��� �������� ������ ����������
		int         anPrecinctHeight[33]; // ������ ������� (� ������� 2), ��� �������� ������ ����������
		PacketInfo *pPacket;              // PacketInfo
		int         nNBPix;               // ��������� Fixed Quality
		double      dDistoTile;           // ��������� Fixed Quality
	} TileInfo;

	typedef struct TImageInfo
	{
		int              nIndexOn;        // 0 - ��� ���������, 1 - ����.
		double           dDistoMax;       // ������������ ������� ���������� ��������� ��� ����� �����������
		int              nPacketCount;    // ���������� �������
		int              nIndexWrite;     //
		int              nImageWidth;     // ������ ��������
		int              nImageHeight;    // ������ ��������
		ProgressionOrder eProgOrder;      // Progression Order
		int              nXTsiz;          // ������ tile �� �
		int              nYTsiz;          // ������ tile �� Y
		int              nXTOsiz;         // ����� ������� ����� �� �
		int              nYTOsiz;         // ����� ������� ����� �� Y
		int              nXTilesCount;    // ���������� ������ �� �
		int              nYTilesCount;    // ���������� ������ �� Y
		int              nCompCount;      // ���������� ���������
		int              nLayersCount;    // ���������� layers
		int              nDecompCount;    // ���������� ������� ����������
		int              nMainHeadEnd;    // ������� ��������� Main Header
		int              nCodestreamSize; // ������ codestream
		TileInfo        *pTile;           // TileInfo
	} ImageInfo;

	// JPEG-2000 codestream reader/writer
	typedef struct TJ2kCodestream
	{
		PCommon           pCodecInfo;     // ����� ���������� ��� ����������� � �������������
		int               nState;         // ����� ������ ������� ������� �������� Codestream'a ( Main header, Tile header, ����� ������������� )
		int               nCurTileIndex;  // ����� �������� ����� (�.�. ������� � ������ ������ ����������/������������)
		//unsigned char    *pEndOfTile;     // ������������ ��� ����������� ������� ����� ����� � codestream (����� ���������� ���������� �� codestream)
		int				  nSizeOfTile;
		int               nSOTStartPos;   // ��������� ��������� ������� ������� SOT, ����� ����� ����������� �������� ����� �� ����� ��������� � �������� ����� ������� �������
		int               nSODStartPos;   // ����������, ��� ������� SOD
		int               nPosCorrection; // ���������� ��� ������������� �������, ��������� J2k-���� ������� � ��������� ������, �� ����� �����������
		unsigned char   **ppTileData;     // ������ � ������� ���� ������
		int              *pTileLen;       // ������, �������� ������ ������ ��� ������� �����
		TileCodingParams *pDefaultTCP;    // ������������ ������ ��� ������������: ������ ����� ���������� ��� ���� ������ ( COD, COC )
		Image            *pImage;         // ��������� �� �����������, ������� �� ���� ��������, ���� ����������
		CodingParams     *pCodingParams;  // ��������� �� ���������, �������� ���������� � �����������
		ImageInfo        *pImageInfo;     // 
		CReader           *pStreamIO;      // Input / Output stream
	} J2kCodestream;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Tile Coder/Decoder (Tile.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTCDSegment
	{
		int            nPassesCount;
		int            nLength;
		unsigned char *pData;
		int            nMaxPasses;
		int            nNewPassesCount;
		int            nNewLength;
	} TCDSegment;

	typedef struct TTCDPass
	{
		int            nRate;
		double         dDistortionDec;
		int            nTerm;
		int            nLen;
	} TCDPass;

	typedef struct TLayer
	{
		int            nPassesCount; // ���������� ��������
		int            nLen;         // ����� ����������
		double         dDisto;       // ����������
		unsigned char *pData;        // ������
	} Layer;

	typedef struct TCodeBlock
	{
		int           nX0;                  // ������� CodeBlock
		int           nY0;                  // (x0, y0) - ����� ������� ����
		int           nX1;                  // (�1, �1) - ������ ������ ����
		int           nY1;                  // 
		int           nBPSCount;            //
		int           nLenBitsCount;        //
		int           nLen;                 // �����
		int           nPassesCount;         // ���������� ��� ����������� �������� ��� ������� CodeBlock
		int           nNewPassesCount;      // ���������� �������� ����������� � ������� CodeBlock
		int           nSegmentsCount;       // ���������� ���������
		TCDSegment    aSegments[100];		// ���� ��������
		unsigned char  aData[8192];			 // ������
		int           nPassesCountInLayers; // ���������� �������� � �����
		Layer         aLayers[100];				// ���������� � �������	
		int           nTotalPassesCount;    // ����� ����� ��������
		TCDPass       aPasses[100];         // ���������� � ��������
	} CodeBlock;

	typedef struct TPrecinct
	{
		int        nX0;            // ������� Precinct
		int        nY0;            // (x0, y0) - ����� ������� ����
		int        nX1;            // (�1, �1) - ������ ������ ����
		int        nY1;            // 
		int        nCountInWidth;  // ���������� ������ � ������
		int        nCountInHeight; // ���������� ������ � ������
		CodeBlock *pCodeBlocks;    // CodeBlocks
		TagTree   *pInclTree;      // ������ ���������
		TagTree   *pIMSBTree;      // ������ IMSB
	} Precinct;

	typedef struct TBand
	{
		int       nX0;        // ������� Band
		int       nY0;        // (x0, y0) - ����� ������� ����
		int       nX1;        // (�1, �1) - ������ ������ ����
		int       nY1;        // 
		int       nBandIndex; // ����� �������� Band
		Precinct *pPrecincts; // ���������� � ��������(Precinct), �������� � ������ Band
		int       nBPSCount;  //
		float     fStepSize;  //
	} Band;

	typedef struct TResolution
	{
		int  nX0;         // ������� Resolution
		int  nY0;         // (x0, y0) - ����� ������� ����
		int  nX1;         // (�1, �1) - ������ ������ ����
		int  nY1;         // 
		int  nPrecCountW; // ���������� �������� (Precinct) �� ������
		int  nPrecCountH; // ���������� �������� (Precinct) �� ������
		int  nBandsCount; // ���������� Sub-band
		Band aBands[3];   // Sub-bands
	} Resolution;

	typedef struct TTileComp
	{
		int         nX0;               // ������� TileComp 
		int         nY0;               // (x0, y0) - ����� ������� ����
		int         nX1;               // (�1, �1) - ������ ������ ����
		int         nY1;               // 
		int         nResolutionsCount; // ����� ����������
		Resolution *pResolutions;      // ���������� (Resolutions)
		int        *pData;             // ������ ����������
		int         nNBPix;            // Fixed_quality
	} TileComp;

	typedef struct TTile
	{
		int       nX0;               // ������� Tile
		int       nY0;               // (x0, y0) - ����� ������� ����
		int       nX1;               // (�1, �1) - ������ ������ ����
		int       nY1;               // 
		int       nComponentsCount;  // ���������� ���������
		TileComp *pComponents;       // ����������
		int       nNBPix;            // Fixed_quality
		double    dDistoTile;        // Fixed_quality
		double    adDistoLayer[100]; // Fixed_quality
	} Tile;

	typedef struct TTCDImage
	{
		int   nTilesCountW; // ���������� ������ �� ������
		int   nTilesCountH; // ���������� ������ �� ������
		Tile *pTiles;       // �����
	} TCDImage;

	typedef struct TTCD
	{
		PCommon           pCodecInfo;    // ���������� � ������
		TCDImage         *pTCDImage;     // ������������ �����������
		Image            *pImage;        // �����������
		CodingParams     *pCodingParams; // ���������
		Tile             *pTCDTile;      // ��������� �� ������� ����������/������������ ����
		TileCodingParams *pTCP;          // ��������� �����������/������������� ��� ���� ������
		int               nTCDTileIndex; // ����� �������� �����������/������������� �����
	} TCD;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Jp2 file format (Jp2.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define JPIP_JPIP 0x6a706970

	// Jp2 boxes (Table I-2)
#define JP2_JP   0x6a502020 // JPEG 2000 signature box
#define JP2_PRFL 0x7072666c // Profile box
#define JP2_FTYP 0x66747970 // File type box
#define JP2_JP2H 0x6a703268 // JP2 header box
#define JP2_IHDR 0x69686472 // Image header box
#define JP2_COLR 0x636f6c72 // Colour specification box
#define JP2_JP2C 0x6a703263 // Contiguous codestream box
#define JP2_URL  0x75726c20 // URL box
#define JP2_DBTL 0x6474626c // ???
#define JP2_BPCC 0x62706363 // Bits per component box
#define JP2_JP2  0x6a703220 // File type fields
#define JP2_CDEF 0x63646566 // Component Definition box

	//-------------------------------------------------------------------------------------------------------------------------------

	// Jp2 component
	typedef struct TJp2Component
	{
		int nDepth;
		int nSigned;
		int nBPCC;
	} Jp2Component;

	typedef struct TJp2Color
	{
		BYTE *icc_profile_buf;
		DWORD icc_profile_len;

		//opj_jp2_cdef_t *jp2_cdef;
		//opj_jp2_pclr_t *jp2_pclr;

		BOOL has_colr;
	} Jp2Color;

	// Jp2 file format reader/writer
	typedef struct TJp2Stream
	{
		PCommon        pCodecInfo;       // ����� ���������� ��� ����������� � �������������
		J2kCodestream *pJ2k;             // ��������� �� ��������� ��� ������/������ JPEG2000 Codestream

		unsigned int   nWidth;           // ������ �����������
		unsigned int   nHeight;          // ������ �����������
		unsigned int   nComponentsCount; // ����� ���������
		unsigned int   nBPC;             // Bits per component
		unsigned int   nCompressionType; // CompressionType
		unsigned int   nColorSpaceUnk;   // Colourspace Unknown
		unsigned int   nIPR;             // Intellectual Property

		unsigned int   nMethod;          // Color specification method
		unsigned int   nPrec;            // Precedence
		unsigned int   nApprox;          // Colourspace approximation
		unsigned int   nEnumCS;          // Enumerated colourspace

		unsigned int   nBrand;           // Brand
		unsigned int   nMinVersion;      // MinVersion
		unsigned int   nCompListLength;  // ����� ������ Compatibility list
		unsigned int  *pCompList;        // Compatibility list

		Jp2Component  *pComponents;
		unsigned int   nJ2kCodestreamOffset;
		unsigned int   nJ2kCodestreamLength;

		Jp2Color	   oColor;
	} Jp2Stream;

	// Jp2 Box
	typedef struct TJp2Box
	{
		int nLength;
		int nType;
		int nInitPos;
	} Jp2Box;

	//-------------------------------------------------------------------------------------------------------------------------------
	// MQ-coder (ArithmeticCoder.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// ��. Annex C. fcd15444-1.pdf ( MQ-coder Encoder/Decoder )

#define MQC_NUMCTXS 32

	typedef struct TMQCoderState
	{
		unsigned int          nLPSValue; // ����������� ��� ����������� �������������� ������� (Least Probable Symbol) (0.75->0x8000, 1.5->0xffff)
		int                   nMPS;      // ���������� ������������� ������ ( 0 ��� 1 ) (Most Probable Symbol)
		struct TMQCoderState *pNextMPS;  // ��������� �� ��������� ���������, ���� ��������� �������������� ������ MPS
		struct TMQCoderState *pNextLPS;  // ��������� �� ��������� ���������, ���� ��������� �������������� ������ LPS
	} MQCoderState;

	// MQ-coder
	typedef struct TMQCoder
	{
		unsigned int   nCregister;
		unsigned int   nAregister;
		unsigned int   nCt;

		unsigned char *pBufferPointer;
		unsigned char *pStart;
		unsigned char *pEnd;
		MQCoderState  *pContexts[MQC_NUMCTXS];
		MQCoderState **ppCurContext;
	} MQCoder;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Raw (Raw.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TRaw
	{
		unsigned char  nBuffer;        // ��������� �����, � ������� ���������� ��������� �����������/�������������
		unsigned int   nFreeBitsCount; // ���������� ���, ��������� ��� ������ ��� ������
		unsigned int   nMaxLen;        // ������������ ����� ��� �������������
		unsigned int   nLen;           // ����� �������������� ����������
		unsigned char *pBufferPointer; // ��������� �� ������� ������� � ������
		unsigned char *pBufferStart;   // ��������� �� ������ ������
		unsigned char *pBufferEnd;     // ��������� �� ����� ������
	} Raw;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Jpt (Jpt.h)
	//-------------------------------------------------------------------------------------------------------------------------------

	// Message Header JPT stream structure
	typedef struct TJPTMessageHeader
	{
		unsigned int nId;            // In-class Identifier
		unsigned int nLastByte;      // Last byte information
		unsigned int nClassId;       // Class Identifier
		unsigned int nCSnId;         // CSn : index identifier
		unsigned int nMessageOffset; // Message offset
		unsigned int nMessageLength; // Message length
		unsigned int nLayerNB;       // Auxiliary for JPP case
	} JPTMessageHeader;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Packet Iterator 
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TPacketResolution
	{
		int nDx;
		int nDy;
		int nWidth;
		int nHeight;
	} PacketResolution;

	typedef struct TPacketComponent
	{
		int               nDx;
		int               nDy;
		int               nResolutionsCount;
		PacketResolution *pResolutions;
	} PacketComponent;

	typedef struct TPacketIterator
	{
		short int       *pInclude;         // ����, ��������������� ������������� �� ������ ����� (������������ � POC)
		int              nStepLayer;       // LayerStep ������������ ��� ����������� ������ � pInclude
		int              nStepResolution;  // ResoultionStep ������������ ��� ����������� ������ � pInclude
		int              nStepComponent;   // ComponentStep ������������ ��� ����������� ������ � pInclude
		int              nStepPrecinct;    // PrecinctStep ������������ ��� ����������� ������ � pInclude

		int              nIndexComponent;  // ����� ���������� ��� ����������� ������
		int              nIndexResolution; // ����� ������ ����������� ��� ����������� ������
		int              nIndexPrecinct;   // ����� ������� ��� ����������� ������
		int              nIndexLayer;      // ����� ������ ��� ����������� ������  

		int              nFirst;           // ����, ������������ ������ �� ��� �����
		POC              oPOC;             // Progression Order Change
		int              nComponentsCount; // ����� ���������

		PacketComponent *pComponents;
		int              nTileX0;          //
		int              nTileY0;          // ������� �������� �����
		int              nTileX1;          //
		int              nTileY1;          //
		int              nX;
		int              nY;
		int              nDx;
		int              nDy;
	} PacketIterator;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Tier-1 coding (����������� �������������)
	//-------------------------------------------------------------------------------------------------------------------------------

#define T1_NMSEDEC_BITS 7

#define T1_MAXCBLKW 1024	// ������������ ������ CodeBlock (�� ������)
#define T1_MAXCBLKH 1024	// ������������ ������ CodeBlock (�� ������)

#define T1_SIG_NE 0x0001	// ���������� ��������� : ������-��������� �����������
#define T1_SIG_SE 0x0002	// ���������� ��������� : ���-��������� �����������
#define T1_SIG_SW 0x0004	// ���������� ��������� : ���-�������� �����������
#define T1_SIG_NW 0x0008	// ���������� ��������� : ������-�������� �����������
#define T1_SIG_N  0x0010	// ���������� ��������� : �������� �����������
#define T1_SIG_E  0x0020	// ���������� ��������� : ��������� �����������
#define T1_SIG_S  0x0040	// ���������� ��������� : ����� �����������
#define T1_SIG_W  0x0080	// ���������� ��������� : �������� �����������
#define T1_SIG_OTH  (T1_SIG_N|T1_SIG_NE|T1_SIG_E|T1_SIG_SE|T1_SIG_S|T1_SIG_SW|T1_SIG_W|T1_SIG_NW)
#define T1_SIG_PRIM (T1_SIG_N|T1_SIG_E|T1_SIG_S|T1_SIG_W)

#define T1_SGN_N 0x0100
#define T1_SGN_E 0x0200
#define T1_SGN_S 0x0400
#define T1_SGN_W 0x0800
#define T1_SGN (T1_SGN_N|T1_SGN_E|T1_SGN_S|T1_SGN_W)

#define T1_SIG    0x1000
#define T1_REFINE 0x2000
#define T1_VISIT  0x4000

#define T1_NUMCTXS_ZC 9
#define T1_NUMCTXS_SC 5
#define T1_NUMCTXS_MAG 3
#define T1_NUMCTXS_AGG 1
#define T1_NUMCTXS_UNI 1

#define T1_CTXNO_ZC 0
#define T1_CTXNO_SC  (T1_CTXNO_ZC  + T1_NUMCTXS_ZC )
#define T1_CTXNO_MAG (T1_CTXNO_SC  + T1_NUMCTXS_SC )
#define T1_CTXNO_AGG (T1_CTXNO_MAG + T1_NUMCTXS_MAG)
#define T1_CTXNO_UNI (T1_CTXNO_AGG + T1_NUMCTXS_AGG)
#define T1_NUMCTXS   (T1_CTXNO_UNI + T1_NUMCTXS_UNI)

#define T1_NMSEDEC_FRACBITS (T1_NMSEDEC_BITS - 1)

#define T1_TYPE_MQ  0  // ���������� �����������, � �������������� Arithmetic entropy coding (Annex C)
#define T1_TYPE_RAW 1  // ��� �������� �����������, ���������� � Codestream ���������� � ����� ����

	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTier1
	{
		PCommon  pCodecInfo;  // ���������� � ������
		MQCoder *pMQCoder;    // ��������� �� ��������� ��� ������/������ MQ-������������ ������
		Raw     *pRaw;        // ��������� �� ��������� ��� ������/������ �������������� ������

		int      lut_ctxno_zc[1024];
		int      lut_ctxno_sc[256];
		int      lut_ctxno_mag[4096];
		int      lut_spb[256];
		int      lut_nmsedec_sig[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_sig0[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_ref[1 << T1_NMSEDEC_BITS];
		int      lut_nmsedec_ref0[1 << T1_NMSEDEC_BITS];

		int      aData[T1_MAXCBLKH][T1_MAXCBLKW];          // ������ codeblock
		int      aFlags[T1_MAXCBLKH + 2][T1_MAXCBLKH + 2]; // �����

	} Tier1;

	//-------------------------------------------------------------------------------------------------------------------------------
	//  Tier-2 coding
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TTier2
	{
		PCommon       pCodecInfo;
		Image        *pImage;        // Encoding: ��������� �� �������� ��������. Decoding: ��������� �� �������������� ��������.
		CodingParams *pCodingParams;
	} Tier2;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Mj2 file format (Mj2.h)
	//-------------------------------------------------------------------------------------------------------------------------------

#define MJ2_JP    0x6a502020
#define MJ2_FTYP  0x66747970
#define MJ2_MJ2   0x6d6a7032
#define MJ2_MJ2S  0x6d6a3273
#define MJ2_MDAT  0x6d646174
#define MJ2_MOOV  0x6d6f6f76
#define MJ2_MVHD  0x6d766864
#define MJ2_TRAK  0x7472616b
#define MJ2_TKHD  0x746b6864
#define MJ2_MDIA  0x6d646961
#define MJ2_MDHD  0x6d646864
#define MJ2_MHDR  0x6d686472
#define MJ2_HDLR  0x68646C72
#define MJ2_MINF  0x6d696e66
#define MJ2_VMHD  0x766d6864
#define MJ2_SMHD  0x736d6864
#define MJ2_HMHD  0x686d6864
#define MJ2_DINF  0x64696e66
#define MJ2_DREF  0x64726566
#define MJ2_URL   0x75726c20
#define MJ2_URN   0x75726e20
#define MJ2_STBL  0x7374626c
#define MJ2_STSD  0x73747364
#define MJ2_STTS  0x73747473
#define MJ2_STSC  0x73747363
#define MJ2_STSZ  0x7374737a
#define MJ2_STCO  0x7374636f
#define MJ2_MOOF  0x6d6f6f66
#define MJ2_FREE  0x66726565
#define MJ2_SKIP  0x736b6970
#define MJ2_JP2C  0x6a703263
#define MJ2_FIEL  0x6669656c
#define MJ2_JP2P  0x6a703270
#define MJ2_JP2X  0x6a703278
#define MJ2_JSUB  0x6a737562
#define MJ2_ORFO  0x6f72666f
#define MJ2_MVEX  0x6d766578
#define MJ2_JP2   0x6a703220
#define MJ2_J2P0  0x4a325030

	// Decompressed format used in parameters YUV = 0
#define YUV_DFMT 1 

	// Compressed format used in parameters MJ2 = 0
#define MJ2_CFMT 2


	//-------------------------------------------------------------------------------------------------------------------------------

	// Time To Sample
	typedef struct TMj2_TimeToSample
	{
		int nSampleCount;
		int nSampleDelta;
	} Mj2_TimeToSample;

	// Chunk
	typedef struct TMj2_Chunk
	{
		int nNumSamples;
		int nSampleDescriptionIndex;
		int nOffset;
	} Mj2_Chunk;

	// Sample to chunk
	typedef struct TMj2_SampleToChunk
	{
		int nFirstChunk;
		int nSamplesPerChunk;
		int nSampleDescriptionIndex;
	} Mj2_SampleToChunk;

	// Sample 
	typedef struct TMj2_Sample
	{
		unsigned int unSampleSize;
		unsigned int unOffset;
		unsigned int unSampleDelta;
	} Mj2_Sample;

	// URL
	typedef struct TMj2_Url
	{
		int anLocation[4];
	} Mj2_Url;

	// URN
	typedef struct TMj2_Urn
	{
		int anName[4];
		int anLocation[4];
	} Mj2_Urn;

	// Image Track Parameters
	typedef struct TMj2_TrackParams
	{
		PCommon           pCodecInfo;
		int               nTrackID;
		int               nTrackType;
		unsigned int      unCreationTime;
		unsigned int      unModificationTime;
		int               nDuration;
		int               nTimescale;
		int               nLayer;
		int               nVolume;
		int               nLanguage;
		int               nBalance;
		int               nMaxPDUsize;
		int               nAvgPDUsize;
		int               nMaxBitrate;
		int               nAvgBitrate;
		int               nSlidingAvgBitrate;
		int               nGraphicsMode;
		int               anOpColor[3];
		int               nNumUrl;
		Mj2_Url*          pUrl;
		int               nNumUrn;
		Mj2_Urn*          pUrn;
		int               anDim[2];
		int               nWidth;
		int               nHeight;
		int               nVisualWidth;
		int               nVisualHeight;
		int               nCbCr_SubSampling_dx;
		int               nCbCr_SubSampling_dy;
		int               nSampleRate;
		int               nSampleDescription;
		int               nHorResolution;
		int               nVerResolution;
		int               anCompressorName[8];
		int               nDepth;
		unsigned char     unFieldCount;
		unsigned char     unFieldOrder;
		unsigned char     unOr_FieldCount;
		unsigned char     unOr_FieldOrder;
		int               nNumBr;
		unsigned int*     pBr;
		unsigned char     unNumJp2x;
		unsigned char*    pJp2xData;
		unsigned char     unHsub;
		unsigned char     unVsub;
		unsigned char     unHoff;
		unsigned char     unVoff;
		int               anTransMatrix[9];
		unsigned int      unNumSamples;       // Number of samples
		int               nTransorm;
		int               nHandlerType;
		int               nNameSize;
		unsigned char     unSameSampleSize;
		int               nNumTimeToSample;
		Mj2_TimeToSample* pTimeToSample;      // Time to sample
		unsigned int      unNumChunks;
		Mj2_Chunk*        pChunk;
		int               nNumSamplesToChunk;
		Mj2_SampleToChunk*pSampleToChunk;
		char*             sName;
		Jp2Stream         oJp2;
		Mj2_Sample       *pSample;             // Sample parameters
	} Mj2_TrackParams;

	// MJ2 box
	typedef struct TMj2_Box
	{
		int nLength;
		int nType;
		int nInitPos;
	} Mj2_Box;

	// MJ2 Movie
	typedef struct TMj2_Movie
	{
		PCommon          pCodecInfo;
		J2kCodestream*   pJ2k;
		unsigned int     unBrand;
		unsigned int     unMinVersion;
		int              nCompListLength;
		unsigned int*    pCompList;
		unsigned int     unCreationTime;
		unsigned int     unModificationTime;
		int              nTimescale;
		unsigned int     unDuration;
		int              nRate;
		int              nNumVtk;
		int              nNumStk;
		int              nNumHtk;
		int              nVolume;
		int              anTransMatrix[9];
		int              nNextTrackId;
		Mj2_TrackParams* pTrack;
	} Mj2_Movie;

	// Compression parameters
	typedef struct TMj2_EncoderParameters
	{
		TEncoderParams pJ2k_Parameters;
		int            nDecoderFormat;       // input file format 0:MJ2
		int            nEncoderFormat;       // output file format 0:YUV
		int            anDim[2];             // Portion of the image coded
		int            nWidth;               // YUV Frame width
		int            nHeight;              // YUV Frame height
		int            nCbCr_SubSampling_dx; // Sample rate of YUV 4:4:4, 4:2:2 or 4:2:0	
		int            nCbCr_SubSampling_dy; // Sample rate of YUV 4:4:4, 4:2:2 or 4:2:0
		int            nFrameRate;           //   Image Frame Rate
		int            nNumComps;            // In YUV files, numcomps always considered as 3
		int            nPrec;                // In YUV files, precision always considered as 8
	} Mj2_EncoderParameters;


	// Bit input-output stream (BitIO)
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef struct TBitIO
	{
		unsigned char *pStart;          // ��������� �� ������ ������
		unsigned char *pEnd;            // ��������� �� ����� ������
		unsigned char *pCurPos;         // ��������� �� ������� ������� � ������
		unsigned int   nBuffer;         // �����
		int            nValidBitsCount; // Coder   : ����� ��������� ��� ��� ������
		// Decoder : ����� ��������� ��� ��� ������
	} BitIO;
}