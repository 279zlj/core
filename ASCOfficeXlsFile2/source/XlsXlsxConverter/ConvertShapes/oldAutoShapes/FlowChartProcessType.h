#pragma once
#include "../CustomShape.h"

// 109
namespace NSCustomShapesConvert
{
class CFlowChartProcessType : public CCustomShape
{
public:
	CFlowChartProcessType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,l,21600r21600,l21600,xe");
        LoadConnectorsList(_T("Rectangle"));
    }
};
}