#ifndef _WMF_TYPES_H_
#define _WMF_TYPES_H_


//---------------------------------------------------------------------------------------------------
// �������������� �����
//---------------------------------------------------------------------------------------------------

#ifdef UShort_2_Short
#undef UShort_2_Short
#endif /* UShort_2_Short */
#define UShort_2_Short(X) (((X) & 0x8000) ? ((short) ((int) (X) - (int) 0x10000)) : ((short) (X)))

#ifdef UShort_2_Long
#undef UShort_2_Long
#endif /* UShort_2_Long */
#define UShort_2_Long(X) (((X) & 0x8000) ? ((int) (X) - (int) 0x10000) : ((int) (X)))

#define POINT_TO_INCH(X) ((double) (X) / (double) 72  )
#define INCH_TO_POINT(X) ((double) (X) * (double) 72  )
#define MM_TO_INCH(X)    ((double) (X) / (double) 25.4)

//---------------------------------------------------------------------------------------------------
// Modes for CWmfFile.SetMapMode
//---------------------------------------------------------------------------------------------------
#define MM_TEXT        1
#define MM_LOMETRIC    2
#define MM_HIMETRIC    3
#define MM_LOENGLISH   4
#define MM_HIENGLISH   5
#define MM_TWIPS       6
#define MM_ISOTROPIC   7
#define MM_ANISOTROPIC 8
#define MM_DPI         9

//---------------------------------------------------------------------------------------------------
// Regions
//---------------------------------------------------------------------------------------------------

#define ERROR         0
#define NULLREGION    1
#define SIMPLEREGION  2
#define COMPLEXREGION 3

#define RGN_AND  1
#define RGN_OR   2
#define RGN_XOR  3
#define RGN_DIFF 4
#define RGN_COPY 5

#define CLIP_INTERSECT 0x0001
#define CLIP_EXCLUDE   0x0002
#define CLIP_KEEPRGN   0x0004

//---------------------------------------------------------------------------------------------------
// ��������� ��� ������ �����
//---------------------------------------------------------------------------------------------------

struct TFileInfo
{	
	FILE *pFile;
};

struct TMemoryInfo
{	
	unsigned char *pMemory;   // ��������� �� ������ ������
	unsigned char *pPointer;  // ��������� �� ������� �������

	long           lPos;      // ������� �������
	long           lLength;   // ������ ������
};

#define WMF_NOT_OPEN         -1
#define WMF_OPEN_FROM_FILE    0
#define WMF_OPEN_FROM_MEMORY  1

//---------------------------------------------------------------------------------------------------
// ��������� ��� ������
//---------------------------------------------------------------------------------------------------
enum EWmfWrite
{	
	wmf_write_WMF = 0,
	wmf_write_XML = 1
};

struct TWmfWriteFileInfo
{	
	FILE     *pFile;
	EWmfWrite eType;
};


//---------------------------------------------------------------------------------------------------
// ��������� Wmf-�����
//---------------------------------------------------------------------------------------------------

struct TWmfPlaceableMetaHeader
{	
	unsigned int   unKey;       // Magic number (������ 9AC6CDD7h)
	unsigned short ushHandle;   // Metafile HANDLE number (������ 0)
	short          shLeft;      // ����� ���������� � �������� ���������
	short          shTop;       // ������� ���������� � �������� ���������
	short          shRight;     // ������ ���������� � �������� ���������
	short          shBottom;    // ������� ���������� � �������� ���������
	unsigned short ushInch;     // ����� ������ � ����� ����� (dpi)
	unsigned int   unReserved;  // Reserved (������ 0)
	unsigned short ushChecksum; // �������� ��� ���������� 20 ����
};

struct TWmfHead
{
	unsigned short ushFileType;        // ��� ��������� (0 = memory, 1 = disk)
	unsigned short ushHeaderSize;      // ������ ������� ��������� (������ 9)
	unsigned short ushVersion;         // Version of Microsoft Windows
	unsigned int   unFileSize;         // ����� ������ ������� ��������� � Unsigned Short
	unsigned short ushNumberOfObjects; // ����� ���������� ����������� �������� � �����
	unsigned int   unMaxRecordSize;    // ������������ ������ ����� ���� �������(Record) � ������ �����
	unsigned short ushNumberOfMembers; // Not Used (������ 0)
};

