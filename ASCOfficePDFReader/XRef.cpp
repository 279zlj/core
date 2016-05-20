#include "stdafx.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "Stream.h"
#include "Lexer.h"
#include "Parser.h"
#include "Dict.h"
#include "ErrorConstants.h"
#include "XRef.h"
#include "../Common/TemporaryCS.h"

//---------------------------------------------------------------------------------------------------------------------------

#define XrefSearchSize 1024	// ������ ������� ����, ������� � ����� �����, ����� ����� 'startxref'

//---------------------------------------------------------------------------------------------------------------------------
// Permission bits
//---------------------------------------------------------------------------------------------------------------------------

#define PermissionPrint       ( 1 << 2 )
#define PermissionChange      ( 1 << 3 )
#define PermissionCopy        ( 1 << 4 )
#define PermissionNotes       ( 1 << 5 )
#define DefaultPermissionFlags 0xfffc

//---------------------------------------------------------------------------------------------------------------------------
// ObjectStream
//---------------------------------------------------------------------------------------------------------------------------

class ObjectStream 
{
public:

	ObjectStream(XRef *pXref, int nObjectStreamNum);

	~ObjectStream();

	int GetObjectStreamNum() 
	{ 
		return m_nObjectStreamNum; 
	}

	Object *GetObject(int nObjectIndex, int nObjectNum, Object *pObject);

private:

	int     m_nObjectStreamNum; // ���������� ����� �������(Stream Objects)
	int     m_nObjectsCount;    // ���������� �������� � ������
	Object *m_arrObjects;       // ������ �������� � ������
	int    *m_arrObjectsNums;   // ���������� ������ �������� � ������

};

ObjectStream::ObjectStream(XRef *pXref, int nObjectStreamNum) 
{
	Object oTempObject1, oTempObject2;

	m_nObjectStreamNum = nObjectStreamNum;
	m_nObjectsCount    = 0;
	m_arrObjects       = NULL;
	m_arrObjectsNums   = NULL;

	Object oObjectStream;
	if ( !pXref->Fetch( m_nObjectStreamNum, 0, &oObjectStream)->IsStream() ) 
	{
		oObjectStream.Free();
		return;
	}

	if ( !oObjectStream.StreamGetDict()->Search("N", &oTempObject1)->IsInt() ) 
	{
		oTempObject1.Free();
		oObjectStream.Free();
		return;
	}
	m_nObjectsCount = oTempObject1.GetInt();
	oTempObject1.Free();

	if ( m_nObjectsCount <= 0 ) 
	{
		oObjectStream.Free();
		return;
	}

	if ( !oObjectStream.StreamGetDict()->Search("First", &oTempObject1)->IsInt() ) 
	{
		oTempObject1.Free();
		oObjectStream.Free();
		return;
	}

	int nFirst = oTempObject1.GetInt();
	oTempObject1.Free();

	if ( nFirst < 0 ) 
	{
		oObjectStream.Free();
		return;
	}

	m_arrObjects     = new Object[ m_nObjectsCount ];
	m_arrObjectsNums = (int *)MemUtilsMallocArray( m_nObjectsCount, sizeof(int));
	int *arrnOffsets = (int *)MemUtilsMallocArray( m_nObjectsCount, sizeof(int));

	oObjectStream.StreamReset();
	oTempObject1.InitNull();

	Stream *pStream = new EmbedStream( oObjectStream.GetStream(), &oTempObject1, TRUE, nFirst);
	Parser *pParser = new Parser( pXref, new Lexer( pXref, pStream), FALSE);
	for (int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex ) 
	{
		pParser->GetObject( &oTempObject1 );
		pParser->GetObject( &oTempObject2 );
		
		if ( !oTempObject1.IsInt() || !oTempObject2.IsInt() ) 
		{
			oTempObject1.Free();
			oTempObject2.Free();
			delete pParser;
			MemUtilsFree( arrnOffsets );
			oObjectStream.Free();
			return;
		}
		m_arrObjectsNums[ nIndex ] = oTempObject1.GetInt();
		arrnOffsets[ nIndex ] = oTempObject2.GetInt();
		oTempObject1.Free();
		oTempObject2.Free();
		if ( m_arrObjectsNums[ nIndex ] < 0 || arrnOffsets[ nIndex ] < 0 || ( nIndex > 0 && arrnOffsets[ nIndex ] < arrnOffsets[ nIndex - 1 ] ) ) 
		{
			delete pParser;
			MemUtilsFree(arrnOffsets);
			oObjectStream.Free();
			return;
		}
	}
	while ( pStream->GetChar() != EOF) ;
	delete pParser;

	// ���������� � ������ ������� �������
	for ( int nIndex = nFirst; nIndex < arrnOffsets[0]; ++nIndex ) 
	{
		oObjectStream.GetStream()->GetChar();
	}

	for (int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex ) 
	{
		oTempObject1.InitNull();
		if ( nIndex == m_nObjectsCount - 1) 
		{
			pStream = new EmbedStream(oObjectStream.GetStream(), &oTempObject1, FALSE, 0);
		} 
		else 
		{
			pStream = new EmbedStream(oObjectStream.GetStream(), &oTempObject1, TRUE, arrnOffsets[ nIndex + 1 ] - arrnOffsets[ nIndex ]);
		}
		pParser = new Parser( pXref, new Lexer( pXref, pStream), FALSE);
		pParser->GetObject(&m_arrObjects[ nIndex ]);
		while ( pStream->GetChar() != EOF) ;
		delete pParser;
	}

	MemUtilsFree(arrnOffsets);

	oObjectStream.Free();
	return;
}

