#pragma once

#include <atlstr.h>
#include <atlcoll.h>
#include <atlenc.h>
#include <math.h>

// �����������: ���������� � ���������� PDF

template <typename T>
inline double MMToPDFCoords( T tMM )
{
	return ( tMM / 25.4 ) * 72.0;
}
template <typename T>
inline double PDFCoordsToMM( T tX )
{
	return ( tX / 72.0 ) * 25.4;
}

template <typename T>
inline double PxToMM( T tX )
{
	return tX * 25.4 / 96;
}


// ��������� ������ ����� �� AVSOfficeSWFRenderer.
enum   ERendererState
{
	rendstateNone      = 0, // ��� ������� ������� ���������
	rendstateText      = 1, // ���� ������� �����
	rendstatePath      = 2, // ���� ������� ����
	rendstateClipPath  = 3, // ���� ������� ���� ��� �����
	rendstateClipText  = 4, // ���� ������� ������ ��� �����
	rendstateClipReset = 5, // ������� ����
	rendstateImage     = 6, // ��������� �����������
};

struct TMatrix
{
	float fA;
	float fB; 
	float fC;
	float fD;
	float fE;
	float fF;

	void Set(float fNewA, float fNewB, float fNewC, float fNewD, float fNewE, float fNewF)
	{
		fA = fNewA;
		fB = fNewB;
		fC = fNewC;
		fD = fNewD;
		fE = fNewE;
		fF = fNewF;
	}
};

struct TCommandParams
{
	double dAngle;
	double dLeft;
	double dTop;
	double dWidth;
	double dHeight;
	DWORD  nFlag;

	TMatrix oMatrix;
};

struct TCommandType
{
	int           nType; // ��� �������
	DWORD         nFlag; // �����, ��� ������ �������          
	TCommandType *pPrev; // ��������� �� ���������� ������� � �����
};

class CMatrix
{
public:

	CMatrix()
	{
		Reset();
	}

	CMatrix(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		Set( dA, dB, dC, dD, dE, dF );
	}
	void Reset()
	{
		m_dA = 1;
		m_dB = 0;
		m_dC = 0;
		m_dD = 1;
		m_dE = 0;
		m_dF = 0;

		m_dRotate = 0;
		m_dSkewX  = 0;
		m_dSkewY  = 0;
		m_dScaleX = 1;
		m_dScaleY = 1;
	}

	void Set(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		m_dA = dA;
		m_dB = dB;
		m_dC = dC;
		m_dD = dD;
		m_dE = dE;
		m_dF = dF;

		Split2();
	}


	template <typename T> void DoTransform(T &tX, T &tY)
	{
		T tOldX = tX, tOldY = tY;

		tX = tOldX * m_dA + tOldY * m_dC + m_dE;
		tY = tOldX * m_dB + tOldY * m_dD + m_dF;
	}

	// � ������ �������������� �� �������� �������
	template <typename T> void DoTransform2(T &tX, T &tY)
	{
		T tOldX = tX, tOldY = tY;

		tX = tOldX * m_dA + tOldY * m_dC;
		tY = tOldX * m_dB + tOldY * m_dD;
	}

	template <typename T> void DoITransform(T &tX, T &tY)
	{
		double dDet = m_dA * m_dD - m_dB * m_dC;
		if ( fabs( dDet ) < 0.001 )
			return;

		// �������� �������
		//             |     d         -b       0 |
		// det^(-1) *  |    -c          a       0 |
		//             | c*f - d*e  b*e - a*f   1 |

		double dIDet = 1 / dDet;
		double dIDet_2 = dIDet * dIDet;



		T tOldX = tX, tOldY = tY;

		tX = dIDet * ( tOldX * m_dD - tOldY * m_dC + m_dC * m_dF - m_dD * m_dE );
		tY = dIDet * (-tOldX * m_dB + tOldY * m_dA + m_dB * m_dE - m_dA * m_dF );
	}
	template <typename T> void DoITransform2(T &tX, T &tY)
	{
		double dDet = m_dA * m_dD - m_dB * m_dC;
		if ( fabs( dDet ) < 0.001 )
			return;

		// �������� �������
		//             | d  -b  0 |
		// det^(-1) *  |-c   a  0 |
		//             | 0   0  1 |

		double dIDet = 1 / dDet;

		T tOldX = tX, tOldY = tY;

		tX = dIDet * ( tOldX * m_dD - tOldY * m_dC );
		tY = dIDet * (-tOldX * m_dB + tOldY * m_dA );
	}
	double get_Rotate() const
	{
		return m_dRotate;
	}