typedef struct _TWmfMetaHeader
{	
	TWmfPlaceableMetaHeader *pPlaceableMetaHeader;
	TWmfHead                *pHeader;

	FILE                    *pFileIn;
	long                     lPos;
	int                      nPlaceable;
} TWmfMetaHeader, *TWmfFile;


//---------------------------------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------------------------------

enum EWmfError
{	
	wmf_error_None            = 0, // ��� ������
	wmf_error_NotEnoughMemory = 1, // ���������� �������� ������
	wmf_error_BadFile         = 2, // ������������� ��� ����������� ����
	wmf_error_BadFormat       = 3, // �������� ���������
	wmf_error_EOF             = 4, // ��������������� ����� �����
	wmf_error_DeviceError     = 5, // 
	wmf_error_Internal        = 6, // ��������� ������
	wmf_error_Assert          = 7, // ��������� ������
	wmf_error_UserExit        = 8, //
	wmf_error_UnSupported     = 9  // ���������������� ���
};

//---------------------------------------------------------------------------------------------------
// �������� ���������
//---------------------------------------------------------------------------------------------------

// ���������� �����
struct TWmfCoordF
{	
	float fX;
	float fY;
};

// �������������
struct TWmfRectF
{
	TWmfCoordF oTL; // ����� ������� ����
	TWmfCoordF oBR; // ������ ������ ����
};

struct TWmfMapping
{
	char       *sName;
	char       *sMapping;

	/* I had been hoping to keep FT out of this file, but
	 * it seems easier just to use the FT encoding defs
	 * rather than create some kind of wrapper...
	 */
	FT_Encoding eEncoding;
};

struct TWmfFontMap
{
	char *sName;   // Wmf font name

	char *sNamePS; // Postscript font names
	char *sItalic;
	char *sBold;
	char *sBoldItalic;
};


struct TWmfRGB
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct TWmfBMP
{
	unsigned short ushWidth;
	unsigned short ushHeight;

	BYTE          *pData;
};

struct TWmfPen
{	
	unsigned short ushStyle;

	double         dWidth;
	double         dHeight;  // ignored

	TWmfRGB        oColor;
};

struct TWmfBrush
{
	unsigned short ushStyle;
	unsigned short ushHatch;

	TWmfRGB        oColor;
	TWmfBMP        oBitmap;
};

struct TWmfFont
{
	unsigned short ushHeight;
	unsigned short ushWidth;

	short          shEscapement;
	short          shOrientation;

	unsigned short ushWeight;

	unsigned char  unItalic;
	unsigned char  unUnderline;
	unsigned char  unStrikeOut;
	unsigned char  unCharSet;
	unsigned char  unOutPrecision;
	unsigned char  unClipPrecision;
	unsigned char  unQuality;
	unsigned char  unPitchAndFamily;

	char          *sFaceName;

	void          *pUserData;
};

// Structure containing list of XML attributes
struct TWmfAttributes
{
	char          *sName;

	char         **psAttrs;

	unsigned long  ulCount;
	unsigned long  ulMax;

	unsigned char *sBuffer;

	unsigned long  ulLength;
	unsigned long  ulOffset;
};

// Structure containing list of lists of XML attributes
struct TWmfAttributeStore
{	
	TWmfAttributes *pAttrlist;

	unsigned long   ulCount;
	unsigned long   ulMax;
};

struct TWmfRegion;
struct TWmfDC
{	
	TWmfBrush      *pBrush;
	TWmfPen        *pPen;
	TWmfFont       *pFont;

	TWmfRGB         oTextColor;
	TWmfRGB         oBGColor;

	unsigned short  ushTextAlign;
	unsigned short  ushBGMode;
	unsigned short  ushPolyFillMode;
	unsigned short  ushCharExtra;
	unsigned short  ushBreakExtra;

	unsigned short  ushROPMode;

	struct
	{
		int nX;
		int nY;
		int nWidth;
		int nHeight;
	} oWindow;

	double          dPixelWidth; // dpi
	double          dPixelHeight;

