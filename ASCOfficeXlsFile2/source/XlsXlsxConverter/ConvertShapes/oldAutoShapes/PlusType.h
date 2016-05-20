#pragma once
#include "../CustomShape.h"

// 11
namespace NSCustomShapesConvert
{
class CPlusType : public CCustomShape
{
public:
	CPlusType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,l@0@0,0@0,0@2@0@2@0,21600@1,21600@1@2,21600@2,21600@0@1@0@1,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("prod @0 2929 10000"));
        AddGuide(_T("sum width 0 @3"));
        AddGuide(_T("sum height 0 @3"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("prod height 1 2"));

        m_arAdjustments.push_back(5400);

        LoadConnectorsList(_T("@8,0;0,@9;@8,@7;@6,@9"));
        LoadTextRect(_T("0,0,21600,21600;5400,5400,16200,16200;10800,10800,10800,10800"));

		CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.switchHandle = _T("true");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);

        m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
}