	double get_SkewX() const
	{
		return m_dSkewX;
	}
	double get_SkewY() const
	{
		return m_dSkewY;
	}

	double get_ScaleX() const
	{
		return m_dScaleX;
	}

	double get_ScaleY() const
	{
		return m_dScaleY;
	}

	double get_A() const
	{
		return m_dA;
	}
	double get_B() const
	{
		return m_dB;
	}
	double get_C() const
	{
		return m_dC;
	}
	double get_D() const
	{
		return m_dD;
	}

	double get_E() const
	{
		return m_dE;
	}
	double get_F() const
	{
		return m_dF;
	}
private:

	void Split()
	{
		m_dScaleX = sqrt( m_dA * m_dA + m_dB * m_dB );
		m_dScaleY = sqrt( m_dC * m_dC + m_dD * m_dD );

		double dX_0 = 0; 
		double dY_0 = 1;

		DoTransform2( dX_0, dY_0 );

		double dX_1 = 1;
		double dY_1 = 0;

		DoTransform2( dX_1, dY_1 );

		m_dSkewX = atan2( dY_0, dX_0 ) - 1.57079632675;
		m_dSkewY = atan2( dY_1, dX_1 );

		m_dRotate = m_dSkewX;
	}

	void Split2()
	{
		// ������ ������� �������� ���������� ��� SWF::Matrix::ComputeMatrix
		m_dRotate = 0;
		m_dScaleX = m_dA;
		m_dSkewX  = m_dB;
		m_dSkewY  = m_dC;
		m_dScaleY = m_dD;
	}

private:

	double  m_dA;      //
	double  m_dB;      // ������� ��������������
	double  m_dC;      // | a b 0 |
	double  m_dD;      // | c d 0 |
	double  m_dE;      // | e f 1 |
	double  m_dF;      // 

	double  m_dRotate; // ���� ��������

	double  m_dSkewX;  // ������������ ���������
	double  m_dSkewY;  //

	double  m_dScaleX; // ������������ ������/����������
	double  m_dScaleY; // �� ���� oX,oY
};
class CRendererState
{
public:

	struct TMatrix
	{
		double dA;
		double dB;
		double dC;
		double dD;
		double dE;
		double dF;

		void Reset()
		{
			dA = 1; dB = 0;
			dC = 0; dD = 1;
			dE = 0; dF = 0;
		}

		void Set(double d_A, double d_B, double d_C, double d_D, double d_E, double d_F )
		{
			dA = d_A; dB = d_B;
			dC = d_C; dD = d_D;
			dE = d_E; dF = d_F;
		}
	};

public:

	CRendererState()
	{
		Reset();
	}

	~CRendererState()
	{
	}

	void Reset()
	{
		m_eState     = rendstateNone;
		m_ePrevState = rendstateNone;

		m_oMatrix.Reset();
		m_oPrevMatrix.Reset();
	}

	ERendererState get_State() const
	{
		return m_eState;
	}
	ERendererState get_PrevState() const
	{
		return m_ePrevState;
	}

	const TMatrix &get_PrevMatrix() const
	{
		return m_oPrevMatrix;
	}
	void set_NewState(ERendererState eState, CMatrix oMatrix)
	{
		m_ePrevState  = m_eState;
		m_oPrevMatrix = m_oMatrix;

		m_eState = eState;
		m_oMatrix.Set( oMatrix.get_A(), oMatrix.get_B(), oMatrix.get_C(), oMatrix.get_D(), oMatrix.get_E(), oMatrix.get_F() );
	}