	unsigned short  ushMapMode;

	TWmfRegion     *pClip;
};

//---------------------------------------------------------------------------------------------------
// ��������� ��� ���������
//---------------------------------------------------------------------------------------------------

struct TWmfFlood
{	
	TWmfDC        *pDC;

	TWmfCoordF     oPoint;
	TWmfRGB        oColor;

	unsigned short ushType;

	double         dPixelWidth;
	double         dPixelHeight;
};

struct TWmfDrawPixel
{
	TWmfDC    *pDC;

	TWmfCoordF oPoint;
	TWmfRGB    oColor;

	double     dPixelWidth;
	double     dPixelHeight;
};

struct TWmfDrawArc
{	
	TWmfDC    *pDC;

	TWmfCoordF oTL; // �������������, � ������� ������ ������ 
	TWmfCoordF oBR; // ������ ����

	TWmfCoordF oStart; // ������ ����(��� ��������� ������� �� ������������)
	TWmfCoordF oEnd;   // ����� ���� (���������� ������������ ������ �������)
};

struct TWmfDrawLine
{	
	TWmfDC    *pDC;

	TWmfCoordF oFrom;
	TWmfCoordF oTo;
};

struct TWmfPolyLine
{	
	TWmfDC        *pDC;

	TWmfCoordF    *pPoints;

	unsigned short ushCount;
};

struct TWmfPolyPoly
{	
	TWmfDC         *pDC;

	TWmfCoordF    **ppPoints;    // ppPoints[i][*] - ����� i-�� ��������

	unsigned short *pCount;      // ���������� ����� � i-�� ��������
	unsigned short ushPolyCount; // ���������� ���������
};

struct TWmfDrawRectangle
{	
	TWmfDC    *pDC;

	TWmfCoordF oTL;
	TWmfCoordF oBR;

	float      fWidth;  // ��� ������� ����������� (0, ���� ������������� ��� ������������ �����)
	float      fHeight; // 
};

struct TWmfPolyRectangle
{
	TWmfDC      *pDC;

	TWmfCoordF  *pTL;
	TWmfCoordF  *pBR;

	unsigned int unCount;

	float        fWidth;
	float        fHeight;
};

struct TWmfBMPRead
{
	long           lOffset;
	long           lLength;
	unsigned char *pBuffer;

	unsigned short ushWidth;
	unsigned short ushHeight;

	TWmfBMP        oBitmap;
};

struct TWmfBMPDraw
{
	TWmfDC         *pDC;

	TWmfCoordF      oPoint;
	TWmfBMP         oBitmap;

	unsigned int    unType;

	struct
	{	
		unsigned short ushX;
		unsigned short ushY;
		unsigned short ushW;
		unsigned short ushH;
	} oCrop;

	double          dPixelWidth;
	double          dPixelHeight;
};

struct TWmfROPDraw
{
	TWmfDC      *pDC;

	TWmfCoordF   oTL;
	TWmfCoordF   oBR;

	unsigned int unROP;

	double       dPixelWidth;
	double       dPixelHeight;
};

struct TWmfDrawText
{
	TWmfDC         *pDC;

	TWmfCoordF      oPoint;   // �������� ����� ��� ���������(� ������ TextAlign)
	TWmfCoordF      oOrigin;  // �������� ����� ��� ���������
	bool            bUseDx; 

	TWmfCoordF      oTL; // Clip
	TWmfCoordF      oBR; //

	struct
	{	
		TWmfCoordF oTL;
		TWmfCoordF oTR;
		TWmfCoordF oBL;
		TWmfCoordF oBR;
	} oBBox;

	char           *sText;

	unsigned short  ushFlags;

	double          dFontHeight;
	double          dFontRatio;  // ��������� ������ � ������
};

struct TWmfUserData	
{
	TWmfDC *pDC;
	void   *pData;
};

struct TWmfFunctionReference
{
	void (*Device_Open )();
	void (*Device_Close)();
	void (*Device_Begin)();
	void (*Device_End  )();

	void (*Flood_Interior) (TWmfFlood*);
	void (*Flood_Exterior) (TWmfFlood*);

