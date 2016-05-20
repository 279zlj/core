#pragma once

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			/* @desc: */
			// ������, ������������ ��������� ����������
			const int c_nErrorNone = 0;								// ��� ������ - ������ ���
			const int c_nErrorInvalidPointer = -1;					// ��������� �� ��������� ��� �����-�� ����������� ������� �������� - NULL
			const int c_nErrorInvalidInterface = -2;				// ��������� �� ��������� �� NULL, �� ��������� �� ��������� �� ���� ����, ������� ���������
			const int c_nErrorInvalidFormat = -3;					// ������ ��������, ������� � ���������� �� ��������� � ���������
			const int c_nErrorNoMemory = -4;						// �� ������� ������ ��� ����, ����� ���������� �����-�� ��������
			const int c_nErrorInvalidSourceImage = -10;				// ������� �������� ���������
			const int c_nErrorNotImplemented = -100;				// ���������������� ��� �� �����������
			const int c_nErrorBaseInterfacesNotFound = -200;		// �� ������� ����������� ���������� (������ �������������� ����������)
			const int c_nErrorGdiplusFailure = -300;				// ��������� ������ � gdiplus � ��������� �� ������������� ����������; �������� gdiplus �� ���������������, ���� ���������, �������� � gdiplus ����� ���������
			const int c_nErrorPixelNotFound = -400;					// ������ � ��������� ������������ ��������� ��� �������������� ��������
			const int c_nErrorPathIsEmpty = -500;					// ����������� ���� (GraphicsPath) �� �������� �����
			const int c_nErrorInvalidPointsCount = -510;			// ���������� ����� �����������
			const int c_nErrorAlphaCopositionFailure = -600;		// ���������� �������, ����������� �������� ������ �� ������ ��������� �������������� ���������
			const int c_nErrorImageCopositionFailure = -610;		// ���������� �������, ����������� �������� �������� �� ������ ��������� �������������� ���������
			const int c_nErrorTextPainterFailure = -700;			// ��������� ������ � text painter'� ��� ������ ������� DrawText (��. ImagePaintText.h) �������� ������������� text painter'� �������� (�.�. ���������� �������� �� ���������� ���������)
			/* @desc */

			/* @desc: */
			// ���� ������ (������������ ������), ������� �������������� �������� GDIPLUS_DrawCredits
			const int c_nCreditsHMMBottom	= 5100;
			const int c_nCreditsHMMCenter	= 5101;
			const int c_nCreditsHMMTop		= 5102;
			const int c_nCreditsHMWBottom	= 5200;
			const int c_nCreditsHMWCenter	= 5201;
			const int c_nCreditsHMWTop		= 5202;
			const int c_nCreditsHMSBottom	= 5300;
			const int c_nCreditsHMSCenter	= 5301;
			const int c_nCreditsHMSTop		= 5302;
			const int c_nCreditsHMHBottom	= 5400;
			const int c_nCreditsHMHCenter	= 5401;
			const int c_nCreditsHMHTop		= 5402;
			const int c_nCreditsHHMBottom	= 5500;
			const int c_nCreditsHHMCenter	= 5501;
			const int c_nCreditsHHMTop		= 5502;

			const int c_nCreditsVMMLeft		= 6100;
			const int c_nCreditsVMMCenter	= 6101;
			const int c_nCreditsVMMRight	= 6102;
			const int c_nCreditsVMWLeft		= 6200;
			const int c_nCreditsVMWCenter	= 6201;
			const int c_nCreditsVMWRight	= 6202;
			const int c_nCreditsVMSLeft		= 6300;
			const int c_nCreditsVMSCenter	= 6301;
			const int c_nCreditsVMSRight	= 6302;
			const int c_nCreditsVMHLeft		= 6400;
			const int c_nCreditsVMHCenter	= 6401;
			const int c_nCreditsVMHRight	= 6402;
			const int c_nCreditsVHMLeft		= 6500;
			const int c_nCreditsVHMCenter	= 6501;
			const int c_nCreditsVHMRight	= 6502;
			/* @desc */

			enum EImageFileType
			{
				c_nImageFileTypeAny    = 0,
				c_nImageFileTypeBmp    = 1,
				c_nImageFileTypeJpg    = 2,
				c_nImageFileTypeGif    = 3,
				c_nImageFileTypeTiff   = 4,
				c_nImageFileTypePsd    = 5,
				c_nImageFileTypeTga    = 6,
				c_nImageFileTypeRas    = 7,
				c_nImageFileTypePcx    = 8,
				c_nImageFileTypeIPod   = 9,
				c_nImageFileTypeRaw    = 10,
				c_nImageFileTypeJp2    = 11,
			};
		}
	}
}