	// ���������, ���������� �� ������� ��������������
	bool IsMatrixChanged()
	{
		if ( m_ePrevState == rendstateNone )
			return false;

		return ( fabs( m_oMatrix.dA - m_oPrevMatrix.dA  ) > 0.001 || fabs( m_oMatrix.dB - m_oPrevMatrix.dB  ) > 0.001 || fabs( m_oMatrix.dC - m_oPrevMatrix.dC  ) > 0.001 || fabs( m_oMatrix.dD - m_oPrevMatrix.dD  ) > 0.001 || fabs( m_oMatrix.dE - m_oPrevMatrix.dE  ) > 0.001 || fabs( m_oMatrix.dF - m_oPrevMatrix.dF  ) > 0.001 );
	}

	// ��������� ���������� �� ������� ����� ������� ��������������
	bool IsMatrixChanged2()
	{
		if ( m_ePrevState == rendstateNone )
			return false;

		return ( fabs( m_oMatrix.dA - m_oPrevMatrix.dA  ) > 0.001 || fabs( m_oMatrix.dB - m_oPrevMatrix.dB  ) > 0.001 || fabs( m_oMatrix.dC - m_oPrevMatrix.dC  ) > 0.001 || fabs( m_oMatrix.dD - m_oPrevMatrix.dD  ) > 0.001 );
	}

	void change_State(ERendererState eState)
	{
		m_eState = eState;
	}
private:

	ERendererState m_eState;      // ������� ���������
	ERendererState m_ePrevState;  // ���������� ���������

	TMatrix        m_oMatrix;     // ������� �������
	TMatrix        m_oPrevMatrix; // ������� ����������� ���������

	// TO DO: ��������, ���� ����, �������������� ���������

};
class CContiniousText
{
public:

	struct TColor
	{
		unsigned char unR;
		unsigned char unG;
		unsigned char unB;
		unsigned char unA;
	};
	struct TMatrix
	{
		double dA;
		double dB;
		double dC; 
		double dD;
		double dE;
		double dF;

		void Reset()
		{
			dA = 1; dB = 0;
			dC = 0; dD = 1;
			dE = 0; dF = 0;
		}
		void Set(double d_A, double d_B, double d_C, double d_D, double d_E, double d_F)
		{
			dA = d_A; dB = d_B;
			dC = d_C; dD = d_D;
			dE = d_E; dF = d_F;
		}
	};

	struct TText
	{
		BSTR  bsText;
		BSTR  bsUnicodeText;
		float fX;
		float fY;
		float fWidth;
		float fHeight;
		float fBaseLineOffset;
		bool  bStringGID;
	};


public:

	CContiniousText()
	{
		Reset();
	}

	~CContiniousText()
	{
		Reset();
	}

	void Reset()
	{
		m_bFirstEntry = TRUE;
		m_fX          = 0;
		m_fY          = 0;
		m_oMatrix.Reset();

		for ( int nIndex = 0; nIndex < m_arrTexts.GetSize(); nIndex++ )
		{
			::SysFreeString( m_arrTexts[nIndex].bsText        );
			::SysFreeString( m_arrTexts[nIndex].bsUnicodeText );
		}

		m_arrTexts   .RemoveAll();
		m_arrFonts   .RemoveAll();
		m_arrColors  .RemoveAll();
		m_arrMatrixes.RemoveAll();
		m_arrCharSpacing.RemoveAll();
	}
	void Add(CString sText, CString sUnicodeText, float fX, float fY, float fWidth, float fHeight, float fLineOffset, bool bStringGID, NSStructures::CFont oFont, NSStructures::CBrush oBrush, CMatrix oMatrix, double dCharSpacing)
	{
		TText oText;
		m_arrTexts.Add( oText );

		double dX          = MMToPDFCoords( fX          );
		double dY          = MMToPDFCoords( fY          );
		double dWidth	   = MMToPDFCoords( fWidth      );
		double dHeight     = MMToPDFCoords( fHeight     );
		double dLineOffset = MMToPDFCoords( fLineOffset );

		int nLast = m_arrTexts.GetSize() - 1;
		m_arrTexts[nLast].bsText          = sText.AllocSysString();
		m_arrTexts[nLast].bsUnicodeText   = sUnicodeText.AllocSysString();
		m_arrTexts[nLast].fX              = dX;
		m_arrTexts[nLast].fY              = dY;
		m_arrTexts[nLast].fWidth          = dWidth;
		m_arrTexts[nLast].fHeight         = dHeight;
		m_arrTexts[nLast].fBaseLineOffset = dLineOffset;
		m_arrTexts[nLast].bStringGID      = bStringGID;

		TMatrix oNewMatrix;
		oNewMatrix.Set( oMatrix.get_A(), oMatrix.get_B(), oMatrix.get_C(), oMatrix.get_D(), oMatrix.get_E(), oMatrix.get_F() );

		TColor oColor;
		oColor.unR = (unsigned char) ( ( oBrush.Color1 & 0x000000FF )       );
		oColor.unG = (unsigned char) ( ( oBrush.Color1 & 0x0000FF00 ) >> 8  );
		oColor.unB = (unsigned char) ( ( oBrush.Color1 & 0x00FF0000 ) >> 16 );
		oColor.unA = (unsigned char) ( oBrush.Alpha1 );

		m_arrColors     .Add( oColor       );
		m_arrFonts      .Add( oFont        );
		m_arrMatrixes   .Add( oNewMatrix   );
		m_arrCharSpacing.Add( dCharSpacing );

		if ( m_bFirstEntry )
		{
			m_bFirstEntry = FALSE;

			m_fX = dX;
			m_fY = dY;
			m_oMatrix = oMatrix;
		}
	}

