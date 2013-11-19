#pragma once
#include "../../AVSOfficeStudio/Common/OfficeDrawing/ElementsContainer.h"
#include "../Records/TextFullSettings.h"

class CSlide : public CElementsContainer
{
public:
	enum SlideType
	{
		stSlide		= 0x00,
		stMaster	= 0x01,
		stNote		= 0x02,
		stNone      = 0xFF
	};
public:
	// SlideListWithText properties
	CAtlArray<CTextFullSettings> m_arTextPlaceHolders;

	// indentLevels
	CSimpleMap<DWORD, CTextMasterStyle*> m_mapTextStyles;
	
	SlideType m_eSlideType;
	
	double m_dReadDuration;

public:
	CSlide() : CElementsContainer(),
		m_arTextPlaceHolders(), m_mapTextStyles()
	{
		m_eSlideType = stNone;
		m_dReadDuration = -1.0;

		m_parEmptyPictures = NULL;
	}

public:

	void SetUpTextStyle(IElement* pElem)
	{
		// ������� ��������� �� shape
		// ����� ��������� ��� ��������� ��-�������
		// 1) master + TextMasterStyles
		// 2) persist + TextMasterStyles
		// 3) ���� ��������� + TextMasterStyles
		// ������ "���� ���������" - ��� ����� "����������� ���������"
		// ������ ��� ��� ����� ( ����� �������� ) - ��� �����������
		// ��� ����� ��������� ���������� �������.
		// ������ - ������ ����� ������� ��� ������, 
		// �, ����� ������ ������ ������ .cpp ����� - ����� �� ����� ��������� 
		// ��������� ������ (�.�. ��������� �� ����� �������� �� ��������)

		if (NULL == pElem)
			return;

		if (etShape != pElem->m_etType)
			return;

		CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
		if (NULL == pShape)
			return;

		// � ��� ���� ������, ������ ��� ����� ����� ���� ��� �� ���������
		//if (_T("") == pShape->m_oShape.m_oText.m_sText) // ������ ���� - ��� ���� ����� ��� ������??
		//	return;

		CTextAttributesEx* pTextSettings = &(pShape->m_oShape.m_oText);

		// ������� �������� ������ �� masterstyle (��� ���������� ��������)
		// ��� ������ - ������ ���� �� ������� (�.�. ��� ����� ��������� ���� ������ ������)
		// - ������ ����� ������ �� ����� - �.�. � ������� ���� ���������� ��� ������� - 
		// � ��� ��� ��� ������ ��� �����������
		BOOL bIsPersistPresentSettings = FALSE;
		BOOL bIsOwnPresentSettings = FALSE;

		NSOfficePPT::TextType eTypeMaster = (NSOfficePPT::TextType)pTextSettings->m_lMasterTextType;
		NSOfficePPT::TextType eTypePersist = NSOfficePPT::NoPresent;
		NSOfficePPT::TextType eTypeOwn = (NSOfficePPT::TextType)pTextSettings->m_lTextType;

		//  persist ----------------------------------------------------------------------
		LONG lCountPersistObjects = (LONG)m_arTextPlaceHolders.GetCount();
		LONG lPersistIndex = pShape->m_lPersistIndex;

		if ((lPersistIndex >= 0) && (lPersistIndex < lCountPersistObjects))
		{
			eTypePersist = (NSOfficePPT::TextType)m_arTextPlaceHolders[lPersistIndex].m_nTextType;
			m_arTextPlaceHolders[lPersistIndex].ApplyProperties(pTextSettings);

			bIsPersistPresentSettings = ((NULL != m_arTextPlaceHolders[lPersistIndex].m_pTextStyleProp) && 
					(0 < m_arTextPlaceHolders[lPersistIndex].m_pTextStyleProp->m_lCount));

		}
		//  ------------------------------------------------------------------------------

		//  own properties ---------------------------------------------------------------
		if (NULL != pShape->m_pStream)
		{
			// ������ ����� ��������� ����������� ��������� ������ �� ������.
			LONG lPosition = 0; StreamUtils::StreamPosition(lPosition, pShape->m_pStream);

			if (-1 != pShape->m_lOffsetTextStyle)
			{
				StreamUtils::StreamSeek(pShape->m_lOffsetTextStyle - 8, pShape->m_pStream);

				SRecordHeader oHeader;
				pShape->m_pStream->Read(&oHeader, sizeof(oHeader), NULL);

				if (RECORD_TYPE_STYLE_TEXTPROP_ATOM == oHeader.RecType)
				{			
					CRecordStyleTextPropAtom* pStyle = new CRecordStyleTextPropAtom();
					pStyle->m_lCount = pShape->m_oShape.m_oText.m_sText.GetLength();
					
					pStyle->ReadFromStream(oHeader, pShape->m_pStream);
					pStyle->ApplyProperties(&(pShape->m_oShape.m_oText));

					bIsOwnPresentSettings = (0 < pStyle->m_lCount);

					RELEASEOBJECT(pStyle);
				}
			}

			if (-1 != pShape->m_lOffsetTextProp)
			{
				StreamUtils::StreamSeek(pShape->m_lOffsetTextProp - 8, pShape->m_pStream);

				SRecordHeader oHeader;
				pShape->m_pStream->Read(&oHeader, sizeof(oHeader), NULL);

				if (RECORD_TYPE_TEXTSPECINFO_ATOM == oHeader.RecType)
				{			
					CRecordTextSpecInfoAtom* pSpecInfo = new CRecordTextSpecInfoAtom();
					pSpecInfo->m_lCount = pShape->m_oShape.m_oText.m_sText.GetLength();
					pSpecInfo->ReadFromStream(oHeader, pShape->m_pStream);
					pSpecInfo->ApplyProperties(&(pShape->m_oShape.m_oText));

					RELEASEOBJECT(pSpecInfo);
				}
			}

			StreamUtils::StreamSeek(lPosition, pShape->m_pStream);
		}
		//  ------------------------------------------------------------------------------

		SetUpAllTextType(pTextSettings, eTypeMaster, eTypePersist, bIsPersistPresentSettings, eTypeOwn, bIsOwnPresentSettings);

		// ������ �������� - ���� ���� ������ ��������� �� ��������� � ������� - 
		// ������� �� ��������� ��� ������ ���� ������
		// �� ���� ����� ��� ������ ���������

		if (0 == pShape->m_oShape.m_oText.m_arPFs.GetCount())
		{
			CTextMasterStyle* pStyle = this->m_mapTextStyles.Lookup(pShape->m_oShape.m_oText.m_lTextType);
			if (NULL != pStyle)
			{
				if (0 < pStyle->m_lLevels)
				{
					pShape->m_oShape.m_oText.m_arPFs.Add(pStyle->m_arrLevels[0].oPFRun);
					pShape->m_oShape.m_oText.m_arPFs[0].lCount = pShape->m_oShape.m_oText.m_sText.GetLength();
				}
			}
		}
		if (0 == pShape->m_oShape.m_oText.m_arCFs.GetCount())
		{
			CTextMasterStyle* pStyle = this->m_mapTextStyles.Lookup(pShape->m_oShape.m_oText.m_lTextType);
			if (NULL != pStyle)
			{
				if (0 < pStyle->m_lLevels)
				{
					pShape->m_oShape.m_oText.m_arCFs.Add(pStyle->m_arrLevels[0].oCFRun);
					pShape->m_oShape.m_oText.m_arCFs[0].lCount = pShape->m_oShape.m_oText.m_sText.GetLength();
				}
			}
		}

		// ������ ����� ����������, �������� �� ���� placeholder'��
		// ���� ��, �� ��������� ��� ��������� �� ���� ���������
		LONG lPlaceholderIndex = pShape->m_lPlaceholderPosition;
		if (-1 != lPlaceholderIndex)
		{
			CTextMasterStyle* pStyle = this->m_mapTextStyles.Lookup(lPlaceholderIndex);

			if (NULL != pStyle)
			{
				LONG lCountPF	= (LONG)pShape->m_oShape.m_oText.m_arPFs.GetCount();
				LONG lCountCF	= (LONG)pShape->m_oShape.m_oText.m_arCFs.GetCount();

				LONG lLevels	= (LONG)pStyle->m_arrLevels.GetCount();
				
				if (0 != lLevels)
				{
					for (LONG lIndexPF = 0; lIndexPF < lCountPF; ++lIndexPF)
					{
						LONG lIndentLevel = pTextSettings->m_arPFs[lIndexPF].lIndentLevel;

						if (lIndentLevel >= 0 && lIndentLevel < lLevels)
						{
							pTextSettings->m_arPFs[lIndexPF].ApplyBefore(pStyle->m_arrLevels[lIndentLevel].oPFRun);
						}
					}
					for (LONG lIndexCF = 0; lIndexCF < lCountCF; ++lIndexCF)
					{
						pTextSettings->m_arCFs[lIndexCF].ApplyBefore(pStyle->m_arrLevels[0].oCFRun);
					}
				}
			}
		}

		// ������ ����� ����������� ��� ��������� � ����������� �� ���� ������ (������ ��� ������!!!)
		if (stSlide == m_eSlideType)
		{
			pShape->m_oShape.m_oText.ApplyProperties(this);
		}

		if ((pShape->m_oActions.m_bPresent) && (7 < m_arColorScheme.GetSize()))
		{
			ApplyHyperlink(pShape, m_arColorScheme[6]);
		}

		CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShape->m_oShape.m_pShape);
		
