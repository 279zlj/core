#ifndef _ERROR_CONSTANTS_H
#define _ERROR_CONSTANTS_H

#define ErrorNone             0 // ��� ������

#define ErrorOpenFile         1 // ������ ��� �������� PDF �����

#define ErrorBadCatalog       2	// couldn't read the page catalog

#define ErrorDamaged          3 // PDF ���� ��� ��������� � ��� ���������� ������������

#define ErrorEncrypted        4 // ���� ����������, ����������� �� ��������

#define ErrorHighlightFile    5	// nonexistent or invalid highlight file

#define ErrorBadPrinter       6 // ������ �������

#define ErrorPrinting         7 // ������ �� ����� ������

#define ErrorPermission       8	// ������ ��������� � ������������� ����������� �� ����

#define ErrorBadPageNum       9	// �������� ���������� �������

#define ErrorFileIO          10 // ������ ��� ������/������

#endif /* _ERROR_CONSTANTS_H */