	int  GetSize()
	{
		int nSize = m_arrTexts.GetSize();
		if ( nSize == m_arrMatrixes.GetSize() && nSize == m_arrFonts.GetSize() && nSize == m_arrCharSpacing.GetSize() && nSize == m_arrColors.GetSize() )
			return nSize;

		return -1;
	}
	void LeaveLast()
	{
		// ��������� ��������� ������. ��� �����������, ����� � ���������
		// ������ ����� ������� ��������������.

		int nSize = m_arrTexts.GetSize() - 1;
		for ( int nIndex = 0; nIndex < nSize; nIndex++ )
		{
			::SysFreeStringHelper( m_arrTexts[0].bsText );

			m_arrTexts   .RemoveAt( 0 );
			m_arrFonts   .RemoveAt( 0 );
			m_arrColors  .RemoveAt( 0 );
			m_arrMatrixes.RemoveAt( 0 );
			m_arrCharSpacing.RemoveAt( 0 );
		}

		if ( 1 == m_arrTexts.GetSize() )
		{
			m_bFirstEntry = FALSE;

			m_fX = m_arrTexts[0].fX;
			m_fY = m_arrTexts[0].fY;

			m_oMatrix.Set( m_arrMatrixes[0].dA, m_arrMatrixes[0].dB, m_arrMatrixes[0].dC, m_arrMatrixes[0].dD, m_arrMatrixes[0].dE, m_arrMatrixes[0].dF );
		}
		else
		{
			m_bFirstEntry = TRUE;

			m_fX = 0;
			m_fY = 0;

			m_oMatrix.Reset();
		}

	}

	const CMatrix             &get_Matrix () const
	{
		return m_oMatrix;
	}
	const float               &get_X() const
	{
		return m_fX;
	}

	const float               &get_Y() const
	{
		return m_fY;
	}

	const TText               *get_Text   (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.GetSize() )
			return (m_arrTexts.GetData() + nIndex);

		return NULL;
	}

	const NSStructures::CFont *get_Font   (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.GetSize() )
			return (m_arrFonts.GetData() + nIndex);

		return NULL;
	}

	const TColor              *get_Color  (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.GetSize() )
			return (m_arrColors.GetData() + nIndex);

		return NULL;
	}
	const double               get_CharSp (int nIndex) const
	{
		if ( nIndex >= 0 && nIndex < m_arrTexts.GetSize() )
			return m_arrCharSpacing[nIndex];

		return 0;
	}
