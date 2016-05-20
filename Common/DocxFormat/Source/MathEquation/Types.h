#ifndef _MATH_EQUATION_TYPES_H
#define _MATH_EQUATION_TYPES_H

#include <map>
#include "../../../../DesktopEditor/common/Types.h"

namespace MathEquation
{

	typedef unsigned char  uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int   uint32_t;
	typedef signed char    int8_t;
	typedef signed short   int16_t;
	typedef signed int     int32_t;

	typedef unsigned short Unicode_t;
	typedef signed char    Char_t;

	struct TMathFont
	{
		std::string sName;
		bool        bItalic;
		bool        bBold;
	};

	typedef std::map<uint8_t, TMathFont> TFontMap;

	enum MBRACKETSTYPE
	{
		bracketsAngle            = 0x00, // < > ������� ������
		bracketsRound            = 0x01, // ( ) ������� ������
		bracketsCurve            = 0x02, // { } �������� ������
		bracketsSquare           = 0x03, // [ ] ���������� ������
		bracketsLine             = 0x04, // | | ������ � ���� �����
		bracketsDLine            = 0x05, // || ||������ � ���� ������� �����
		bracketsFloor            = 0x06, // ������ � ����� �����
		bracketsCeil             = 0x07, // ������ � ����� �������
		bracketsSquareOpenOpen   = 0x08, // [ [
		bracketsSquareCloseClose = 0x09, // ] ]
		bracketsSquareCloseOpen  = 0x0a, // ] [
		bracketsSquareRound      = 0x0b, // [ )
		bracketsRoundSquare      = 0x0c  // ( ]

	};

	enum MFRACTIONTYPES
	{
		fractionRegular = 0, // ������� �����
		fractionSlanted = 1  // ��������� �����
	};

	enum MSCRIPTALIGN
	{
		scriptalignRight  = 0,
		scriptalignCenter = 1,
		scriptalignLeft   = 2
	};

	enum MBARTYPE
	{
		bartypeLine        = 0, // ������� �����
		bartypeDoubleLine  = 1, // ������� �����
		bartypeArrowLeft   = 2, // ������� ������
		bartypeArrowRight  = 3, // ������� �������
		bartypeArrowDouble = 4  // ������� ������� (� ������, � �������)
	};

	enum MARROWTYPE
	{
		arrowtypeLeft   = 0, // ������� ������
		arrowtypeRight  = 1, // ������� �������
		arrowtypeDouble = 2  // ������� ������� (� ������, � �������)
	};

	enum MINTEGRALTYPE
	{
		integraltypeSingle             = 0, // ������� ��������� �������� ��� ��������
		integraltypeSingleRSub         = 1, // ��������� �������� � ������ ������ ��������
		integraltypeSingleRSubSup      = 2, // ��������� �������� � ������� ������� � ������ ���������
		integraltypeSingleOriented     = 3, // ��������� ��������������� ��������
		integraltypeSingleOrientedRSub = 4, // ��������� ��������������� �������� � ������ ������ ��������

		integraltypeDouble             = 5,  // ������� �������� ��� ��������
		integraltypeDoubleRSub         = 6,  // ������� �������� � ������ ������ ��������
		integraltypeDoubleOriented     = 7,  // ������� ��������������� ��������
		integraltypeDoubleOrientedRSub = 8,  // ������� ��������������� �������� � ������ ������ ��������

		integraltypeTriple             = 9,  // ������� �������� ��� ��������
		integraltypeTripleRSub         = 10, // ������� �������� � ������ ������ ��������
		integraltypeTripleOriented     = 11, // ������� ��������������� ��������
		integraltypeTripleOrientedRSub = 12, // ������� ��������������� �������� � ������ ������ ��������

		integraltypeSingleCSubSup      = 13, // ��������� �������� � ������������ ������� � ������ ���������
		integraltypeSingleCSub         = 14, // ��������� �������� � ����������� ������ ��������
		integraltypeSingleOrientedCSub = 15, // ��������� ��������������� �������� � ����������� ������ ��������

		integraltypeDoubleOrientedCSub = 16, // ������� ��������������� �������� � ����������� ������ ��������
		integraltypeDoubleCSub         = 17, // ������� �������� � ����������� ������ ��������

		integraltypeTripleOrientedCSub = 18, // ������� ��������������� �������� � ����������� ������ ��������
		integraltypeTripleCSub         = 19  // ������� �������� � ����������� ������ ��������
	};

	enum MNARRAYTYPE
	{
		narySumCSub          = 0,  // ���� ����� � ����������� ������ ��������
		narySumCSubSup       = 1,  // ���� ����� � ������������ ������ � ������� ���������
		narySum              = 2,  // ���� ����� ��� ��������

