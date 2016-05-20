#pragma once
#include "../CustomShape.h"

// 126
class CFlowChartSortType : public CCustomShape
{
public:
	CFlowChartSortType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l,10800,10800,21600,21600,10800xem,10800nfl21600,10800e");
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("5400,5400,16200,16200"));
    }
};


