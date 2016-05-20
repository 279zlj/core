// JBig2File.cpp : Implementation of CJBig2File


#include "JBig2File.h"

#include "Encoder/jbig2encoder.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <tchar.h>
#endif

CJBig2File::CJBig2File()
{
	m_bDuplicateLineRemoval = false;
	m_bPDFMode				= true;
	m_bSymbolMode			= false;
	m_bRefine				= false;
	m_bUpscale2x			= false;
	m_bUpscale4x			= false;
	m_bSegment				= false;
	
	m_dTreshold				= 0.85;	
	m_nBwTreshold			= 188;
}

bool CJBig2File::MemoryToJBig2(unsigned char* pBufferBGRA ,int BufferSize, int nWidth, int nHeight, std::wstring sDstFileName)
{
	// check for valid input parameters

///////////////////////////////////////////////////////////
	if ( NULL == pBufferBGRA )	return false;

	int lBufferSize   = BufferSize;
	unsigned char *pSourceBuffer = pBufferBGRA;

	PIX  *pSource = pixCreate( nWidth, nHeight, 32 );
	if ( !pSource )	return false;

	for ( int nY = 0; nY < nHeight; nY++ )
	{
		for ( int nX = 0; nX < nWidth; nX++, pSourceBuffer += 3 )//todooo ������� 3 ? 4
		{
			pixSetRGBPixel( pSource, nX, nY, pSourceBuffer[ 2 ], pSourceBuffer[ 1 ], pSourceBuffer[ 0 ] );
		}
	}


	jbig2ctx *pContext = jbig2_init( m_dTreshold, 0.5, 0, 0, ! m_bPDFMode,  m_bRefine ? 10 : -1 );

	// ���� ������� ������ ����� �������� � JBig2
	// TO DO: ���� ����� ������� ������ ���������� �������� � 1 JBig2 ����

	// ������� ColorMap
	PIX *pPixL = NULL;
	if ( NULL == ( pPixL = pixRemoveColormap( pSource, REMOVE_CMAP_BASED_ON_SRC ) ) ) 
	{
		pixDestroy( &pSource );
		jbig2_destroy( pContext );
		return false;
	}
	pixDestroy( &pSource );

	PIX *pPixT = NULL;
	if ( pPixL->d > 1 ) 
	{
		PIX *pGray = NULL;

		if ( pPixL->d > 8 ) 
		{
			pGray = pixConvertRGBToGrayFast( pPixL );
			if ( !pGray )
			{
				pixDestroy( &pSource );
				jbig2_destroy( pContext );
				return false;
			}
		} 
		else 
		{
			pGray = pixClone( pPixL );
		}

		if (  m_bUpscale2x ) 
		{
			pPixT = pixScaleGray2xLIThresh( pGray,  m_nBwTreshold );
		} 
		else if (  m_bUpscale4x ) 
		{
			pPixT = pixScaleGray4xLIThresh( pGray,  m_nBwTreshold );
		} 
		else 
		{
			pPixT = pixThresholdToBinary( pGray,  m_nBwTreshold );
		}

		pixDestroy( &pGray );
	} 
	else 
	{
		pPixT = pixClone( pPixL );
	}

	if ( m_sOutputTreshold.length() > 0 ) 
	{
		pixWrite( m_sOutputTreshold.c_str(), pPixT, IFF_BMP );
	}

	if (  m_bSegment && pPixL->d > 1 ) 
	{
		PIX *pGraphics = segment_image( pPixT, pPixL );
		if ( pGraphics ) 
		{
			char *sFilename;
			asprintf( &sFilename, "%s.%04d.%s", m_sBaseName.c_str(), 0, ".bmp" );
			pixWrite( sFilename, pGraphics, IFF_BMP );
			free( sFilename );
		} 
		if ( !pPixT ) 
		{
			// ������ �� ������
			return true;
		}
	}

	pixDestroy( &pPixL );

	if ( !m_bSymbolMode ) 
	{
		int nLength = 0;
		uint8_t *pBuffer = jbig2_encode_generic( pPixT, !m_bPDFMode, 0, 0, m_bDuplicateLineRemoval, &nLength );

		bool bRes = true;
		FILE *pFile = _wfopen( sDstFileName.c_str(), _T("wb") );
		if ( pFile && pBuffer )
		{
			::fwrite( pBuffer, nLength, 1, pFile );
			::fclose( pFile );
			bRes = true;
		}
		else
			bRes = false;

		pixDestroy( &pPixT );
		if ( pBuffer ) free( pBuffer );
		jbig2_destroy( pContext );

		return bRes;
	}

	int nNumPages = 1;
	jbig2_add_page( pContext, pPixT );
	pixDestroy( &pPixT );

	int nLength = 0;
	uint8_t *pBuffer = jbig2_pages_complete( pContext, &nLength );
	if ( !pBuffer )
	{
		jbig2_destroy( pContext );
		return false;
	}

	if ( m_bPDFMode ) 
	{
		std::wstring sFileName = sDstFileName;//m_sBaseName + _T(".sym");
		const int nFileD = _wopen( sFileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT /*| WINBINARY*/, 0600 );
		
		if ( nFileD < 0 )
		{
			free( pBuffer );
			jbig2_destroy( pContext );
			return false;
		}
		write( nFileD, pBuffer, nLength );
		close( nFileD );
	}
	free( pBuffer );

	for ( int nIndex = 0; nIndex < nNumPages; ++nIndex ) 
	{
		pBuffer = jbig2_produce_page( pContext, nIndex, -1, -1, &nLength );
		if ( m_bPDFMode ) 
		{
			std::wstring sFileName = m_sBaseName + _T(".0000");
			const int nFileD = _wopen( sFileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT /*| WINBINARY*/, 0600 );

			if ( nFileD < 0 )
			{
				free( pBuffer );
				jbig2_destroy( pContext );
				return false;
			}
			write( nFileD, pBuffer, nLength );
			close( nFileD );
		} 
		free( pBuffer );
	}

	jbig2_destroy( pContext );

	return true;
}


// CJBig2File