	void (*Draw_Pixel      ) (TWmfDrawPixel*);
	void (*Draw_Pie        ) (TWmfDrawArc*);
	void (*Draw_Chord      ) (TWmfDrawArc*);
	void (*Draw_Arc        ) (TWmfDrawArc*);
	void (*Draw_Ellipse    ) (TWmfDrawArc*);
	void (*Draw_Line       ) (TWmfDrawLine*);
	void (*Poly_Line       ) (TWmfPolyLine*);
	void (*Draw_Polygon    ) (TWmfPolyLine*);
	void (*Draw_Polypolygon) (TWmfPolyPoly*);
	void (*Draw_Rectangle  ) (TWmfDrawRectangle*);

	void (*Rop_Draw) (TWmfROPDraw*);
	void (*Bmp_Draw) (TWmfBMPDraw*);
	void (*Bmp_Read) (TWmfBMPRead*);
	void (*Bmp_Free) (TWmfBMP*);

	void (*Draw_Text) (TWmfDrawText*);

	void (*UserData_Init) (TWmfUserData*);
	void (*UserData_Copy) (TWmfUserData*);
	void (*UserData_Set ) (TWmfUserData*);
	void (*UserData_Free) (TWmfUserData*);

	void (*Region_Frame) (TWmfPolyRectangle*);
	void (*Region_Paint) (TWmfPolyRectangle*);
	void (*Region_Clip ) (TWmfPolyRectangle*);
};

//---------------------------------------------------------------------------------------------------
// Player
//---------------------------------------------------------------------------------------------------

#define PLAYER_SCANNED          (1 << 0)
#define PLAYER_PLAY             (1 << 1)
#define PLAYER_TLBR_D_SET       (1 << 2)

struct TWmfCoord
{
	unsigned int unX;
	unsigned int unY;
};

struct TWmfRegion
{
	unsigned int   unSize;
	unsigned int   unNumRects;

	unsigned short ushType; // NULL, SIMPLE, COMPLEX

	TWmfRectF     *pRects;
	TWmfRectF      oExtents;
};

struct TWmfObject
{
	int            nType;
	union
	{	
		TWmfBrush  oBrush;
		TWmfPen	   oPen;
		TWmfFont   oFont;

		int        nPalette;

		TWmfRegion oRegion;
	} uObject;
};

struct TWmfPlayer
{
	TWmfPen        oDefaultPen;
	TWmfBrush      oDefaultBrush;
	TWmfFont       oDefaultFont;

	TWmfDC        *pDC; // ������� DC

	int            nDCStackMaxLen;
	int            nDCStackLength;
	TWmfDC       **ppDCStack;

	TWmfObject    *pObjects;

	TWmfRegion     oVisible; /* I don't understand this... what is the purpose of `visible'? */

	TWmfCoord      oCurrent; /* Current position */

	TWmfCoordF     oDisplayTL;    /* Bounding box from Display perspective */
	TWmfCoordF     oDisplayBR;

	TWmfCoordF     oBoundTL;
	TWmfCoordF     oBoundBR;

	TWmfCoordF     oViewportOrigin; /* Origin of Viewport */

	unsigned int   unViewportWidth; /* Display extents	 */
	unsigned int   unViewportHeight;

	unsigned char *pParameters; /* meta file parameter values */

	unsigned long  ulFlags;

	struct
	{
		int nX;
		int nY;
		int nWidth;
		int nHeight;
	} oWindow;
};

struct TWmfRecord
{	
	unsigned long  ulSize;
	unsigned int   unFunction;
	unsigned char *sParameter;
	long           lPosition;
};

//---------------------------------------------------------------------------------------------------
// Device Indepent Bitmap
//---------------------------------------------------------------------------------------------------

enum EWmfBitCount
{
	BI_BITCOUNT_0 = 0x0000,
	BI_BITCOUNT_1 = 0x0001,
	BI_BITCOUNT_2 = 0x0004,
	BI_BITCOUNT_3 = 0x0008,
	BI_BITCOUNT_4 = 0x0010,
	BI_BITCOUNT_5 = 0x0018,
	BI_BITCOUNT_6 = 0x0020
};

#endif /* _WMF_TYPES_H_ */