ObjectStream::~ObjectStream() 
{
	if ( m_arrObjects ) 
	{
		for ( int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex ) 
		{
			m_arrObjects[ nIndex ].Free();
		}
		delete[] m_arrObjects;
	}
	MemUtilsFree(m_arrObjectsNums);
}

Object *ObjectStream::GetObject(int nObjectIndex, int nObjectNum, Object *pObject ) 
{
	if ( nObjectIndex < 0 || nObjectIndex >= m_nObjectsCount || nObjectNum != m_arrObjectsNums[ nObjectIndex ] ) 
	{
		return pObject->InitNull();
	}
	return m_arrObjects[ nObjectIndex ].Copy( pObject );
}

//------------------------------------------------------------------------
// XRef
//------------------------------------------------------------------------

XRef::XRef(BaseStream *pStream) 
{
	InitializeCriticalSection(&m_oCS);

	m_bValidXref = TRUE;
	m_nErrorCode = ErrorNone;
	m_nEntrySize = 0;
	m_arrEntries = NULL;
	m_punStreamEnds = NULL;
	m_nStreamEndsCount = 0;
	m_pObjectStream = NULL;

	m_bEncrypted = FALSE;
	m_nPermissionFlags = DefaultPermissionFlags;
	m_bOwnerPassword = FALSE;

	// ������ Trailer
	m_pStream = pStream;
	m_nStart  = m_pStream->GetStartPos();
	unsigned int nPos = GetStartXref();

	// ���� �������� �������� ��� ������ 'startxref', �������� ������������ 
	// ������� xref

	if ( 0 == nPos ) 
	{
		if ( !( m_bValidXref = ConstructXRef() ) ) 
		{
			m_nErrorCode = ErrorDamaged;
			return;
		}
	} 
	else // ������ ������� Xref
	{
		while ( ReadXRef(&nPos) ) ;

		// ���� �������� �������� ��� ������ 'startxref', �������� ������������ 
		// ������� xref
		if ( !m_bValidXref ) 
		{
			if ( !( m_bValidXref = ConstructXRef() ) ) 
			{
				m_nErrorCode = ErrorDamaged;
				return;
			}
		}
	}

	Object oTempObject;

	m_oTrailerDict.DictLookupAndCopy( "Root", &oTempObject);
	if ( oTempObject.IsRef() ) 
	{
		m_nRootNum = oTempObject.GetRefNum();
		m_nRootGen = oTempObject.GetRefGen();
		oTempObject.Free();
	} 
	else 
	{
		oTempObject.Free();
		if ( !( m_bValidXref = ConstructXRef() ) ) 
		{
			m_nErrorCode = ErrorDamaged;
			return;
		}
	}

	// ������ ������������� � ������� Trailer ������ �� ������ xref, ����� �� �����
	// �� ������ ������� �������� ��������� �������
	m_oTrailerDict.GetDict()->SetXRef(this);
}

