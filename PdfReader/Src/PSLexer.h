#ifndef _PDF_READER_PS_LEXER_H
#define _PDF_READER_PS_LEXER_H

namespace PdfReader
{
	class PSLexer
	{
	public:

		PSLexer(int(*GetCharFunc)(void *), void *pData);
		~PSLexer();

		// ��������� ������ ������� Lexer::GetObject. �� ��� ������ PS ������� � PDF �����.
		// ����� �������� ������ ����������� ����� ������ "<>", "()" ��� �����-������ �������.
		// ������ � ���������� ������� "[]" �� ��������.
		bool GetToken(char *sBuffer, int nSize, int *pnLength);

	private:

		int LookChar();
		int GetChar();

	private:

		int(*GetCharFunction)(void *); // ��������� �� ������� ��� ���������� �������
		void *m_pData;
		int   m_nCharBuffer;
	};
}

#endif // _PDF_READER_PS_LEXER_H
