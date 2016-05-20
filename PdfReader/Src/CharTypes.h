#ifndef _PDF_READER_CHARTYPES_H
#define _PDF_READER_CHARTYPES_H

namespace PdfReader
{
	// Unicode-�������.
	typedef unsigned int Unicode;

	// ��� ��������� CID.
	typedef unsigned int CID;

	// ����� ���, ��������� ������ �� ��������� ���������:
	// - 8-������ �������
	// - 16-������ CID
	// - Unicode
	typedef unsigned int CharCode;
}

#endif // _PDF_READER_CHARTYPES_H
