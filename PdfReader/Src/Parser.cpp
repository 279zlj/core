#include <stddef.h>
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Decrypt.h"
#include "Parser.h"
#include "XRef.h"

namespace PdfReader
{
	Parser::Parser(XRef *pXref, Lexer *pLexer, bool bAllowStreams)
	{
		m_pXref  = pXref;
		m_pLexer = pLexer;
		m_nInlineImage = 0;
		m_bAllowStreams = bAllowStreams;
		m_pLexer->GetObject(&m_oBuffer1);
		m_pLexer->GetObject(&m_oBuffer2);
	}

	Parser::~Parser()
	{
		m_oBuffer1.Free();
		m_oBuffer2.Free();
		delete m_pLexer;
	}

	Object *Parser::GetObject(Object *pObject, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen)
	{
		Stream *pStream = NULL;
		Object oTemp;
		int nNum = 0;

		// ��������� ������ ����� Inline image data
		if (m_nInlineImage == 2)
		{
			m_oBuffer1.Free();
			m_oBuffer2.Free();
			m_pLexer->GetObject(&m_oBuffer1);
			m_pLexer->GetObject(&m_oBuffer2);
			m_nInlineImage = 0;
		}

		// ������
		if (m_oBuffer1.IsCommand("["))
		{
			Shift();
			pObject->InitArray(m_pXref);
			while (!m_oBuffer1.IsCommand("]") && !m_oBuffer1.IsEOF())
				pObject->ArrayAdd(GetObject(&oTemp, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen));
			if (m_oBuffer1.IsEOF())
			{
				// TO DO: Error "End of file inside array"
			}
			Shift();
		}
		else if (m_oBuffer1.IsCommand("<<")) 	// Dictionary ��� Stream
		{
			Shift();
			pObject->InitDict(m_pXref);
			while (!m_oBuffer1.IsCommand(">>") && !m_oBuffer1.IsEOF())
			{
				if (!m_oBuffer1.IsName())
				{
					// TO DO: Error "Dictionary key must be a name object"
					Shift();
				}
				else
				{
					char *sKey = CopyString(m_oBuffer1.GetName());
					Shift();
					if (m_oBuffer1.IsEOF() || m_oBuffer1.IsError())
					{
						MemUtilsFree(sKey);
						break;
					}
					pObject->DictAdd(sKey, GetObject(&oTemp, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen));
				}
			}
			if (m_oBuffer1.IsEOF())
			{
				// TO DO: Error "End of file inside dictionary"
			}
			// ������� Stream Objects �� ��������� ������ ������ ��� ������� ���� Stream Objects
			if (m_bAllowStreams && m_oBuffer2.IsCommand("stream"))
			{
				if ((pStream = CreateStream(pObject, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen)))
				{
					pObject->InitStream(pStream);
				}
				else
				{
					pObject->Free();
					pObject->InitError();
				}
			}
			else
			{
				Shift();
			}
		}
		else if (m_oBuffer1.IsInt()) 	// ���� ������, ���� �����
		{
			nNum = m_oBuffer1.GetInt();
			Shift();
			if (m_oBuffer1.IsInt() && m_oBuffer2.IsCommand("R"))
			{
				pObject->InitRef(nNum, m_oBuffer1.GetInt());
				Shift();
				Shift();
			}
			else
			{
				pObject->InitInt(nNum);
			}
		}
		else if (m_oBuffer1.IsString() && sDecryptKey) // ������
		{
			StringExt *seTemp = m_oBuffer1.GetString();
			StringExt *seRes = new StringExt();
			oTemp.InitNull();
			DecryptStream *pDecrypt = new DecryptStream(new MemoryStream(seTemp->GetBuffer(), 0, seTemp->GetLength(), &oTemp), sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen);
			pDecrypt->Reset();
			int nChar = 0;
			while ((nChar = pDecrypt->GetChar()) != EOF)
			{
				seRes->Append((char)nChar);
			}
			delete pDecrypt;
			pObject->InitString(seRes);
			Shift();
		}
		else // ������� ������
		{
			m_oBuffer1.Copy(pObject);
			Shift();
		}

		return pObject;
	}

	Stream *Parser::CreateStream(Object *pDict, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen)
	{
		Object oTemp;
		unsigned int nLength, unEndPos;

		m_pLexer->SkipToNextLine();
		unsigned int unPos = m_pLexer->GetPos();

		// ��������� �����
		pDict->DictLookup("Length", &oTemp);
		if (oTemp.IsInt())
		{
			nLength = (unsigned int)oTemp.GetInt();
			oTemp.Free();
		}
		else
		{
			oTemp.Free();
			unsigned int unEndPos = unPos;

			while (!m_oBuffer2.IsCommand("endstream") && !m_oBuffer2.IsEOF())
			{
				unEndPos = m_pLexer->GetPos();
				Shift();
			}

			nLength = unEndPos - unPos;
			m_pLexer->SetPos(unPos);

			// ������� �� ������ ������ ��������
			if (nLength <= 0)
				nLength = 5000;
		}

		// ������ �����, ���� ���� ���������
		if (m_pXref && m_pXref->GetStreamEnd(unPos, &unEndPos))
		{
			nLength = unEndPos - unPos;
		}

		// � ������ ������������ ������ PDF, ���������� ����� ������ �����
		// ����� ��� ������
		if (!m_pLexer->GetStream())
		{
			return NULL;
		}

		BaseStream *pBaseStream = m_pLexer->GetStream()->GetBaseStream();

		// ���������� ������ ������
		m_pLexer->SetPos(unPos + nLength);

		// ��������� 'endstream'
		Shift();  // '>>'
		Shift();  // 'endstream'
		if (m_oBuffer1.IsCommand("endstream"))
		{
			Shift();
		}
		else
		{
			// TO DO : Error "Missing 'endstream'"

			// �������� ��� ������������ ������: 
			nLength += 5000;
		}

		Stream *pStream = pBaseStream->MakeSubStream(unPos, true, nLength, pDict);

		// Decryption
		if (sDecryptKey)
		{
			pStream = new DecryptStream(pStream, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen);
		}

		// Filters
		pStream = pStream->AddFilters(pDict);

		return pStream;
	}

	void Parser::Shift()
	{
		if (m_nInlineImage > 0)
		{
			if (m_nInlineImage < 2)
			{
				++m_nInlineImage;
			}
			else
			{
				// � ������������ ������, ���� 'ID' ���������� � �������� ������,
				// ����� �������� ���������
				m_nInlineImage = 0;
			}
		}
		else if (m_oBuffer2.IsCommand("ID"))
		{
			m_pLexer->SkipChar(); // ���������� ������ ����� ������� 'ID'
			m_nInlineImage = 1;
		}
		m_oBuffer1.Free();
		m_oBuffer1 = m_oBuffer2;
		if (m_nInlineImage > 0) // �� ������������� ������ Inline Image
			m_oBuffer2.InitNull();
		else
			m_pLexer->GetObject(&m_oBuffer2);
	}
}