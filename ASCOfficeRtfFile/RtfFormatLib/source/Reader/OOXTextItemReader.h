﻿#pragma once
#include "../RtfDocument.h"
#include "../RtfTable.h"

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"

class OOXTextItemReader
{
private:
//	OOXParagraphReader	m_oParagraphReader; ///?????
//	OOXTableReader		m_oTableReader;

public: 
	TextItemContainerPtr m_oTextItems;

	OOXTextItemReader( )
	{
		m_oTextItems = TextItemContainerPtr( new TextItemContainer() );
	}
	bool Parse( OOX::WritingElement* ooxElement, ReaderParameter oParam )
	{
		switch(ooxElement->getType())
		{
			case OOX::et_w_p:
			{
				OOX::Logic::CParagraph * pParagraph = dynamic_cast<OOX::Logic::CParagraph*>(ooxElement);
				
				OOXParagraphReader	m_oParagraphReader(pParagraph);
				RtfParagraphPtr oNewParagraph( new RtfParagraph() );
				//применяем к новому параграфу default property
				oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
				oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
				oNewParagraph->m_oProperty.m_nItap = 0;

				if( true == m_oParagraphReader.Parse( oParam, (*oNewParagraph), CcnfStyle() ))
				{
					m_oTextItems->AddItem( oNewParagraph );
					if( true == oParam.oRtf->m_oStatusSection.start_new )
					{
						RtfSectionPtr oCurSection;
						if( true == oParam.oRtf->GetItem( oCurSection, oParam.oRtf->m_oStatusSection.number - 1) )
						{
							m_oTextItems = oCurSection;
						}
						oParam.oRtf->m_oStatusSection.start_new = false;
					}
				}
			}break;
			case OOX::et_w_tbl:
			{
				OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(ooxElement);
				RtfTablePtr oNewTable( new RtfTable() );
				
				OOXTableReader		oTableReader(pTbl);
				oParam.oReader->m_nCurItap = 1;
				if( true == oTableReader.Parse( oParam, (*oNewTable)) )
				{
					m_oTextItems->AddItem( oNewTable );
				}
				oParam.oReader->m_nCurItap = 0;
			}break;
			case OOX::et_w_sdt:
			{
				OOX::Logic::CSdt * pSdt = dynamic_cast<OOX::Logic::CSdt*>(ooxElement);
				if( pSdt->m_oSdtEndPr.IsInit())
				{
					//todo
				}
				if( pSdt->m_oSdtContent.IsInit() )
				{
					Parse( pSdt->m_oSdtContent.GetPointer(), oParam );
				}
			}break;
		}
		return true;
	}
};