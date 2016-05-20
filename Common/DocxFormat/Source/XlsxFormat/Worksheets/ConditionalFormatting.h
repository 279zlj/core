#pragma once
#ifndef OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_
#define OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//������������:
		//<extLst>
		class CConditionalFormatValueObject : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConditionalFormatValueObject)
			CConditionalFormatValueObject()
			{
			}
			virtual ~CConditionalFormatValueObject()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (m_oType.IsInit())
				{
					CString sValue;
					CString sRoot;
                    sRoot.Format(_T("<cfvo type=\"%ls\""), (const TCHAR *) m_oType.get().ToString());
					writer.WriteString(sRoot);
					if (m_oGte.IsInit() && false == m_oGte->ToBool())
						writer.WriteString(_T (" gte=\"0\""));
					if (m_oVal.IsInit())
					{
						sValue.Format(_T(" val=\"%ls\""), m_oVal.get());
						writer.WriteString(sValue);
					}
					

					writer.WriteString(_T("/>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
				
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_ConditionalFormatValueObject;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("gte")		, m_oGte)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("type")	, m_oType)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("val")		, m_oVal)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::COnOff<>>						m_oGte;
			nullable<SimpleTypes::Spreadsheet::ST_CfvoType<>>	m_oType;	
			nullable<CString>									m_oVal;
		};

		class CColorScale : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CColorScale)
			CColorScale()
			{
			}
			virtual ~CColorScale()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (3 < m_arrItems.size()) // min 2 + 2
				{
					CString sValue;
					writer.WriteString(_T("<colorScale>"));

					for (unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(_T("</colorScale>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CWCharWrapper sName = oReader.GetName();
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
					else if (_T("color") == sName)
						m_arrItems.push_back(new CColor(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_ColorScale;
			}

		public:
		};

		class CDataBar : public WritingElementWithChilds<CConditionalFormatValueObject>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CDataBar)
			CDataBar()
			{
			}
			virtual ~CDataBar()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (2 == m_arrItems.size() && m_oColor.IsInit())
				{
					CString sValue;
					writer.WriteString(_T("<dataBar"));
					if (m_oMaxLength.IsInit())
					{
						sValue.Format(_T(" maxLength=\"%d\""), m_oMaxLength->GetValue());
						writer.WriteString(sValue);
					}
					if (m_oMaxLength.IsInit())
					{
						sValue.Format(_T(" maxLength=\"%d\""), m_oMaxLength->GetValue());
						writer.WriteString(sValue);
					}
					if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
					{
						writer.WriteString(_T(" showValue=\"0\""));
					}

					writer.WriteString(_T(">"));

					for (unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					m_oColor->toXML2(writer, _T("color"));

					writer.WriteString(_T("</dataBar>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CWCharWrapper sName = oReader.GetName();
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
					else if (_T("color") == sName)
						m_oColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_DataBar;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("maxLength")	, m_oMaxLength)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("minLength")	, m_oMinLength)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("showValue")	, m_oShowValue)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			nullable<SimpleTypes::COnOff<>>					m_oShowValue;

			nullable<CColor>								m_oColor;
		};

		class CFormulaCF : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFormulaCF)
			CFormulaCF()
			{
			}
			virtual ~CFormulaCF()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<formula>"));
				writer.WriteString(XmlUtils::EncodeXmlString(m_sText));
				writer.WriteString(_T("</formula>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				m_sText = oReader.GetText2();
			}

			virtual EElementType getType () const
			{
				return et_FormulaCF;
			}

		public:
			CString m_sText;
		};

		class CIconSet : public WritingElementWithChilds<CConditionalFormatValueObject>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CIconSet)
			CIconSet()
			{
			}
			virtual ~CIconSet()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (1 < m_arrItems.size()) // min value = 2
				{
					CString sValue;
					writer.WriteString(_T("<iconSet"));
					if (m_oIconSet.IsInit())
					{
                        sValue.Format(_T(" iconSet=\"%ls\""), (const TCHAR *) m_oIconSet.get().ToString());
						writer.WriteString(sValue);
					}
					if (m_oPercent.IsInit() && false == m_oPercent->ToBool())
					{
						writer.WriteString(_T(" percent=\"0\""));
					}
					if (m_oReverse.IsInit() && true == m_oReverse->ToBool())
					{
						writer.WriteString(_T(" reverse=\"1\""));
					}
					if (m_oShowValue.IsInit() && false == m_oShowValue->ToBool())
					{
						writer.WriteString(_T(" showValue=\"0\""));
					}

					writer.WriteString(_T(">"));

					for (unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(_T("</iconSet>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CWCharWrapper sName = oReader.GetName();
					if (_T("cfvo") == sName)
						m_arrItems.push_back(new CConditionalFormatValueObject(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_IconSet;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("iconSet")		, m_oIconSet)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("percent")		, m_oPercent)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("reverse")		, m_oReverse)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("showValue")	, m_oShowValue)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::Spreadsheet::ST_IconSetType<>>m_oIconSet;
			nullable<SimpleTypes::COnOff<>>						m_oPercent;
			nullable<SimpleTypes::COnOff<>>						m_oReverse;
			nullable<SimpleTypes::COnOff<>>						m_oShowValue;
		};

		//������������:
		//<extLst>
		class CConditionalFormattingRule : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConditionalFormattingRule)
			CConditionalFormattingRule()
			{
			}
			virtual ~CConditionalFormattingRule()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (m_oType.IsInit() && m_oPriority.IsInit() && 0 < m_arrItems.size())
				{
					CString sValue;
					CString sRoot;
                    sRoot.Format(_T("<cfRule type=\"%ls\" priority=\"%d\""), (const TCHAR *) m_oType.get().ToString(), m_oPriority->GetValue());
					writer.WriteString(sRoot);
					if (m_oAboveAverage.IsInit() && false == m_oAboveAverage->ToBool())
						writer.WriteString(_T (" aboveAverage=\"0\""));
					if (m_oBottom.IsInit() && true == m_oBottom->ToBool())
						writer.WriteString(_T (" bottom=\"1\""));
					if (m_oDxfId.IsInit())
					{
						sValue.Format(_T(" dxfId=\"%d\""), m_oDxfId->GetValue());
						writer.WriteString(sValue);
					}
					if (m_oEqualAverage.IsInit() && true == m_oEqualAverage->ToBool())
						writer.WriteString(_T (" equalAverage=\"1\""));
					if (m_oOperator.IsInit())
					{
                        sValue.Format(_T(" text=\"%ls\""), (const TCHAR *) m_oOperator.get().ToString());
						writer.WriteString(sValue);
					}
					if (m_oPercent.IsInit() && true == m_oPercent->ToBool())
						writer.WriteString(_T (" percent=\"1\""));
					if (m_oRank.IsInit())
					{
						sValue.Format(_T(" rank=\"%d\""), m_oRank->GetValue());
						writer.WriteString(sValue);
					}
					if (m_oStdDev.IsInit())
					{
						sValue.Format(_T(" stdDev=\"%d\""), m_oStdDev->GetValue());
						writer.WriteString(sValue);
					}
					if (m_oStopIfTrue.IsInit() && true == m_oStopIfTrue->ToBool())
						writer.WriteString(_T (" stopIfTrue=\"1\""));
					if (m_oText.IsInit())
					{
						sValue.Format(_T(" text=\"%ls\""), m_oText.get());
						writer.WriteString(sValue);
					}
					if (m_oTimePeriod.IsInit())
					{
						sValue.Format(_T(" timePeriod=\"%ls\""), m_oTimePeriod.get());
						writer.WriteString(sValue);
					}

					writer.WriteString(_T(">"));

					for (unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(_T("</cfRule>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CWCharWrapper sName = oReader.GetName();
					if (_T("colorScale") == sName)
						m_arrItems.push_back(new CColorScale(oReader));
					else if (_T("dataBar") == sName)
						m_arrItems.push_back(new CDataBar(oReader));
					else if (_T("formula") == sName)
						m_arrItems.push_back(new CFormulaCF(oReader));
					else if (_T("iconSet") == sName)
						m_arrItems.push_back(new CIconSet(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_ConditionalFormattingRule;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("aboveAverage")	, m_oAboveAverage)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("bottom")			, m_oBottom)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("dxfId")			, m_oDxfId)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("equalAverage")	, m_oEqualAverage)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("operator")		, m_oOperator)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("percent")			, m_oPercent)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("priority")		, m_oPriority)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("rank")			, m_oRank)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("stdDev")			, m_oStdDev)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("stopIfTrue")		, m_oStopIfTrue)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("text")			, m_oText)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("timePeriod")		, m_oTimePeriod)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("type")			, m_oType)

				WritingElement_ReadAttributes_End(oReader)
			}

		public:
			nullable<SimpleTypes::COnOff<>>						m_oAboveAverage;
			nullable<SimpleTypes::COnOff<>>						m_oBottom;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oDxfId;
			nullable<SimpleTypes::COnOff<>>						m_oEqualAverage;
			nullable<SimpleTypes::Spreadsheet::ST_CfOperator<>>	m_oOperator;	
			nullable<SimpleTypes::COnOff<>>						m_oPercent;
			nullable<SimpleTypes::CDecimalNumber<>>				m_oPriority;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRank;
			nullable<SimpleTypes::CDecimalNumber<>>				m_oStdDev;
			nullable<SimpleTypes::COnOff<>>						m_oStopIfTrue;
			nullable<CString>									m_oText;
			nullable<CString>									m_oTimePeriod;	// ToDo ���������� �� ��� ST_TimePeriod (18.18.82)
			nullable<SimpleTypes::Spreadsheet::ST_CfType<>>		m_oType;	
		};

		//������������:
		//<extLst>
		class CConditionalFormatting  : public WritingElementWithChilds<CConditionalFormattingRule>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConditionalFormatting)
			CConditionalFormatting()
			{
			}
			virtual ~CConditionalFormatting()
			{
			}

		public:
			virtual CString toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if (m_oSqRef.IsInit() && 0 < m_arrItems.size())
				{
					CString sRoot;
					sRoot.Format(_T("<conditionalFormatting sqref=\"%ls\""), m_oSqRef->GetValue());
					writer.WriteString(sRoot);

					if (m_oPivot.IsInit() && true == m_oPivot->ToBool())
					{
						writer.WriteString(_T (" pivot=\"1\""));
					}

					writer.WriteString(_T(">"));

					for (unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);

					writer.WriteString(_T("</conditionalFormatting>"));
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					CWCharWrapper sName = oReader.GetName();

					if (_T("cfRule") == sName)
						m_arrItems.push_back(new CConditionalFormattingRule(oReader));
				}
			}

			virtual EElementType getType () const
			{
				return et_ConditionalFormatting;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start(oReader)

				WritingElement_ReadAttributes_Read_if		(oReader, _T("sqref")	, m_oSqRef)
				WritingElement_ReadAttributes_Read_else_if	(oReader, _T("pivot")	, m_oPivot)

				WritingElement_ReadAttributes_End(oReader)
			}
		public:
			nullable<SimpleTypes::COnOff<>>			m_oPivot;
			nullable<SimpleTypes::CRelationshipId >	m_oSqRef; // ToDo ���������� �� ��� "sqref" (18.18.76) - ������������������ "ref", ����������� ��������
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CONDITIONALFORMATTING_FILE_INCLUDE_H_
