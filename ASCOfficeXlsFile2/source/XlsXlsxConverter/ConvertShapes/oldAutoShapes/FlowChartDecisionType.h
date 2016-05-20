#pragma once
#include "../CustomShape.h"

// 110
class CFlowChartDecisionType : public CCustomShape
{
public:
	CFlowChartDecisionType()

    {
        m_bConcentricFill = true;

        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m10800,l,10800,10800,21600,21600,10800xe");

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("5400,5400,16200,16200"));
    }
};
