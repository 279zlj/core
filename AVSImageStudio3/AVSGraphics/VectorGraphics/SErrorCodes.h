#ifndef _SERROR_CODES_H
#define _SERROR_CODES_H

//-------------------------------------------------------------------------------------------------------------------------------

#define SNoError                 0  // ��� ������

#define SErrorNoCurrentPoint     1  // � Path ��� ������� �����

#define SErrorEmptyPath          2  // � Path ��� �����

#define SErrorWrongPath          3  // � Subpath ������ ���� �����

#define SErrorNoSaveState        4  // ���� ��������� SState ������

#define SErrorOpenFile           5  // ������ ��� �������� �����

#define SErrorNoGlyph            6  // ���������� �������� ������ ������

#define SErrorColorModeMismatch  7  // ������������ �������� �������

#define SErrorSingularMatrix     8  // ������� ����������

#define SErrorTooSmallBMP        9  // ����������� ������� ��������� ��� ���������� ��� � BMP

#define SErrorMemory            10  // ������������ ������

#define SErrorTemp              11  // ���������� ������� ��������� ����

#define SErrorAVSImageStudio    12  // ������ ��� ������ � AVSImageStudio

#endif /* _SERROR_CODES_H */