XRef::~XRef() 
{
	MemUtilsFree(m_arrEntries);
	m_oTrailerDict.Free();
	if (m_punStreamEnds) 
	{
		MemUtilsFree(m_punStreamEnds);
	}
	if (m_pObjectStream) 
	{
		delete m_pObjectStream;
	}

	DeleteCriticalSection(&m_oCS);
}

unsigned int XRef::GetStartXref() 
{
	char sBuffer[ XrefSearchSize + 1 ];
	char *pCurChar;
	int nChar = 0, nPos = 0, nCur = 0;

	// ��������� ��������� xrefSearchSize ����
	m_pStream->SetPos( XrefSearchSize, -1);
	for ( nPos = 0; nPos < XrefSearchSize; ++nPos ) 
	{
		if ( ( nChar = m_pStream->GetChar() ) == EOF) 
		{
			break;
		}
		sBuffer[ nPos ] = nChar;
	}
	sBuffer[ nPos ] = '\0';

	// ���� startxref
	for ( nCur = nPos - 9; nCur >= 0; --nCur ) 
	{
		if ( !strncmp( &sBuffer[ nCur ], "startxref", 9 ) ) 
		{
			break;
		}
	}
	if ( nCur < 0 ) 
		return 0;

	for ( pCurChar = &sBuffer[ nCur + 9 ]; isspace(*pCurChar); ++pCurChar ) ;

	m_unLastXRefOffset = StrintToUInt( pCurChar );

	return m_unLastXRefOffset;
}

// ��������� ���� ������ ������� Xref. ����� ��������� ��������������� ������� Trailer
// � ���������� ��������� �� ����������.
BOOL XRef::ReadXRef(unsigned int *punPos) 
{
	Object oObject;
	BOOL bResult = TRUE;

	oObject.InitNull();
	Parser *pParser = new Parser( NULL, new Lexer(NULL, m_pStream->MakeSubStream( m_nStart + *punPos, FALSE, 0, &oObject ) ), TRUE );
	pParser->GetObject( &oObject );

	// ������ ������� xref � ������ �����
	if ( oObject.IsCommand("xref") ) 
	{
		oObject.Free();
		bResult = ReadXRefTable( pParser, punPos);
	} 
	// ������ ������� xref, ����� ��� �������� � ���� ������
	else if ( oObject.IsInt() ) 
	{
		oObject.Free();
		if ( !pParser->GetObject( &oObject )->IsInt() ) 
		{
			oObject.Free();
			delete pParser;
			m_bValidXref = FALSE;
			return FALSE;
		}
		oObject.Free();
		if ( !pParser->GetObject( &oObject )->IsCommand("obj") ) 
		{
			oObject.Free();
			delete pParser;
			m_bValidXref = FALSE;
			return FALSE;
		}
		oObject.Free();
		if ( !pParser->GetObject( &oObject )->IsStream() ) 
		{
			oObject.Free();
			delete pParser;
			m_bValidXref = FALSE;
			return FALSE;
		}
		bResult = ReadXRefStream( oObject.GetStream(), punPos);
		oObject.Free();

	} 
	else 
	{
		oObject.Free();
		delete pParser;
		m_bValidXref = FALSE;
		return FALSE;
	}

	delete pParser;
	return bResult;
}

