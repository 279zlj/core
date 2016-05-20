#pragma once

#include "Types.h"
#include "Utils.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Image
//-------------------------------------------------------------------------------------------------------------------------------

Image* Image_CreateEmpty() 
{
	Image *pImage = (Image*)Malloc(HEAP_ZERO_MEMORY,  sizeof(Image) );
	return pImage;
}

void   Image_Destroy(Image *pImage) 
{
	if( pImage ) 
	{
		if( pImage->pComponents ) 
		{
			for( int nIndex = 0; nIndex < pImage->nCsiz; nIndex++ ) 
			{
				ImageComponent *pImageComp = &pImage->pComponents[nIndex];
				RELEASEHEAP( pImageComp->pData );
			}
			RELEASEHEAP( pImage->pComponents );
		}
		RELEASEHEAP( pImage );
	}
}

Image* Image_Create(int nComponentsCount, ImageComponentParams *pCompParams, ColorSpace eColorSpace) 
{
	Image *pImage = NULL;
	pImage = (Image*)Malloc(HEAP_ZERO_MEMORY,  sizeof(Image) );

	if( pImage ) 
	{
		pImage->eColorSpace = eColorSpace;
		pImage->nCsiz       = nComponentsCount;

		// �������� ������ ��� ����������
		pImage->pComponents = (ImageComponent*)Malloc(HEAP_ZERO_MEMORY,  pImage->nCsiz * sizeof(ImageComponent) );

		if( !pImage->pComponents ) 
		{
			Image_Destroy( pImage );
			return NULL;
		}

		for( int nCurComponent = 0; nCurComponent < nComponentsCount; nCurComponent++ )
		{
			ImageComponent *pComponent = &pImage->pComponents[nCurComponent];

			pComponent->nXRsiz     = pCompParams[nCurComponent].nXRsiz;
			pComponent->nYRsiz     = pCompParams[nCurComponent].nYRsiz;
			pComponent->nWidth     = pCompParams[nCurComponent].nWidth;
			pComponent->nHeight    = pCompParams[nCurComponent].nHeight;
			pComponent->nXOsiz     = pCompParams[nCurComponent].nXoffset;
			pComponent->nYOsiz     = pCompParams[nCurComponent].nYoffset;
			pComponent->nPrecision = pCompParams[nCurComponent].nPrecision;
			pComponent->nBPP       = pCompParams[nCurComponent].nBPP;
			pComponent->nSigned    = pCompParams[nCurComponent].nSigned;

			pComponent->pData = (int*)Malloc(HEAP_ZERO_MEMORY,  pComponent->nWidth * pComponent->nHeight * sizeof(int) );
			if( !pComponent->pData ) 
			{
				Image_Destroy( pImage );
				return NULL;
			}
		}
	}
	else
	{
		// TO DO: ������ ��������� �� ������
	}

	return pImage;
}
