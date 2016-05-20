#include "stdafx.h"

#include <string.h>
#include "FontFile.h"
#include "Font.h"

//-------------------------------------------------------------------------------------------------------------------------------

struct TFontCacheTag 
{
	int   nCode;
	short nFracX;    // ��� ������ � ������������� ������������. ���������
	short nFracY;    // ������������� ������ �� x � y ������������ ���������� � int
	int   nMRU;      // Valid bit (0x80000000) � ����� MRU

	int   nX;        // ����� �� X ��������� ����� ��� ��������� �������
	int   nY;        // ����� �� Y ��������� ����� ��� ��������� �������
	int   nWidth;    // ������ �������
	int   nHeight;   // ������ �������
};

//-------------------------------------------------------------------------------------------------------------------------------
// CFont
//-------------------------------------------------------------------------------------------------------------------------------

CFont::CFont(CFontFile *pFontFile, BOOL bAntiAliasing, BOOL bUseKerning) 
{
	m_pFontFile = pFontFile;
	m_pFontFile->AddRef();

	m_arrdFontMatrix[0] = 1;
	m_arrdFontMatrix[1] = 0;
	m_arrdFontMatrix[2] = 0;
	m_arrdFontMatrix[3] = 1;

	m_arrdTextMatrix[0] = 1;
	m_arrdTextMatrix[1] = 0;
	m_arrdTextMatrix[2] = 0;
	m_arrdTextMatrix[3] = 1;

	m_bAntiAliasing = bAntiAliasing;
	m_bUseKerning   = bUseKerning;

	m_pCache     = NULL;
	m_pCacheTags = NULL;

	m_nMinX = m_nMinY = m_nMaxX = m_nMaxY = 0;

	m_fSize = 1.0f;

	m_fCharSpacing = 0.0;

	m_unHorDpi = 72;
	m_unVerDpi = 72;

	m_bNeedDoItalic = FALSE;
	m_bNeedDoBold   = FALSE;

	m_pDefaultFont    = NULL;
	m_bUseDefaultFont = FALSE;
}

void CFont::InitCache() 
{
	// �� ����� ���� ������ ���� (max - min + 1), �� �� �������� 2 �������, ����� �������� ������ ��������� � �����������.
	m_nGlyphWidth  = m_nMaxX - m_nMinX + 3;
	m_nGlyphHeight = m_nMaxY - m_nMinY + 3;

	if ( m_nGlyphHeight > 1000 || m_nGlyphWidth > 1000 )
	{
		m_nGlyphSize  = 0;
		m_nCacheSets  = 0;
		m_nCacheAssoc = 0;
		m_pCache      = NULL;
		m_pCacheTags  = NULL;

		return;
	}

	if ( m_bAntiAliasing ) 
	{
		m_nGlyphSize = m_nGlyphWidth * m_nGlyphHeight; // 24 ������ �����
	} 
	else 
	{
		m_nGlyphSize = ((m_nGlyphWidth + 7) >> 3) * m_nGlyphHeight; // 1 ������ ����� 
	}

	m_nCacheAssoc = 8;
	if ( m_nGlyphSize <= 256 ) 
	{
		m_nCacheSets = 8;
	} 
	else if ( m_nGlyphSize <= 512 ) 
	{
		m_nCacheSets = 4;
	} 
	else if ( m_nGlyphSize <= 1024 ) 
	{
		m_nCacheSets = 2;
	} 
	else 
	{
		m_nCacheSets = 1;
	}
	m_pCache     = (unsigned char *)malloc( m_nCacheSets * m_nCacheAssoc * m_nGlyphSize );
	m_pCacheTags = (TFontCacheTag *)malloc( m_nCacheSets * m_nCacheAssoc * sizeof(TFontCacheTag) );

	for ( int nIndex = 0; nIndex < m_nCacheSets * m_nCacheAssoc; ++nIndex ) 
	{
		m_pCacheTags[nIndex].nMRU = nIndex & (m_nCacheAssoc - 1);
	}
}
void CFont::ClearCache()
{
	if ( m_pCache ) 
	{
		free( m_pCache );
	}
	if ( m_pCacheTags ) 
	{
		free( m_pCacheTags );
	}

	InitCache();
}

CFont::~CFont() 
{
	m_pFontFile->Release();

	if ( m_pCache ) 
	{
		free( m_pCache );
	}
	if ( m_pCacheTags ) 
	{
		free( m_pCacheTags );
	}
}