private:

	BOOL                              m_bFirstEntry;

	float                             m_fX;             // ���������� ������� ������ 
	float                             m_fY;             //
	CMatrix                           m_oMatrix;        // ������� ������� ������

	CSimpleArray<TText              > m_arrTexts;       // ������ �������
	CSimpleArray<NSStructures::CFont> m_arrFonts;       // ������ ������� � �������
	CSimpleArray<TColor             > m_arrColors;      // ������ ��������� ������
	CSimpleArray<TMatrix            > m_arrMatrixes;    // ������ ������ � ������� ( � �������� ���������� ������ ������ �������� )
	CSimpleArray<double             > m_arrCharSpacing; // ������������� ��������
};
class CFontLoader
{
public:
	CFontLoader()
	{
	}
	~CFontLoader()
	{
	}
	void Add(CString sFilePath, CString sFontName, CString sEncodingName, CString sToUnicodeName )
	{
		m_arrFilePath.Add( sFilePath );
		m_arrFontName.Add( sFontName );
		m_arrEncodingName.Add( sEncodingName );
		m_arrToUnicodeName.Add( sToUnicodeName );

		m_mPathMap.SetAt( sFilePath, m_arrFilePath.GetCount() - 1 );
	}
	BOOL Find(CString sFilePath, CString *psFontName, CString *psEncodingName, CString *psToUnicodeName)
	{
		CAtlMap<CString, int>::CPair* pEl = m_mPathMap.Lookup( sFilePath );

		if ( NULL != pEl )
		{
			int nIndex = pEl->m_value;
			*psFontName = m_arrFontName.GetAt( nIndex );
			*psEncodingName = m_arrEncodingName.GetAt( nIndex );
			*psToUnicodeName = m_arrToUnicodeName.GetAt( nIndex );
			return TRUE;
		}

		return FALSE;
	}

private:

	CAtlMap<CString, int> m_mPathMap;
	CAtlArray<CString> m_arrFilePath;
	CAtlArray<CString> m_arrFontName;
	CAtlArray<CString> m_arrEncodingName;
	CAtlArray<CString> m_arrToUnicodeName;
};
class CFindFonts
{
public:
	CFindFonts()
	{
		m_pFontName  = NULL;
		m_pFontFile  = NULL;

		m_pBoldOut   = NULL;
		m_pItalicOut = NULL;

		m_pBoldIn    = NULL;
		m_pItalicIn  = NULL;

		m_nCount = 0;
		m_nSize  = 0;
	}
	~CFindFonts()
	{
		if ( m_pFontName )
		{
			for ( int nIndex = 0; nIndex < m_nCount; nIndex++ )
			{
				::SysFreeString( m_pFontName[nIndex] );
			}
			free( m_pFontName );
			m_pFontName = NULL;
		}

		if ( m_pFontFile )
		{
			for ( int nIndex = 0; nIndex < m_nCount; nIndex++ )
			{
				::SysFreeString( m_pFontFile[nIndex] );
			}
			free( m_pFontFile );
			m_pFontFile = NULL;
		}

		if ( m_pBoldOut )
		{
			free( m_pBoldOut );
			m_pBoldOut = NULL;
		}

		if ( m_pItalicOut )
		{
			free( m_pItalicOut );
			m_pItalicOut = NULL;
		}

		if ( m_pBoldIn )
		{
			free( m_pBoldIn );
			m_pBoldIn = NULL;
		}

		if ( m_pItalicIn )
		{
			free( m_pItalicIn );
			m_pItalicIn = NULL;
		}

		m_nCount = 0;
		m_nSize  = 0;
	}

	void Add(CString sFontName, CString sFontFile, BOOL bBoldIn, BOOL bItalicIn, BOOL bBoldOut, BOOL bItalicOut)
	{
		Resize(1);

		m_pFontName[m_nCount]  = sFontName.AllocSysString();
		m_pFontFile[m_nCount]  = sFontFile.AllocSysString();

		m_pBoldIn[m_nCount]    = bBoldIn;
		m_pItalicIn[m_nCount]  = bItalicIn;

		m_pBoldOut[m_nCount]   = bBoldOut;
		m_pItalicOut[m_nCount] = bItalicOut;

		++m_nCount;
		return;
	}

