#ifndef _EMF_OUTPUT_DEVICE_H
#define _EMF_OUTPUT_DEVICE_H

#include "../Wmf/WmfTypes.h"

namespace Metafile
{

	class CEmfOutputDevice
	{
	public:

		CEmfOutputDevice() {}
		virtual ~CEmfOutputDevice() {}

		// ������ � ��������� ������ ����������
		virtual void Begin() = 0;
		virtual void End() = 0;

		// pBuffer - BGRA �������� ��������� ulWidth, ulHeight, ������� ���� ���������� � �������� �����
		virtual void Draw_Bitmap(long lX, long lY, long lW, long lH, BYTE* pBuffer, unsigned long ulWidth, unsigned long ulHeight) = 0;
	};

}

#endif //_EMF_OUTPUT_DEVICE_H