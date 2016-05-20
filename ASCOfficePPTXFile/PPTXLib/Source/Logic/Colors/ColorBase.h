#pragma once
#ifndef PPTX_LOGIC_COLORBASE_INCLUDE_H_
#define PPTX_LOGIC_COLORBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "ColorModifier.h"
#include <windows.h>
#include <list>

namespace PPTX
{
	namespace Logic
	{
		class ColorBase : public WrapperWritingElement
		{
		public:
			ColorBase();
			virtual ~ColorBase();			

		public:
			//��� ������� ���������� ���� � ������ �������������
			virtual DWORD GetRGBA(DWORD RGBA);
			virtual DWORD GetARGB(DWORD ARGB);
			virtual DWORD GetBGRA(DWORD BGRA);
			virtual DWORD GetABGR(DWORD ABGR);
			//���� ������� �������� ����� ��� "origin color" � setter'�
			//���, ��� � ���!!! Setter'� ������ ������ � ��������, �.�. ��� ��������� ����� �������� ��� ���!

		protected:
			//origin color
			unsigned char alpha;
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char hue;
			unsigned char saturation;
			unsigned char luminance;

			enum ColorType {ctRGBA, ctARGB, ctBGRA, ctABGR};
			virtual DWORD ApplyModifiers(const ColorType ct)const;

			//��� ������� ������������ ��� ���������� "origin color"
			virtual void SetRGB2HSL();
			virtual void SetHSL2RGB();

			//��� ������������ ��� ���������� �������������
			static void RGB2HSL(unsigned char* RGB, unsigned char* HSL);
			static void HSL2RGB(unsigned char* HSL, unsigned char* RGB);
			static double Hue_2_RGB(double v1, double v2, double vH);
		public:
			property<std::list<ColorModifier> > Modifiers;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COLORBASE_INCLUDE_H