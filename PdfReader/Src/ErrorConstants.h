#ifndef _PDF_READER_ERROR_CONSTANTS_H
#define _PDF_READER_ERROR_CONSTANTS_H

namespace PdfReader
{
	typedef enum
	{
		errorNone          =  0, // ��� ������
		errorOpenFile      =  1, // ������ ��� �������� PDF �����
		errorBadCatalog    =  2, // couldn't read the page catalog
		errorDamaged       =  3, // PDF ���� ��� ��������� � ��� ���������� ������������
		errorEncrypted     =  4, // ���� ����������, ����������� �� ��������
		errorHighlightFile =  5, // nonexistent or invalid highlight file
		errorBadPrinter    =  6, // ������ �������
		errorPrinting      =  7, // ������ �� ����� ������
		errorPermission    =  8, // ������ ��������� � ������������� ����������� �� ����
		errorBadPageNum    =  9, // �������� ���������� �������
		errorFileIO        = 10  // ������ ��� ������/������
	} EError;
}

#endif // _PDF_READER_ERROR_CONSTANTS_H
