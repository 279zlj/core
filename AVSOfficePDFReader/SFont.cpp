#include "stdafx.h"

#include <string.h>
#include "MemoryUtils.h"
#include "SMathExt.h"
#include "SGlyphBitmap.h"
#include "SFontFile.h"
#include "SFont.h"

//-------------------------------------------------------------------------------------------------------------------------------

struct SFontCacheTag 
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
// SFont
//-------------------------------------------------------------------------------------------------------------------------------

SFont::SFont(SFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix, BOOL bAntiAliasing) 
{
	m_pFontFile = pFontFile;
	m_pFontFile->AddRef();

	m_arrdFontMatrix[0] = pFontMatrix[0];
	m_arrdFontMatrix[1] = pFontMatrix[1];
	m_arrdFontMatrix[2] = pFontMatrix[2];
	m_arrdFontMatrix[3] = pFontMatrix[3];

	m_arrdTextMatrix[0] = pTextMatrix[0];
	m_arrdTextMatrix[1] = pTextMatrix[1];
	m_arrdTextMatrix[2] = pTextMatrix[2];
	m_arrdTextMatrix[3] = pTextMatrix[3];

	m_bAntiAliasing = bAntiAliasing;

	m_pCache     = NULL;
	m_pCacheTags = NULL;

	m_nMinX = m_nMinY = m_nMaxX = m_nMaxY = 0;
}

void SFont::InitCache() 
{
	// �� ����� ���� ������ ���� (max - min + 1), �� �� �������� 2 �������, ����� �������� ������ ��������� � �����������.
	m_nGlyphWidth  = m_nMaxX - m_nMinX + 3;
	m_nGlyphHeight = m_nMaxY - m_nMinY + 3;

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
	m_pCache     = (unsigned char *)MemUtilsMallocArray( m_nCacheSets * m_nCacheAssoc, m_nGlyphSize );
	m_pCacheTags = (SFontCacheTag *)MemUtilsMallocArray( m_nCacheSets * m_nCacheAssoc, sizeof(SFontCacheTag) );

	for ( int nIndex = 0; nIndex < m_nCacheSets * m_nCacheAssoc; ++nIndex ) 
	{
		m_pCacheTags[nIndex].nMRU = nIndex & (m_nCacheAssoc - 1);
	}
}

SFont::~SFont() 
{
	m_pFontFile->Release();

	if ( m_pCache ) 
	{
		MemUtilsFree( m_pCache );
	}
	if ( m_pCacheTags ) 
	{
		MemUtilsFree( m_pCacheTags );
	}
}

BOOL SFont::GetGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap) 
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
	SGlyphBitmap oNewBitmap;
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
		MemUtilsFree( oNewBitmap.pData );
	}
	return TRUE;
}
