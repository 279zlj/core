#pragma once

void IPPClearImage(void* pData, int nSize, DWORD nColor);

BOOL IPPEffectBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level);
BOOL IPPEffectSharpen(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level);
BOOL IPPEffectMosaic(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size, BOOL bSimple);
BOOL IPPEffectGaussianBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size);
BOOL IPPEffectNoise(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Level, BOOL bMono);
BOOL IPPEffectDiffuse(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Distance);
BOOL IPPEffectMinimal(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size);
BOOL IPPEffectMaximal(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size);
BOOL IPPEffectDeinterlace(LPBYTE pPixels, int nWidth, int nHeight, double Frame, BOOL bEven, BOOL bInterpolate);
BOOL IPPEffectMedian(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long MedianType);

BOOL IPPTransformTwirl(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double Angle, long Degree, DWORD dwBackColor);
BOOL IPPTransformSphere(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long CenterX, long CenterY, long RadiusX, long RadiusY, double Degree, DWORD dwBackColor);
BOOL IPPTransformCylinder(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Center, long Radius, double Degree, long Type, DWORD dwBackColor);

// ���������� ������ ����� ��� �������� size > 10
BOOL IPPEffectGaussianBlur2(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterPrewitt(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

/*
	������ �������� ������ ����� �� �����������, ��������� �� ��� ������
	� ���������� � ����� Frame � ������ �� �������� ������� �������� ��������
*/
BOOL IPPFilterScharr(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

/*
	������ �������� ������ ����� �� �����������, ��������� �� ��� ������
	� ���������� � ����� Frame � ������ �� �������� ������� �������� ��������
*/
BOOL IPPFilterSobel(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterSobelExt(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterRoberts(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterLaplace(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterHipass(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterLowpass(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Type);

//�� �������������� ������ ����������� � ������ ������
BOOL IPPFilterBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size);

/*
	������ �������� ������ ����� �� �����������, ��������� �� ��� ������
	� ���������� � ����� Frame � ������ �� �������� ������� �������� ��������
*/
BOOL IPPFilterCanny(LPBYTE pPixels, int nWidth, int nHeight, double Frame, double LowThresh, double HighThresh);


/*
	NewImageChannel[0] = OldImageChannel[nChannel0];
	NewImageChannel[1] = OldImageChannel[nChannel1];
	NewImageChannel[2] = OldImageChannel[nChannel2];
*/
BOOL IPPUtilitySwapChannels(LPBYTE pPixels, int nWidth, int nHeight, BYTE nChannel0, BYTE nChannel1, BYTE nChannel2);