		narySumRSub          = 3,  // ���� ����� � ������ ������ ��������
		narySumRSubSup       = 4,  // ���� ����� � ������� ������ � ������� ���������

		naryProdCSub         = 5,  // ���� ������������ � ����������� ������ ��������
		naryProdCSubSup      = 6,  // ���� ������������ � ������������ ������ � ������� ���������
		naryProd             = 7,  // ���� ������������  ��� ��������

		naryProdRSub         = 8,  // ���� ������������ � ������ ������ ��������
		naryProdRSubSup      = 9,  // ���� ������������ � ������� ������ � ������� ���������

		naryCoProdCSub       = 10, // ���� �������������� � ����������� ������ ��������
		naryCoProdCSubSup    = 11, // ���� �������������� � ������������ ������ � ������� ���������
		naryCoProd           = 12, // ���� ��������������  ��� ��������

		naryCoProdRSub       = 13, // ���� �������������� � ������ ������ ��������
		naryCoProdRSubSup    = 14, // ���� �������������� � ������� ������ � ������� ���������

		naryUnionCSub        = 15, // ���� ����������� � ����������� ������ ��������
		naryUnionCSubSup     = 16, // ���� ����������� � ������������ ������ � ������� ���������
		naryUnion            = 17, // ���� �����������  ��� ��������

		naryUnionRSub        = 18, // ���� ����������� � ������ ������ ��������
		naryUnionRSubSup     = 19, // ���� ����������� � ������� ������ � ������� ���������

		naryIntersectCSub    = 20, // ���� ����������� � ����������� ������ ��������
		naryIntersectCSubSup = 21, // ���� ����������� � ������������ ������ � ������� ���������
		naryIntersect        = 22, // ���� �����������  ��� ��������

		naryIntersectRSub    = 23, // ���� ����������� � ������ ������ ��������
		naryIntersectRSubSup = 24  // ���� ����������� � ������� ������ � ������� ���������
	};

	enum MLONGDIVISION
	{
		longdivisionWithResult = 0, // ���� ������� ��������� ��� ��������
		longdivisionRegular    = 1  // ���� ������� ��������� � �������
	};

	enum MANGLEBRACKETSWITHSEPARATORTYPE
	{
		angbrwithsepBoth  = 0, // < | >
		angbrwithsepLeft  = 1, // < |
		angbrwithsepRight = 2  // | >		
	};

	enum MEMBELTYPE
	{
		embelDot         = 0x02, // �����         .
		embelDDot        = 0x03, // ��� �����     ..  
		embelDDDot       = 0x04, // ��� �����     ...
		embelPrime       = 0x05, // �����         '
		embelDPrime      = 0x06, // ��� ������    "
		embelLPrime      = 0x07, // ����� �����   '
		embelTilde       = 0x08, // �����         ~
		embelHat         = 0x09, // ������(�����) ^
		embelSlash       = 0x0a, // ����� �����   /
		embelRArrow      = 0x0b, // ������� ������ <-
		embelLArrow      = 0x0c, // ������� ����� ->
		embelDArrow      = 0x0d, // ������� � ��� ������� <->
		embelRHarpoon    = 0x0e, // ������ ������
		embelLHarpoon    = 0x0f, // ������ �����
		embelStrikeout   = 0x10, // ������������
		embelBar         = 0x11, // �����
		embelTPrime      = 0x12, // ��� ������ '''
		embelFrown       = 0x13, // ���� �����
		embelSmile       = 0x14, // ���� ����

		embelMin         = 0x02,
		embelMax         = 0x14
	};

	enum MMATRIXBORDERTYPE
	{
		matrixborderNone  = 0x00,
		matrixborderSolid = 0x01,
		matrixborderDash  = 0x02,
		matrixborderDot   = 0x03
	};

	enum MMATRIXHORALIGN
	{
		matrixhoralignLeft      = 0x00, // ���������� � ������ ����
		matrixhoralignCenter    = 0x01, // ���������� �� ������
		matrixhoralignRight     = 0x02, // ���������� � ������� ����
		matrixhoralignEqualSign = 0x03, // ���������� �� ����� =
		matrixhoralignCommaSign = 0x04  // ���������� �� ����� ,
	};

	enum MMATRIXVERALIGN
	{
		matrixveralignTop      = 0x00,
		matrixveralignBaseLine = 0x01,
		matrixveralginBottom   = 0x02
	};

	enum MSPECIALSYMBOLS
	{
		specialsymAlignment  = 0xeb00,
		specialsymZeroSpace  = 0xeb01,
		specialsymThinSpace  = 0xeb02,
		specialsymThickSpace = 0xeb04,
		specialsymLargeSpace = 0xeb05,
		specialsymOnePtSpace = 0xeb08
	};
}

#endif _MATH_EQUATION_OUTPUTDEV_H
