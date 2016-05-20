﻿#include "SectionProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// SectPrChange 
		//--------------------------------------------------------------------------------	

		CSectPrChange::CSectPrChange()
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oNode );
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oReader );
		}
		CSectPrChange::~CSectPrChange()
		{
		}
		void CSectPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_sectPr;

			if ( m_pSecPr.IsInit() && oNode.GetNode( _T("w:sectPr"), oNode_sectPr ) )
				m_pSecPr->fromXML( oNode_sectPr );

		}
		void CSectPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( _T("w:sectPr") == sName )
					m_pSecPr->fromXML( oReader );
			}
		}
		CString CSectPrChange::toXML() const
		{			
			CString sResult = _T("<w:sectPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += _T("w:author=\"");
				sResult += m_sAuthor->GetString();
				sResult += _T("\" ");
			}

			if ( m_oDate.IsInit() )
			{
				sResult += _T("w:date=\"");
				sResult += m_oDate->ToString();
				sResult += _T("\" ");
			}

			if ( m_oId.IsInit() )
			{
				sResult += _T("w:id=\"");
				sResult += m_oId->ToString();
				sResult += _T("\" ");
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += _T("oouserid=\"");
				sResult += m_sUserId->GetString();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_pSecPr.IsInit() )
				sResult += m_pSecPr->toXML();

			sResult += _T("</w:sectPrChange>");

			return sResult;
		}
		EElementType CSectPrChange::getType() const
		{
			return et_w_sectPrChange;
		}
		void CSectPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
	} // Logic
} // ComplexTypes