		if (NULL != pPPTShape)
		{
			// ������ ������� - ��� �� ������ �������
			switch (pPPTShape->m_eType)
			{
			case sptTextPlainText:    
			case sptTextStop:  
			case sptTextTriangle:   
			case sptTextTriangleInverted:
			case sptTextChevron:
			case sptTextChevronInverted:
			case sptTextRingInside:
			case sptTextRingOutside:
			case sptTextArchUpCurve:   
			case sptTextArchDownCurve: 
			case sptTextCircleCurve: 
			case sptTextButtonCurve: 
			case sptTextArchUpPour:  
			case sptTextArchDownPour: 
			case sptTextCirclePour:
			case sptTextButtonPour:  
			case sptTextCurveUp:  
			case sptTextCurveDown: 
			case sptTextCascadeUp:   
			case sptTextCascadeDown:
			case sptTextWave1:   
			case sptTextWave2:   
			case sptTextWave3:   
			case sptTextWave4: 
			case sptTextInflate:   
			case sptTextDeflate:    
			case sptTextInflateBottom:  
			case sptTextDeflateBottom:  
			case sptTextInflateTop:
			case sptTextDeflateTop:   
			case sptTextDeflateInflate:   
			case sptTextDeflateInflateDeflate:
			case sptTextFadeRight: 
			case sptTextFadeLeft:   
			case sptTextFadeUp:   
			case sptTextFadeDown:   
			case sptTextSlantUp:    
			case sptTextSlantDown:   
			case sptTextCanUp:   
			case sptTextCanDown:
				{
					pShape->m_oShape.m_oText.m_arCFs.RemoveAll();
					pShape->m_oShape.m_oText.m_arPFs.RemoveAll();

					pShape->m_oShape.m_oText.m_oAttributes.m_oTextBrush = pShape->m_oShape.m_oBrush;

					pShape->m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal	= 1;
					pShape->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical		= 1;

					pShape->m_oShape.m_lDrawType = c_ShapeDrawType_Text;
					break;
				}
			default:
				break;
			};
		}
	}

