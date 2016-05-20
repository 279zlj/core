#pragma once
#include "../CustomShape.h"

// 15
class CHomePlateType : public CCustomShape
{
public:
	CHomePlateType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@0,l,,,21600@0,21600,21600,10800xe"); 

        AddGuide(_T("val #0")); 
        AddGuide(_T("prod #0 1 2")); 

		m_arAdjustments.push_back(16200);

        LoadConnectorsList(_T("@1,0;0,10800;@1,21600;21600,10800"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("0,0,10800,21600;0,0,16200,21600;0,0,21600,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,21600");
        m_arHandles.push_back(oHandle1);
    }
};