	BOOL Find(CString sFontName, BSTR *pbsFontFile, BOOL *pbBold, BOOL *pbItalic)
	{
		for ( int nIndex = 0; nIndex < m_nCount; ++nIndex )
		{
			if ( sFontName == CString(m_pFontName[nIndex]) && *pbBold == m_pBoldIn[nIndex] && *pbItalic == m_pItalicIn[nIndex] )
			{
				*pbsFontFile = m_pFontFile[nIndex];

				*pbBold   = m_pBoldOut[nIndex];
				*pbItalic = m_pItalicOut[nIndex];

				return TRUE;
			}
		}

		return FALSE;
	}

private:

	void Resize(int nCount)
	{
		if ( m_nCount + nCount > m_nSize ) 
		{
			if ( m_nSize == 0 ) 
			{
				m_nSize = 32;
			}
			while ( m_nSize < m_nCount + nCount ) 
			{
				m_nSize *= 2;
			}

			m_pFontName  = (BSTR *)realloc( m_pFontName,  m_nSize * sizeof(BSTR));
			m_pFontFile  = (BSTR *)realloc( m_pFontFile,  m_nSize * sizeof(BSTR));

			m_pBoldOut   = (BOOL *)realloc( m_pBoldOut,   m_nSize * sizeof(BOOL));
			m_pItalicOut = (BOOL *)realloc( m_pItalicOut, m_nSize * sizeof(BOOL)); 

			m_pBoldIn    = (BOOL *)realloc( m_pBoldIn,    m_nSize * sizeof(BOOL));
			m_pItalicIn  = (BOOL *)realloc( m_pItalicIn,  m_nSize * sizeof(BOOL)); 
		}

	}

private:

	BSTR *m_pFontName;
	BSTR *m_pFontFile;

	BOOL *m_pBoldIn;
	BOOL *m_pItalicIn;

	BOOL *m_pBoldOut;
	BOOL *m_pItalicOut;

	int   m_nCount;
	int   m_nSize;
};
class CGdiPath
{
public:

	static const int c_nTextureBrush = 3008;

public: 
	CGdiPath()
	{
		m_pPath = NULL;
	}
	~CGdiPath()
	{
		Delete();
	}

	void Create()
	{
		if ( m_pPath )
			Delete();
		m_pPath = new Gdiplus::GraphicsPath(Gdiplus::FillModeWinding);
	}
	void Delete()
	{
		RELEASEOBJECT( m_pPath );
	}

	void GetBounds(float *pfX, float *pfY, float *pfWidth, float *pfHeight)
	{
		if ( !CheckValidate() )
			return;

		Gdiplus::RectF oPathBounds;
		m_pPath->GetBounds( &oPathBounds );

		*pfX      = oPathBounds.GetLeft();
		*pfY      = oPathBounds.GetTop();
		*pfWidth  = oPathBounds.GetRight() - oPathBounds.GetLeft();
		*pfHeight = oPathBounds.GetBottom() - oPathBounds.GetTop();
	}
	// ������� ��� ����������� Path (��� �� ��� � � IAVSOfficeRendererTemplate)
	inline STDMETHOD(MoveTo)(float fX, float fY)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		m_pPath->StartFigure();
		//RELEASEOBJECT(m_pPath);
		//m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);