private:

	void SetUpTextTypeStyle(CTextAttributesEx* pTextAttributes, NSOfficePPT::TextType eApplyType, LONG lApplyMode)
	{
		if ((NULL == pTextAttributes) || (NSOfficePPT::NoPresent == eApplyType) || (2 <= lApplyMode))
			return;

		CTextMasterStyle* pStyle = this->m_mapTextStyles.Lookup(eApplyType);
		if (NULL == pStyle)
			return;
		
		for (size_t nIndex = 0; nIndex < pTextAttributes->m_arPFs.GetCount(); ++nIndex)
		{
			LONG lIndentLevel = pTextAttributes->m_arPFs[nIndex].lIndentLevel;
			if (NULL != pStyle)
			{
				if (lIndentLevel < pStyle->m_lLevels)
				{
					switch (lApplyMode)
					{
					case 0:
						{
							pTextAttributes->m_arPFs[nIndex].ApplyBefore(pStyle->m_arrLevels[lIndentLevel].oPFRun);
							break;
						}
					case 1:
						{
							pTextAttributes->m_arPFs[nIndex].ApplyAfter(pStyle->m_arrLevels[lIndentLevel].oPFRun);
							break;
						}
					default:
						break;
					};
				}
			}
		}
		for (size_t nIndex = 0; nIndex < pTextAttributes->m_arCFs.GetCount(); ++nIndex)
		{
			if (NULL != pStyle)
			{
				LONG lIndentIndex = pTextAttributes->GetIndentLevelCF(nIndex);				
				if (lIndentIndex < pStyle->m_lLevels)
				{
					// ��� ��������� ����� ����� ����� �����. ����� ������� ����
					switch (lApplyMode)
					{
					case 0:
						{
							pTextAttributes->m_arCFs[nIndex].ApplyBefore(pStyle->m_arrLevels[lIndentIndex].oCFRun);
							break;
						}
					case 1:
						{
							pTextAttributes->m_arCFs[nIndex].ApplyAfter(pStyle->m_arrLevels[lIndentIndex].oCFRun);
							break;
						}
					default:
						break;
					};
				}
			}
		}
	}

	void SetUpAllTextType(CTextAttributesEx* pTextAttributes,	NSOfficePPT::TextType eTypeMaster,
																NSOfficePPT::TextType eTypePersist, BOOL bPersistPresent,
																NSOfficePPT::TextType eTypeOwn, BOOL bOwnPresent)
	{
		if (bOwnPresent)
		{		
			if (eTypeOwn != eTypePersist)
			{
				SetUpTextTypeStyle(pTextAttributes, eTypeOwn, 0);
			}
			if (eTypePersist != eTypeOwn)
			{
				SetUpTextTypeStyle(pTextAttributes, eTypePersist, 0);
			}
			if ((eTypeMaster != eTypePersist) && (eTypeMaster != eTypeOwn))
			{
				SetUpTextTypeStyle(pTextAttributes, eTypeMaster, 0);
			}
		}
		else if (bPersistPresent)
		{
			SetUpTextTypeStyle(pTextAttributes, eTypePersist, 0);

			if (eTypePersist != eTypeMaster)
			{
				SetUpTextTypeStyle(pTextAttributes, eTypeMaster, 0);
			}
			if (NSOfficePPT::NoPresent != eTypeOwn)
			{
				SetUpTextTypeStyle(pTextAttributes, eTypeOwn, 1);
			}
		}
		else
		{
			SetUpTextTypeStyle(pTextAttributes, eTypeMaster, 0);

			if (eTypePersist != eTypeMaster)
			{
				SetUpTextTypeStyle(pTextAttributes, eTypePersist, 1);
			}
			if (eTypeOwn != eTypePersist && ((NSOfficePPT::NoPresent != eTypePersist) || (eTypeMaster != eTypeOwn)))
			{
				SetUpTextTypeStyle(pTextAttributes, eTypeOwn, 1);
			}
		}		
	}

	void ApplyHyperlink(CShapeElement* pShape, SColorAtom& oColor)
	{
		CAtlArray<CInteractiveInfo::CTextRange>* pRanges	= &pShape->m_oActions.m_arRanges;
		CTextAttributesEx* pTextAttributes					= &pShape->m_oShape.m_oText;

		LONG lCountHyper	= (LONG)pRanges->GetCount();
		LONG lCountCFs		= (LONG)pTextAttributes->m_arCFs.GetCount();
		
		if (0 == lCountHyper)
			return;

		CAtlArray<LONG> arStart;
		CAtlArray<LONG> arEnd;

		LONG lStartCur	= 0;
		LONG lEndCur	= 0;

		for (LONG iCF = 0; iCF < lCountCFs; ++iCF)
		{
			lEndCur = lStartCur + pTextAttributes->m_arCFs[iCF].lCount - 1;
			
			arStart.Add(lStartCur);
			arEnd.Add(lEndCur);

			lStartCur = lEndCur + 1;
		}

		// ������� ����������� Ranges
		for (LONG iR = 0; iR < lCountHyper; ++iR)
		{
			LONG lStart = ((*pRanges)[iR]).m_lStart;
			LONG lEnd	= ((*pRanges)[iR]).m_lEnd;

			if (lStart > lEnd)
				continue;

			LONG lStartFound	= -1;
			LONG lEndFound		= -1;

			lCountCFs = (LONG)pTextAttributes->m_arCFs.GetCount();

			for (LONG iCF = 0; iCF < lCountCFs; ++iCF)
			{
				BOOL bStart = ((lStart >= arStart[iCF]) && (lStart <= arEnd[iCF]));
				BOOL bEnd	= ((lEnd   >= arStart[iCF]) && (lEnd   <= arEnd[iCF]));

				if (-1 == lStartFound && bStart)
					lStartFound = iCF;

				if (-1 == lEndFound   && bEnd)
					lEndFound = iCF;
			}

			// ������� ������� �����, � ������� ���� ������
			if (-1 == lStartFound || -1 == lEndFound)
				continue;
			
			if (lStartFound == lEndFound)
			{
				InsertCF(lStart, lEnd, pTextAttributes->m_arCFs, arStart, arEnd, lStartFound, oColor);
			}
			else if (lEndFound > lStartFound)
			{
				// �� ���� ������� ���� �� �����

				// ����� ���������������� ����� �������
				InsertCF(lStart, arEnd[lStartFound], pTextAttributes->m_arCFs, arStart, arEnd, lStartFound, oColor);

				LONG lPlus = (LONG)pTextAttributes->m_arCFs.GetCount() - lCountCFs;
				lCountCFs += lPlus;

				InsertCF(arStart[lEndFound + lPlus], lEnd, pTextAttributes->m_arCFs, arStart, arEnd, lEndFound + lPlus, oColor);
			}
		}

		// ������ ������...
		/*for (size_t i = 0; i < pTextAttributes->m_arCFs.GetCount(); ++i)
		{
			pTextAttributes->m_arCFs[i].hasColor	= true;
			pTextAttributes->m_arCFs[i].oColor		= oColor;
		}*/
	}

	void InsertCF(LONG lStartH, LONG lEndH, CAtlArray<STextCFRun>& oArrayCF, CAtlArray<LONG>& arStart, CAtlArray<LONG>&arEnd, LONG lIndexFound, SColorAtom& oColor)
	{
		STextCFRun oRunOld = oArrayCF[lIndexFound];
		STextCFRun oRunNew = oRunOld;

		oRunNew.oColor = oColor;
		oRunNew.fontStyle |= 0x04;
		oRunNew.hasUnderline = true;

		LONG lSize1 = lStartH - arStart[lIndexFound];
		LONG lSize2 = lEndH - lStartH + 1;
		LONG lSize3 = arEnd[lIndexFound] - lEndH;

		if (lSize1 > 0 && lSize3 > 0)
		{
			oArrayCF[lIndexFound].lCount = lSize1;
			arEnd[lIndexFound] = arStart[lIndexFound] + lSize1 - 1;

			oRunNew.lCount = lSize2;
			oArrayCF.InsertAt(lIndexFound + 1, oRunNew);
			arStart.InsertAt(lIndexFound + 1, arEnd[lIndexFound] + 1);
			arEnd.InsertAt(lIndexFound + 1, arStart[lIndexFound + 1] + lSize2 - 1);

			oRunOld.lCount = lSize3;
			oArrayCF.InsertAt(lIndexFound + 2, oRunOld);
			arStart.InsertAt(lIndexFound + 2, arEnd[lIndexFound + 1] + 1);
			arEnd.InsertAt(lIndexFound + 2, arStart[lIndexFound + 2] + lSize3 - 1);
		}
		else if (lSize1 > 0)
		{
			oArrayCF[lIndexFound].lCount = lSize1;
			arEnd[lIndexFound] = arStart[lIndexFound] + lSize1 - 1;

			oRunNew.lCount = lSize2;
			oArrayCF.InsertAt(lIndexFound + 1, oRunNew);
			arStart.InsertAt(lIndexFound + 1, arEnd[lIndexFound] + 1);
			arEnd.InsertAt(lIndexFound + 1, arStart[lIndexFound + 1] + lSize2 - 1);
		}
		else if (lSize3 > 0)
		{
			oArrayCF[lIndexFound] = oRunNew;
			oArrayCF[lIndexFound].lCount = lSize2;
			arEnd[lIndexFound] = arStart[lIndexFound] + lSize2 - 1;

			oRunOld.lCount = lSize3;
			oArrayCF.InsertAt(lIndexFound + 1, oRunOld);
			arStart.InsertAt(lIndexFound + 1, arEnd[lIndexFound] + 1);
			arEnd.InsertAt(lIndexFound + 1, arStart[lIndexFound + 1] + lSize3 - 1);
		}
		else
		{
			oArrayCF[lIndexFound] = oRunNew;
		}
	}
};