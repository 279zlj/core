#ifndef _PARSER_H
#define _PARSER_H

#include "Lexer.h"

//------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------

class Parser 
{
public:

	Parser(XRef *pXref, Lexer *pLexer, BOOL bAllowStreams);

	~Parser();

	// ��������� ��������� ������ �� ������.
	Object *GetObject(Object *pObject, unsigned char *sDecryptKey = NULL, CryptAlgorithm eEncryptAlgorithm = cryptRC4, int nKeyLength = 0, int nObjectNum = 0, int nObjectGen = 0);

	Stream *GetStream() 
	{ 
		return m_pLexer->GetStream(); 
	}

	int     GetPos() 
	{ 
		return m_pLexer->GetPos(); 
	}

private:

	Stream *CreateStream(Object *pDict, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen);
	void Shift();

private:

	XRef  *m_pXref;         // ������� Xref ��� ����the xref table for this PDF file
	Lexer *m_pLexer;        // �������� �����
	BOOL   m_bAllowStreams; // ����� �� ������� Stream Objects?
	Object m_oBuffer1;      // ��� ��������� �������
	Object m_oBuffer2;      // 
	int    m_nInlineImage;  // �������������, ����� ������������ � Inline Image
};

#endif /* _PARSER_H */