		m_pPath->AddLine( fX, fY, fX, fY );
		return S_OK;
	}
	inline STDMETHOD(LineTo)(float fX, float fY)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		Gdiplus::PointF pointCur( 0, 0 );
		CheckLastPoint( pointCur );
		m_pPath->AddLine( pointCur.X, pointCur.Y, fX, fY );

		return S_OK;
	}
	inline STDMETHOD(LinesTo)(SAFEARRAY** ppPoints)
	{
		if ( !CheckValidate() || ( NULL == ppPoints ) || ( NULL == *ppPoints ) )
			return S_FALSE;

		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		lCount /= 2;

		double* pPoints = (double*)((*ppPoints)->pvData); 

		CSimpleArray<Gdiplus::PointF> arPoints;

		for (int nIndex = 0; nIndex < lCount; ++nIndex)
		{
			Gdiplus::PointF oPoint(*pPoints, *(pPoints + 1));
			pPoints += 2;
			arPoints.Add(oPoint);
		}

		if ( 1 < lCount )
		{
			m_pPath->AddLines(arPoints.m_aT, lCount);
		}

		return S_OK;
	}

	inline STDMETHOD(CurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		Gdiplus::PointF pointCur(0, 0);

		CheckLastPoint(pointCur);

		CSimpleArray<Gdiplus::PointF> arPoints;

		arPoints.Add(pointCur);

		pointCur.X = fX1; pointCur.Y = fY1;
		arPoints.Add(pointCur);
		pointCur.X = fX2; pointCur.Y = fY2;
		arPoints.Add(pointCur);
		pointCur.X = fX3; pointCur.Y = fY3;
		arPoints.Add(pointCur);

		//m_pPath->AddCurve(arPoints.m_aT, 4);
		m_pPath->AddBeziers(arPoints.m_aT, 4);

		return S_OK;
	}
	inline STDMETHOD(CurvesTo)(SAFEARRAY** ppPoints)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		lCount /= 2;

		double* pPoints = (double*)((*ppPoints)->pvData); 

		CSimpleArray<Gdiplus::PointF> arPoints;

		for (int nIndex = 0; nIndex < lCount; ++nIndex)
		{
			Gdiplus::PointF oPoint(*pPoints, *(pPoints + 1));
			pPoints += 2;
			arPoints.Add(oPoint);
		}

		int nSize = arPoints.GetSize();
		if (1 < nSize)
		{
			//m_pPath->AddCurve(arPoints.m_aT, nSize);
			m_pPath->AddBeziers(arPoints.m_aT, nSize);
		}

		return S_OK;
	}
	inline STDMETHOD(ArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		m_pPath->AddArc(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return S_OK;
	}
	inline STDMETHOD(Close)()
	{
		if ( !CheckValidate() )
			return S_FALSE;

		m_pPath->CloseFigure();
		return S_OK;
	}
	inline STDMETHOD(End)()
	{
		if ( !CheckValidate() )
			return S_FALSE;

		m_pPath->Reset();
		return S_OK;
	}

	inline STDMETHOD(Start)()
	{
		if ( !CheckValidate() )
			return S_FALSE;

		m_pPath->StartFigure();
		return S_OK;
	}
	inline STDMETHOD(GetCurrentPoint)(float* fX, float* fY)
	{
		if ( !CheckValidate() )
			return S_FALSE;

		if ( ( NULL == fX ) || ( NULL == fY ) )
			return S_FALSE;

		Gdiplus::PointF oPoint(0, 0);
		CheckLastPoint(oPoint);

		*fX = oPoint.X;
		*fY = oPoint.Y;

		return S_OK;
	}

private:

	inline BOOL CheckValidate()
	{
		return ( NULL != m_pPath );
	}
	void CheckLastPoint(Gdiplus::PointF& pointCur)
	{
		if ( !CheckValidate() )
			return;

		int nCount = m_pPath->GetPointCount();

		if ( 0 < nCount )
		{
			Gdiplus::PointF* pPoints = new Gdiplus::PointF[nCount];
			m_pPath->GetPathPoints( pPoints, nCount );

			pointCur.X = pPoints[nCount - 1].X;
			pointCur.Y = pPoints[nCount - 1].Y;

			delete []pPoints;
		}
	}

private:

	Gdiplus::GraphicsPath *m_pPath;
};

static CString GetTempFileName()
{
	CString sResult;
	TCHAR ctTempPath [MAX_PATH];
	DWORD dwTempPathLen = ::GetTempPath (MAX_PATH, ctTempPath);
	if (0 != dwTempPathLen)
	{
		TCHAR ctTempFileName [MAX_PATH];
		UINT nTempFileLen = ::GetTempFileName(ctTempPath, _T("asc"), 0, ctTempFileName);
		if (0 != nTempFileLen)
		{
			sResult = ctTempFileName;
		}
	}
	else
	{
		ATLTRACE2 ("GetTempPath() failed! (0x%x)\n", GetLastError());
	}
	return sResult;
}
static bool WriteFile (const CString& sTempLogo, const BYTE *cpLogo1, const size_t nSize = 0)
{
	const size_t nCount = (0 == nSize) ? strlen ((const char*) cpLogo1) : nSize;
	FILE *pFile = _tfopen (sTempLogo.GetString(), _T("w"));
	const size_t nWritten  = fwrite (cpLogo1, 1, nCount, pFile);
	return (nWritten == nCount);
}
static bool DecodeBase64 (CStringA sBase64String, BYTE **aBuffer, DWORD *aBufferLen)
{
	if (NULL == aBuffer || NULL == aBufferLen)
		return false;
	const int nSrcLength = sBase64String.GetLength();
	const int nDecodedLength = Base64DecodeGetRequiredLength (nSrcLength);
	*aBuffer = new BYTE [nDecodedLength];

	if (NULL == *aBuffer)
		return false;

	int nTempLength = nDecodedLength;
	LPCSTR pBase64Buffer = sBase64String.GetBuffer();
	BOOL bDecoderResult = Base64Decode (pBase64Buffer, nSrcLength, *aBuffer, &nTempLength);
	sBase64String.ReleaseBuffer();
	*aBufferLen = nTempLength;

	return TRUE == bDecoderResult;
}