BOOL XRef::ReadXRefTable(Parser *pParser, unsigned int *punPos) 
{
	Object oTempObject;

	while (1) 
	{
		// ������� � ������ ���� ����� ������� ������� � ���������� ��������
		pParser->GetObject( &oTempObject );
		if ( oTempObject.IsCommand("trailer") ) 
		{
			oTempObject.Free();
			break;
		}
		if ( !oTempObject.IsInt() ) 
		{
			oTempObject.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
		int nFirst = oTempObject.GetInt();
		oTempObject.Free();
		if ( !pParser->GetObject( &oTempObject )->IsInt() ) 
		{
			oTempObject.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
		int nCount = oTempObject.GetInt();
		oTempObject.Free();

		if ( nFirst < 0 || nCount < 0 || nFirst + nCount < 0 ) 
		{
			oTempObject.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
		if ( nFirst + nCount > m_nEntrySize )
		{
			int nNewSize = 0;
			for ( nNewSize = m_nEntrySize ? 2 * m_nEntrySize : 1024; nFirst + nCount > nNewSize && nNewSize > 0; nNewSize <<= 1) ;
			if ( nNewSize < 0 ) 
			{
				oTempObject.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
			m_arrEntries = (XRefEntry *)MemUtilsReallocArray( m_arrEntries, nNewSize, sizeof(XRefEntry));
			for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex ) 
			{
				m_arrEntries[nIndex].unOffset = 0xffffffff;
				m_arrEntries[nIndex].eType    = xrefEntryFree;
			}
			m_nEntrySize = nNewSize;
		}
		for (int nIndex = nFirst; nIndex < nFirst + nCount; ++nIndex ) 
		{
			// ������: nnnnnnnnnn ggggg n eol
			// nnnnnnnnnn - �������� �����
            // ggggg      - Generation number(����� ������ �������)
            // n          - ����, ������������ ������������ �� ������
            // eol        - ����� ������(��� �������)
			if ( !pParser->GetObject( &oTempObject )->IsInt() ) 
			{
				oTempObject.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}

			XRefEntry oEntry;
			oEntry.unOffset = (unsigned int)oTempObject.GetInt();
			oTempObject.Free();
			if ( !pParser->GetObject(&oTempObject)->IsInt() ) 
			{
				oTempObject.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
			oEntry.nGen = oTempObject.GetInt();
			oTempObject.Free();
			pParser->GetObject( &oTempObject );
			if ( oTempObject.IsCommand("n") ) 
			{
				oEntry.eType = xrefEntryUncompressed;
			} 
			else if ( oTempObject.IsCommand("f") ) 
			{
				oEntry.eType = xrefEntryFree;
			} 
			else 
			{
				oTempObject.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
			oTempObject.Free();
			if ( m_arrEntries[ nIndex ].unOffset == 0xffffffff ) 
			{
				m_arrEntries[ nIndex ] = oEntry;
				// � PDF ������, ��������� � ������� Intellectual Property
				// Network, ������� ���: ������� Xref ���������� � ������� 
				// ��� ������� 1, ������ 0.
				if ( nIndex == 1 && nFirst == 1 && m_arrEntries[1].unOffset == 0 && m_arrEntries[1].nGen == 65535 && m_arrEntries[1].eType == xrefEntryFree ) 
				{
					nIndex = nFirst = 0;
					m_arrEntries[0] = m_arrEntries[1];
					m_arrEntries[1].unOffset = 0xffffffff;
				}
			}
		}
	}

	// ������ ������� Trailer
	if ( !pParser->GetObject( &oTempObject )->IsDict() ) 
	{
		oTempObject.Free();
		m_bValidXref = FALSE;
		return FALSE;
	}

	// ��������� ��������� 'Prev'
	Object oPrev;
	BOOL bPrev = FALSE;
	oTempObject.GetDict()->SearchAndCopy("Prev", &oPrev);
	if ( oPrev.IsInt() ) 
	{
		*punPos = (unsigned int)oPrev.GetInt();
		bPrev = TRUE;
	} 
	else if ( oPrev.IsRef() ) 
	{
		// ��������� ��������� ����� "/Prev NNN 0 R" ������ "/Prev NNN"
		*punPos = (unsigned int)oPrev.GetRefNum();
		bPrev = TRUE;
	} 
	else 
	{
		bPrev = FALSE;
	}
	oPrev.Free();

	// ��������� ������ ������� Trailer
	if ( m_oTrailerDict.IsNone() ) 
	{
		oTempObject.Copy( &m_oTrailerDict );
	}

	// ��������� ���� �� ������� 'XRefStm'
	if ( oTempObject.GetDict()->Search("XRefStm", &oPrev)->IsInt() ) 
	{
		unsigned int unPos2 = (unsigned int)oPrev.GetInt();
		ReadXRef( &unPos2 );
		if ( !m_bValidXref ) 
		{
			oPrev.Free();
			oTempObject.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
	}
	oPrev.Free();

	oTempObject.Free();
	return bPrev;
}

BOOL XRef::ReadXRefStream(Stream *pXrefStream, unsigned int *punPos)
{
	int arrW[3];

	Dict *pDict = pXrefStream->GetDict();
	Object oTemp;

	if ( !pDict->SearchAndCopy("Size", &oTemp)->IsInt() ) 
	{
		oTemp.Free();
		m_bValidXref = FALSE;
		return FALSE;
	}
	int nNewSize = oTemp.GetInt();
	oTemp.Free();

	if ( nNewSize < 0 ) 
	{
		oTemp.Free();
		m_bValidXref = FALSE;
		return FALSE;
	}
	if ( nNewSize > m_nEntrySize ) 
	{
		m_arrEntries = (XRefEntry *)MemUtilsReallocArray( m_arrEntries, nNewSize, sizeof(XRefEntry));
		for ( int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex ) 
		{
			m_arrEntries[nIndex].unOffset = 0xffffffff;
			m_arrEntries[nIndex].eType = xrefEntryFree;
		}
		m_nEntrySize = nNewSize;
	}

	if ( !pDict->SearchAndCopy("W", &oTemp)->IsArray() || oTemp.ArrayGetLength() < 3 ) 
	{
		oTemp.Free();
		m_bValidXref = FALSE;
		return FALSE;
	}
	for ( int nIndex = 0; nIndex < 3; ++nIndex ) 
	{
		Object oWItem;
		if ( !oTemp.ArrayGet( nIndex, &oWItem)->IsInt() ) 
		{
			oWItem.Free();
			oTemp.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
		arrW[nIndex] = oWItem.GetInt();
		oWItem.Free();
		if ( arrW[nIndex] < 0 || arrW[nIndex] > 4 ) 
		{
			oTemp.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
	}
	oTemp.Free();

	pXrefStream->Reset();
	Object oIndex;
	pDict->SearchAndCopy("Index", &oIndex);

	if ( oIndex.IsArray() ) 
	{
		for (int nIndex = 0; nIndex + 1 < oIndex.ArrayGetLength(); nIndex += 2) 
		{
			if ( !oIndex.ArrayGet( nIndex, &oTemp)->IsInt() ) 
			{
				oIndex.Free();
				oTemp.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
			int nFirst = oTemp.GetInt();
			oTemp.Free();
			if ( !oIndex.ArrayGet( nIndex + 1, &oTemp)->IsInt() ) 
			{
				oIndex.Free();
				oTemp.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
			int nCount = oTemp.GetInt();
			oTemp.Free();
			if ( nFirst < 0 || nCount < 0 || !ReadXRefStreamSection( pXrefStream, arrW, nFirst, nCount) ) 
			{
				oIndex.Free();
				m_bValidXref = FALSE;
				return FALSE;
			}
		}
	} 
	else 
	{
		if ( !ReadXRefStreamSection( pXrefStream, arrW, 0, nNewSize) ) 
		{
			oIndex.Free();
			m_bValidXref = FALSE;
			return FALSE;
		}
	}
	oIndex.Free();

	pDict->SearchAndCopy("Prev", &oTemp);
	BOOL bPrev = FALSE;
	if ( oTemp.IsInt() ) 
	{
		*punPos = (unsigned int)oTemp.GetInt();
		bPrev = TRUE;
	} 
	else 
	{
		bPrev = FALSE;
	}
	oTemp.Free();
	if ( m_oTrailerDict.IsNone() ) 
	{
		m_oTrailerDict.InitDict( pDict );
	}

	return bPrev;
}

BOOL XRef::ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount)
{
	if ( nFirst + nCount < 0 ) 
	{
		return FALSE;
	}
	if ( nFirst + nCount > m_nEntrySize ) 
	{
		int nNewSize = 0;
		for ( nNewSize = m_nEntrySize ? 2 * m_nEntrySize : 1024; nFirst + nCount > nNewSize && nNewSize > 0; nNewSize <<= 1) ;
		if ( nNewSize < 0 ) 
		{
			return FALSE;
		}
		m_arrEntries = (XRefEntry *)MemUtilsReallocArray( m_arrEntries, nNewSize, sizeof(XRefEntry));
		for ( int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex ) 
		{
			m_arrEntries[nIndex].unOffset = 0xffffffff;
			m_arrEntries[nIndex].eType = xrefEntryFree;
		}
		m_nEntrySize = nNewSize;
	}
	for ( int nIndex = nFirst; nIndex < nFirst + nCount; ++nIndex ) 
	{
		int nType = -1;
		int nChar = 0, nGen = 0, nJ = 0;
		unsigned int unOffset = 0;
		if ( arrW[0] == 0 ) 
		{
			nType = 1;
		} 
		else 
		{
			for ( nType = 0, nJ = 0; nJ < arrW[0]; ++nJ ) 
			{
				if ( ( nChar = pXrefStream->GetChar() ) == EOF ) 
				{
					return FALSE;
				}
				nType = (nType << 8) + nChar;
			}
		}
		for ( unOffset = 0, nJ = 0; nJ < arrW[1]; ++nJ ) 
		{
			if ( ( nChar = pXrefStream->GetChar() ) == EOF ) 
			{
				return FALSE;
			}
			unOffset = (unOffset << 8) + nChar;
		}
		for ( nGen = 0, nJ = 0; nJ < arrW[2]; ++nJ ) 
		{
			if ( ( nChar = pXrefStream->GetChar() ) == EOF ) 
			{
				return FALSE;
			}
			nGen = (nGen << 8) + nChar;
		}
		if ( m_arrEntries[nIndex].unOffset == 0xffffffff ) 
		{
			switch (nType) 
			{
			case 0:
				m_arrEntries[nIndex].unOffset = unOffset;
				m_arrEntries[nIndex].nGen = nGen;
				m_arrEntries[nIndex].eType = xrefEntryFree;
				break;
			case 1:
				m_arrEntries[nIndex].unOffset = unOffset;
				m_arrEntries[nIndex].nGen = nGen;
				m_arrEntries[nIndex].eType = xrefEntryUncompressed;
				break;
			case 2:
				m_arrEntries[nIndex].unOffset = unOffset;
				m_arrEntries[nIndex].nGen = nGen;
				m_arrEntries[nIndex].eType = xrefEntryCompressed;
				break;
			default:
				return FALSE;
			}
		}
	}

	return TRUE;
}

// ������� ��������� ������� Xref ��� ������������� �����.
BOOL XRef::ConstructXRef() 
{
	BOOL bGetRoot = FALSE;

	MemUtilsFree( m_arrEntries );
	m_nEntrySize = 0;
	m_arrEntries = NULL;

	// TO DO : Error "PDF file is damaged - attempting to reconstruct xref table..."
	m_nStreamEndsCount = 0;

	int nStreamEndsSize = 0;

	m_pStream->Reset();

	while (1) 
	{
		unsigned int nPos = m_pStream->GetPos();
		char sBuffer[256];
		if ( !m_pStream->GetLine( sBuffer, 256) ) 
		{
			break;
		}
		BYTE *pCur = (BYTE*)sBuffer;

		// ���������� �������
		while ( *pCur && Lexer::IsSpace(*pCur & 0xff) ) ++pCur;

		// C�������� ������� Trailer
		if ( !strncmp( (char*)pCur, "trailer", 7) ) 
		{
			Object oTemp;
			oTemp.InitNull();
			Parser *pParser = new Parser( NULL, new Lexer( NULL, m_pStream->MakeSubStream( nPos + 7, FALSE, 0, &oTemp ) ), FALSE);
			Object oNewTrailerDict;
			pParser->GetObject( &oNewTrailerDict );
			if ( oNewTrailerDict.IsDict() ) 
			{
				oNewTrailerDict.DictLookupAndCopy("Root", &oTemp);
				if ( oTemp.IsRef() ) 
				{
					m_nRootNum = oTemp.GetRefNum();
					m_nRootGen = oTemp.GetRefGen();
					if ( !m_oTrailerDict.IsNone() ) 
					{
						m_oTrailerDict.Free();
					}
					oNewTrailerDict.Copy( &m_oTrailerDict );
					bGetRoot = TRUE;
				}
				oTemp.Free();
			}
			oNewTrailerDict.Free();
			delete pParser;
		} 
		else if ( isdigit( *pCur ) ) // ���� ������
		{
			int nNum = atoi( (char*)pCur );
			if ( nNum > 0 ) 
			{
				do {
					++pCur;
				} while (*pCur && isdigit( *pCur ));
				if ( isspace(*pCur) ) 
				{
					do {
						++pCur;
					} while (*pCur && isspace( *pCur ));
					if (isdigit(*pCur)) 
					{
						int nGen = atoi( (char*)pCur );
						do {
							++pCur;
						} while (*pCur && isdigit( *pCur ));
						if (isspace(*pCur)) 
						{
							do {
								++pCur;
							} while (*pCur && isspace( *pCur ));
							if ( !strncmp( (char*)pCur, "obj", 3) ) 
							{
								if ( nNum >= m_nEntrySize ) 
								{
									int nNewSize = ( nNum + 1 + 255) & ~255;
									if ( nNewSize < 0 ) 
									{
										// TO DO: Error "Bad object number"
										return FALSE;
									}
									m_arrEntries = (XRefEntry *) MemUtilsReallocArray( m_arrEntries, nNewSize, sizeof(XRefEntry));
									for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex) 
									{
										m_arrEntries[ nIndex ].unOffset = 0xffffffff;
										m_arrEntries[ nIndex ].eType = xrefEntryFree;
									}
									m_nEntrySize = nNewSize;
								}
								if (m_arrEntries[ nNum ].eType == xrefEntryFree || nGen >= m_arrEntries[ nNum ].nGen ) 
								{
									m_arrEntries[ nNum ].unOffset = nPos - m_nStart;
									m_arrEntries[ nNum ].nGen = nGen;
									m_arrEntries[ nNum ].eType = xrefEntryUncompressed;
								}
							}
						}
					}
				}
			}

		}
		else if (!strncmp( (char*)pCur, "endstream", 9)) 
		{
			if ( m_nStreamEndsCount == nStreamEndsSize ) 
			{
				nStreamEndsSize += 64;
				m_punStreamEnds = (unsigned int *)MemUtilsReallocArray( m_punStreamEnds, nStreamEndsSize, sizeof(int) );
			}
			m_punStreamEnds[m_nStreamEndsCount++] = nPos;
		}
	}

	if ( bGetRoot )
		return TRUE;

	// TO DO: Error "Couldn't find trailer dictionary"
	return FALSE;
}

void XRef::SetEncryption(int nPermissionFlags, BOOL bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm) 
{
	m_bEncrypted = TRUE;
	m_nPermissionFlags = nPermissionFlags;
	m_bOwnerPassword = bOwnerPassword;
	if ( nKeyLength <= 16 ) 
	{
		m_nKeyLength = nKeyLength;
	} 
	else 
	{
		m_nKeyLength = 16;
	}
	for ( int nIndex = 0; nIndex < m_nKeyLength; ++nIndex ) 
	{
		m_arrDecryptKey[nIndex] = sDecryptKey[nIndex];
	}
	m_nEncryptVersion   = nEncryptVersion;
	m_eEncryptAlgorithm = eEncryptAlgorithm;
}

BOOL XRef::CheckPrint(BOOL bIgnoreOwnerPassword) 
{
	return ( !bIgnoreOwnerPassword && m_bOwnerPassword ) || ( m_nPermissionFlags & PermissionPrint );
}

BOOL XRef::CheckChange(BOOL bIgnoreOwnerPassword)
{
	return ( !bIgnoreOwnerPassword && m_bOwnerPassword ) || ( m_nPermissionFlags & PermissionChange );
}

BOOL XRef::CheckCopy(BOOL bIgnoreOwnerPassword) 
{
	return ( !bIgnoreOwnerPassword && m_bOwnerPassword ) || ( m_nPermissionFlags & PermissionCopy );
}

BOOL XRef::CheckAddNotes(BOOL bIgnoreOwnerPassword) 
{
	return ( !bIgnoreOwnerPassword && m_bOwnerPassword ) || ( m_nPermissionFlags & PermissionNotes );
}

Object *XRef::Fetch(int nNum, int nGen, Object *pObject) 
{
	CTemporaryCS oCS( &m_oCS );

	Object oObjNum, oObjGen, oObjContent;

	if ( nNum < 0 || nNum >= m_nEntrySize ) 
	{
		return pObject->InitNull();
	}
	Parser *pParser = NULL;
	XRefEntry *pEntry = &m_arrEntries[nNum];
	switch ( pEntry->eType ) 
	{
	case xrefEntryUncompressed:
		if ( pEntry->nGen != nGen ) 
		{
			return pObject->InitNull();
		}
		oObjNum.InitNull();
		pParser = new Parser( this, new Lexer(this, m_pStream->MakeSubStream( m_nStart + pEntry->unOffset, FALSE, 0, &oObjNum ) ), TRUE);
		pParser->GetObject( &oObjNum );
		pParser->GetObject( &oObjGen );
		pParser->GetObject( &oObjContent );
		if ( !oObjNum.IsInt() || oObjNum.GetInt() != nNum || !oObjGen.IsInt() || oObjGen.GetInt() != nGen || !oObjContent.IsCommand("obj") ) 
		{
			oObjNum.Free();
			oObjGen.Free();
			oObjContent.Free();
			delete pParser;
			return pObject->InitNull();
		}
		pParser->GetObject( pObject, m_bEncrypted ? m_arrDecryptKey : (unsigned char *)NULL, m_eEncryptAlgorithm, m_nKeyLength, nNum, nGen );
		oObjNum.Free();
		oObjGen.Free();
		oObjContent.Free();
		delete pParser;
		break;

	case xrefEntryCompressed:
		if ( nGen != 0 ) 
		{
			return pObject->InitNull();
		}
		if ( !m_pObjectStream || m_pObjectStream->GetObjectStreamNum() != (int)pEntry->unOffset ) 
		{
			if (m_pObjectStream) 
			{
				delete m_pObjectStream;
			}
			m_pObjectStream = new ObjectStream( this, pEntry->unOffset );
		}
		m_pObjectStream->GetObject( pEntry->nGen, nNum, pObject);
		break;

	default:
		return pObject->InitNull();
	}

	return pObject;
}

Object *XRef::GetDocInfo(Object *pObject) 
{
	CTemporaryCS oCS( &m_oCS );

	return m_oTrailerDict.DictLookup( "Info", pObject);
}
Object *XRef::GetDocInfoCopy(Object *pObject) 
{
	CTemporaryCS oCS( &m_oCS );

	return m_oTrailerDict.DictLookupAndCopy( "Info", pObject);
}

BOOL XRef::GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd) 
{
	if ( m_nStreamEndsCount == 0 || nStreamStart > m_punStreamEnds[m_nStreamEndsCount - 1] )
		return FALSE;

	int nStart = -1;
	int nEnd = m_nStreamEndsCount - 1;
	// m_punStreamEnds[ nStart ] < streamStart <= m_punStreamEnds[ nEnd ]
	while ( nEnd - nStart > 1 ) 
	{
		int nMid = ( nStart + nEnd) / 2;
		if ( nStreamStart <= m_punStreamEnds[ nMid ] ) 
		{
			nEnd = nMid;
		} 
		else 
		{
			nStart = nMid;
		}
	}
	*pnStreamEnd = m_punStreamEnds[ nEnd ];
	return TRUE;
}

unsigned int XRef::StrintToUInt(char *sString) 
{
	char *pCur;
	int nIndex = 0;

	unsigned int unRes = 0;
	for ( pCur = sString, nIndex = 0; *pCur && isdigit(*pCur) && nIndex < 10; ++pCur, ++nIndex ) 
	{
		unRes = 10 * unRes + (*pCur - '0');
	}
	return unRes;
}
