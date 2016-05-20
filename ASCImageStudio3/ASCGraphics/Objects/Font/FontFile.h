#ifndef _FONT_FILE_H
#define _FONT_FILE_H

class CFontEngine;
class CFont;

//-------------------------------------------------------------------------------------------------------------------------------
// CFontFile
//-------------------------------------------------------------------------------------------------------------------------------

class CFontFile 
{
public:

	virtual ~CFontFile();

	// ������� ����� ������ CFont, ��� ������� FontFile.
	virtual CFont *MakeFont() = 0;

	// ������� ������. ������� ������ ���� ������� ����� 0.
	void AddRef();
	void Release();

	CStringW GetFontPath() const
	{
		return m_wsFileName;
	}

	long GetFontIndex() const
	{
		return m_lIndex;
	}

protected:

	CFontFile(wchar_t *wsFileName, long lIndex);

private:

	CStringW     m_wsFileName; // ���� � �����
	long         m_lIndex;     // ����� ������ � �����

	int          m_nRef;       // �������� ������

	friend class CFontEngine;
};

#endif /* _FONT_FILE_H */
