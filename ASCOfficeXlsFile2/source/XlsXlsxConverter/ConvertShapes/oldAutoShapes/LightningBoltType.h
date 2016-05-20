#pragma once
#include "../CustomShape.h"

// 73
namespace NSCustomShapesConvert
{
class CLightningBoltType : public CCustomShape
{
public:
	CLightningBoltType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m8472,l,3890,7602,8382,5022,9705r7200,4192l10012,14915r11588,6685l14767,12877r1810,-870l11050,6797r1810,-717xe");

        LoadConnectorsList(_T("8472,0;0,3890;5022,9705;10012,14915;21600,21600;16577,12007;12860,6080"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("8757,7437,13917,14277"));
    }
};
}