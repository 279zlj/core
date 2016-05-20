#pragma once
#include "../CustomShape.h"

// 132
class CFlowChartMagneticDiskType : public CCustomShape
{
public:
	CFlowChartMagneticDiskType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,qx,3391l,18209qy10800,21600,21600,18209l21600,3391qy10800,xem,3391nfqy10800,6782,21600,3391e");
        LoadConnectorsList(_T("10800,6782;10800,0;0,10800;10800,21600;21600,10800"));
        
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		LoadTextRect(_T("0,6782,21600,18209"));
    }
};


