#ifdef _MSC_VER
	#pragma once			// for MSVS
#endif	// #ifdef _MSC_VER

#ifndef _MEDIADATADEFINE_H_	// For Borland compiler
#define _MEDIADATADEFINE_H_

//��������� ��� IAVSMediaData::Dublicate 
#define DUBLICATE_TYPE_LINK				1
#define DUBLICATE_TYPE_COPY				2

#define INGNORETIMESTAMP				0
#define USETIMESTAMP					1
#define USETIMESTAMPALWAYS				2
#define TIMESTAMPCOMPENSATE				3
#define AUDIO_BUFFER_MODE_COUNT			4

//���������� IAVSUncompressedVideoFrame::Allocate, ����� �������� ����������� ����� 
//��� �������� c ������������ ColorSpace � Stride
#define UNCOMPRESSEDALLOCATE			-1

#define RESIZE_MODE_NEAREST				0
#define RESIZE_MODE_LINEAR				1
#define RESIZE_MODE_CUBIC				2
#define RESIZE_MODE_SUPER				3
//���-�� ����� ���������������
#define RESIZE_MODE_COUNT				4

//������ deinterlacing
#define DEINTERLACE_METHOD_BLEND		0
#define DEINTERLACE_METHOD_MIDDLE		1
//���-�� ������� deinterlacing
#define DEINTERLACE_METHOD_COUNT		2

//���-�� ������ � ������� DVD subpicture
#define DVD_SUBPICTURE_COLOR_COUNT		4

#endif // #ifndef _MEDIADATADEFINE_H_	// For Borland compiler