#ifndef _OFFICEDEFINES_H_
#define _OFFICEDEFINES_H_

//AVSOfficeImageFile: TIFFOptions in DocumentConverter.xml

namespace TiffCompression
{
	const int None = 0;
	const int Lzw = 1;
	const int Rle = 2;
	const int CCITT3 = 3;
	const int CCITT4 = 4;
}

//AVSOfficeTxtFile: TXTOptions in DocumentConverter.xml

namespace EncodingType
{
	//from unicodeconverter_encodings.h
	const int Utf8		= 46;
	const int Unicode	= 50;
	const int Ansi		= 1000;
	const int BigEndian = 51;
}

#define PERMISSION_PRINT  ( 1 << 2 ) // ����������� ������ ���������
#define PERMISSION_CHANGE ( 1 << 3 ) // ����������� �� ��������� ����������� ��������� (�����, �������, ��������)
#define PERMISSION_COPY   ( 1 << 4 ) // ����������� �� ����������� ��������� � ��� �����������

//��� ������
const int c_nPrintTypeFit = 0; //��������� �� �������
const int c_nPrintTypeStrech = 1; //�����������
const int c_nPrintType100 = 2; // �������� ��� ����
//������ xml ��� ������� PrintPage <PrintSettings><Mode ZoomEnable=\"0\" RotateEnable=\"0\" PrintableAreaEnable=\"0\">%d</Mode></PrintSettings>
//enable - 0; diable - 1 ( ���� ������� �� �����, �� �� ������� ��� 0 )
//ZoomEnable ����������� ����� �������� �� �������� ��������
//RotateEnable ������������ ��������,���� � ���������� ��������� �� ����� �������� ������� �������(�������� ��� c_nPrintType100) ��� ���� � ������� ��������� �� �� ���������, � � ���������� �����������
//PrintableAreaEnable ������ ������ � ������� ������ ��������

namespace TextFormatting
{
  const int c_nPerCharacter = 0;
  const int c_nLineByLine = 1;
  const int c_nWordProcessing = 2;
}

//additional param ���������� ���������� ���������� � ���� ��� IAVSOfficeFileConverter
const int c_nSaveModeNone = 0;
const int c_nSaveModeStart = 1;
const int c_nSaveModeContinue = 2;
const int c_nSaveModeCommit = 4;

#endif //_OFFICEDEFINES_H_