BOOL CFont::GetGlyph(int nCode, int nFracX, int nFracY, TGlyphBitmap *pBitmap) 
{
	// ��� ������� ��� non-anti-aliased �������� �� ���� �������� � ������������� ������������(���������� �����)
	if ( !m_bAntiAliasing || m_nGlyphHeight > 50 ) 
	{
		nFracX = nFracY = 0;
	}

	// ���� � ����
	int nI = ( nCode & (m_nCacheSets - 1) ) * m_nCacheAssoc;
	for ( int nJ = 0; nJ < m_nCacheAssoc; ++nJ ) 
	{
		if ( ( m_pCacheTags[nI + nJ].nMRU & 0x80000000 ) && m_pCacheTags[nI + nJ].nCode == nCode && (int)m_pCacheTags[nI + nJ].nFracX == nFracX && (int)m_pCacheTags[nI + nJ].nFracY == nFracY ) 
		{
			pBitmap->nX      = m_pCacheTags[nI + nJ].nX;
			pBitmap->nY      = m_pCacheTags[nI + nJ].nY;
			pBitmap->nWidth  = m_pCacheTags[nI + nJ].nWidth;
			pBitmap->nHeight = m_pCacheTags[nI + nJ].nHeight;

			for ( int nK = 0; nK < m_nCacheAssoc; ++nK ) 
			{
				if ( nK != nJ && ( m_pCacheTags[nI + nK].nMRU & 0x7fffffff ) < ( m_pCacheTags[nI + nJ].nMRU & 0x7fffffff ) ) 
				{
					++m_pCacheTags[nI + nK].nMRU;
				}
			}

			m_pCacheTags[nI + nJ].nMRU = 0x80000000;

			pBitmap->bAA       = m_bAntiAliasing;
			pBitmap->pData     = m_pCache + (nI + nJ) * m_nGlyphSize;
			pBitmap->bFreeData = FALSE;
			return TRUE;
		}
	}

	// ������� ����� GlyphBitmap
	TGlyphBitmap oNewBitmap;
	if ( !MakeGlyph( nCode, nFracX, nFracY, &oNewBitmap ) ) 
	{
		return FALSE;
	}

	// ���� Glyph �� ���������� � BBox, ���������� ��������� �������������� Bitmap
	if ( oNewBitmap.nWidth > m_nGlyphWidth || oNewBitmap.nHeight > m_nGlyphHeight ) 
	{
		*pBitmap = oNewBitmap;
		return TRUE;
	}

	// ��������� GlyphBitmap � ���
	int nSize = 0;
	if ( m_bAntiAliasing ) 
	{
		nSize = oNewBitmap.nWidth * oNewBitmap.nHeight;
	} 
	else 
	{
		nSize = ((oNewBitmap.nWidth + 7) >> 3) * oNewBitmap.nHeight;
	}

	unsigned char *pBuffer = NULL;
	for ( int nJ = 0; nJ < m_nCacheAssoc; ++nJ ) 
	{
		if ( ( m_pCacheTags[nI + nJ].nMRU & 0x7fffffff ) == m_nCacheAssoc - 1 ) 
		{
			m_pCacheTags[nI + nJ].nMRU    = 0x80000000;
			m_pCacheTags[nI + nJ].nCode   = nCode;
			m_pCacheTags[nI + nJ].nFracX  = (short)nFracX;
			m_pCacheTags[nI + nJ].nFracY  = (short)nFracY;
			m_pCacheTags[nI + nJ].nX      = oNewBitmap.nX;
			m_pCacheTags[nI + nJ].nY      = oNewBitmap.nY;
			m_pCacheTags[nI + nJ].nWidth  = oNewBitmap.nWidth;
			m_pCacheTags[nI + nJ].nHeight = oNewBitmap.nHeight;

			pBuffer = m_pCache + (nI + nJ) * m_nGlyphSize;
			memcpy( pBuffer, oNewBitmap.pData, nSize );
		} 
		else 
		{
			++m_pCacheTags[nI + nJ].nMRU;
		}
	}

	*pBitmap = oNewBitmap;

	pBitmap->pData     = pBuffer;
	pBitmap->bFreeData = FALSE;

	if ( oNewBitmap.bFreeData ) 
	{
		free( oNewBitmap.pData );
	}
	return TRUE;
}

