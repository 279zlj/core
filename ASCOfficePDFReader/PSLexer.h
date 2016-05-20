#ifndef _PS_LEXER_H
#define _PS_LEXER_H

//------------------------------------------------------------------------

class PSLexer 
{
public:

	PSLexer(int (*GetCharFunc)(void *), void *pData);
	~PSLexer();

	// ��������� ������ ������� Lexer::GetObject. �� ��� ������ PS ������� � PDF �����.
	// ����� �������� ������ ����������� ����� ������ "<>", "()" ��� �����-������ �������.
	// ������ � ���������� ������� "[]" �� ��������.
	BOOL GetToken(char *sBuffer, int nSize, int *pnLength);

private:

	int LookChar();
	int GetChar();

private:

	int (*GetCharFunction)(void *); // ��������� �� ������� ��� ���������� �������
	void *m_pData;
	int   m_nCharBuffer;
};

#endif /* _PS_LEXER_H */
