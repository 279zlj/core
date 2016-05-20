#pragma once
#include <windows.h>

#ifndef _IMAGECONVERTERDEFINES_H_ // For Borland compiler
#define _IMAGECONVERTERDEFINES_H_

#define AVS_ERROR_FIRST                   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0300)

#define IMAGECONVERTER_UNEXPECTED_ERROR         (AVS_ERROR_FIRST + 0x0000) //����������� ������
#define IMAGECONVERTER_ERROR_XML_FORMAT         (AVS_ERROR_FIRST + 0x0001) //������� XML ����� ������������ ������
#define IMAGECONVERTER_ERROR_CREATE_HANDLE      (AVS_ERROR_FIRST + 0x0002) //������ ��� �������� Mutex or Event or OtherHandle
#define IMAGECONVERTER_ERROR_INPUT_DATA         (AVS_ERROR_FIRST + 0x0003) //������� ������
#define IMAGECONVERTER_ERROR_FILE_NOT_EXIST     (AVS_ERROR_FIRST + 0x0004) //�������� ���� �� ����������
//#define IMAGECONVERTER_ERROR_FILECACHE        (AVS_ERROR_FIRST + 0x0005) //������ FileCache
#define IMAGECONVERTER_ERROR_IMAGESTUDIO        (AVS_ERROR_FIRST + 0x0006) //������ ImageStudio
#define IMAGECONVERTER_ERROR_CONVERT            (AVS_ERROR_FIRST + 0x0007) //������ ��� �����������
#define IMAGECONVERTER_ERROR_IMAGEFILE          (AVS_ERROR_FIRST + 0x0008) //������ ��� �������� ����� ImageFile3
#define IMAGECONVERTER_ERROR_REMOVE_TIFF        (AVS_ERROR_FIRST + 0x000a) //������ �������� tiff ��� gif ����� ��� Stop
#define IMAGECONVERTER_ERROR_REMOVE_TEMP_DIR    (AVS_ERROR_FIRST + 0x000b) //������ �������� �������� ���������� ��� ����������� � PDF
#define IMAGECONVERTER_ERROR_PDFFILE_CONTROL    (AVS_ERROR_FIRST + 0x000c) //�� ������� ������� ������� PdfFile
#define IMAGECONVERTER_ERROR_TIFF_SAVE          (AVS_ERROR_FIRST + 0x000d) //������ ���������� � TIFF
#define IMAGECONVERTER_ERROR_GIF_SAVE           (AVS_ERROR_FIRST + 0x000e) //������ ���������� � GIF
#define IMAGECONVERTER_ERROR_FILE_ACCESS_READ   (AVS_ERROR_FIRST + 0x000f) //������ ������� ��� ������
#define IMAGECONVERTER_ERROR_FILE_ACCESS_WRITE  (AVS_ERROR_FIRST + 0x0010) //������ ������� ��� ������
#define IMAGECONVERTER_ERROR_FILE_SAVE			(AVS_ERROR_FIRST + 0x0011) //������ ������� ��� ������

#define IMAGECONVERTER_ERROR_SAVE_PDF_ADDITIONAL    (0x0100) //�������� � ������� PdfFile




#define IMAGECONVERTER_STATE_IDLE           0x0000
#define IMAGECONVERTER_STATE_RUN            0x0001
#define IMAGECONVERTER_STATE_SUSPENDED      0x0002

#define IMAGECONVERTER_LOGOPARAM_VERT_TOP       1<<0  //1
#define IMAGECONVERTER_LOGOPARAM_VERT_CENTER    1<<1  //2
#define IMAGECONVERTER_LOGOPARAM_VERT_BOTTOM    1<<2  //4

#define IMAGECONVERTER_LOGOPARAM_HOR_LEFT       1<<3  //8
#define IMAGECONVERTER_LOGOPARAM_HOR_CENTER     1<<4  //16
#define IMAGECONVERTER_LOGOPARAM_HOR_RIGHT      1<<5  //32

#endif //#ifndef _IMAGECONVERTERDEFINES_H_  // For Borland compiler