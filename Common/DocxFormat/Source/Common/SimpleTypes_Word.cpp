#include "SimpleTypes_Word.h"

namespace SimpleTypes
{	
    template<>
    EHexColor CHexColor<EHexColor::hexcolorAuto>::FromString (CString &sValue)
    {
        if ( _T("auto") == sValue || _T("none") == sValue )
            this->m_eValue = hexcolorAuto;
        else
        {
            //� ������������ �� ��������, ��� ���� ����� ��������� �������, �� � �������� ��������� ����������� � word ��� �����������.
            //CHighlightColor<highlightcolorNone> oHighlightColor(sValue);
            CHighlightColor<> oHighlightColor(sValue);
            if(SimpleTypes::highlightcolorNone != oHighlightColor.GetValue())
            {
                this->m_eValue = hexcolorRGB;
                m_unR = oHighlightColor.Get_R();
                m_unG = oHighlightColor.Get_G();
                m_unB = oHighlightColor.Get_B();
            }
            else if ( 6 <= sValue.GetLength() )
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue.Mid( 0, 6 );
                Parse();
            }
            else if ( 3 == sValue.GetLength() )// a la #339 (Compo 3AP.docx)
            {
                this->m_eValue = hexcolorRGB;
                m_sValue = sValue;
                Parse3();
            }
            else   this->m_eValue = EHexColor::hexcolorAuto;//eDefValue;

        }

        return this->m_eValue;

    }
} // SimpleTypes