struct ImageUtils
{
	static inline IUnknown* LoadImage(CString sFilePath)
	{
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
		OfficeCore::IImageGdipFilePtr pImageFile;
		pImageFile.CreateInstance(OfficeCore::CLSID_CImageGdipFile);

		BSTR filename = sFilePath.AllocSysString();
		pImageFile->OpenFile(filename);
		SysFreeString(filename);

		IUnknown* punkFrame = NULL;
		pImageFile->get_Frame(&punkFrame);
		return punkFrame;
#else
		ImageStudio::IImageTransforms* pTransform = NULL;
		if (FAILED(CoCreateInstance(__uuidof(ImageStudio::ImageTransforms), NULL, CLSCTX_INPROC_SERVER, __uuidof(ImageStudio::IImageTransforms), (void**)(&pTransform))))
			return NULL;

		if (!pTransform)
			return NULL;

		sFilePath.Replace( _T("\\\\"), _T("\\") );

		CFileDownloader* loader = NULL;
		if (CFileDownloader::IsNeedDownload(sFilePath))
		{
			loader = new CFileDownloader(sFilePath, TRUE);
			if (loader)
			{
				loader->StartWork(1);

				while(loader->IsRunned())
				{
					Sleep(10);
				}

				if (loader->IsFileDownloaded())
				{
					sFilePath = loader->GetFilePath();
				}
			}
		}

		CString sXml(L"<transforms><ImageFile-LoadImage sourcepath=\"");
		sXml += sFilePath;
		sXml += L"\"></ImageFile-LoadImage></transforms>";

		BSTR bsXML = sXml.AllocSysString();
		if (!pTransform->SetXml(bsXML))
		{
			SysFreeString(bsXML);
			RELEASEINTERFACE(pTransform);
			RELEASEOBJECT(loader);
			return NULL;
		}

		SysFreeString(bsXML);

		if (!pTransform->Transform())
		{
			RELEASEINTERFACE(pTransform);
			RELEASEOBJECT(loader);
			return NULL;
		}

		VARIANT vImage;
		pTransform->GetResult(0, &vImage);

		RELEASEINTERFACE(pTransform);
		RELEASEOBJECT(loader);

		if (VT_UNKNOWN != vImage.vt)
			return NULL;

		return vImage.punkVal;
#endif
	}
};

//	text utils

struct ParseUtils
{
	static inline BOOL DoubleValues (const CString& src, CAtlArray<double>& numbers)
	{
		CString number	=	_T("");			
		int length		=	src.GetLength();

		for (int i = 0; i < length; ++i)
		{
			if (L'-' == src [i])
			{
				if (i > 1)
				{
					if (L'e' == src [i - 1])
					{
						number.AppendChar(src[i]);
						continue;
					}
				}

				if (number.GetLength())
					numbers.Add(_tstof(number));

				number	=	_T("");
			}

			if (isdigit(src[i]) || (src[i] == L'.') || (src[i] == L'-') || (src[i] == 'e'))
			{
				number.AppendChar(src[i]);
				continue;
			}	

			if (number.GetLength())
				numbers.Add(_tstof(number));

			number	=	_T("");
		}

		if (number.GetLength ())
			numbers.Add(_tstof(number));

		return (0 != numbers.GetCount());
	}
};
