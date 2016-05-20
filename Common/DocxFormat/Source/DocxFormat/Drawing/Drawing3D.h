#pragma once
#ifndef OOX_LOGIC_DRAWING_3D_INCLUDE_H_
#define OOX_LOGIC_DRAWING_3D_INCLUDE_H_

#include "DrawingExt.h"
#include "DrawingColors.h"

namespace OOX
{
	namespace Drawing
	{
		enum EText3DType
		{
			text3dtypeUnknown = 0,
			text3dtypeShape   = 1,
			text3dtypeFlat    = 2,
		};
		//--------------------------------------------------------------------------------
		// CPoint3D 20.1.5.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPoint3D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPoint3D)
			CPoint3D()
			{
			}
			virtual ~CPoint3D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CPoint3D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:anchor x=\"") + m_oX.ToString()
									   + _T("\" y=\"") + m_oY.ToString()
									   + _T("\" z=\"") + m_oZ.ToString()
									   + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_anchor;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("x"), m_oX )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("y"), m_oY )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("z"), m_oZ )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Childs
			SimpleTypes::CCoordinate m_oX;
			SimpleTypes::CCoordinate m_oY;
			SimpleTypes::CCoordinate m_oZ;

		};
		//--------------------------------------------------------------------------------
		// CVector3D 20.1.5.10;20.1.5.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CVector3D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CVector3D)
			CVector3D()
			{
				m_eType = et_Unknown;
			}
			virtual ~CVector3D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: ����������� CVector3D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();

				if ( _T("a:norm") == sName )
					m_eType = et_a_norm;
				else if ( _T("a:up") == sName )
					m_eType = et_a_up;
				else
					return;

				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( et_a_norm == m_eType )
					sResult = _T("<a:norm dx=\"") + m_oDx.ToString()
					             + _T("\" dy=\"") + m_oDy.ToString()
					             + _T("\" dz=\"") + m_oDz.ToString()
					             + _T("\"/>");
				if ( et_a_up == m_eType )
					sResult = _T("<a:up dx=\"") + m_oDx.ToString()
					           + _T("\" dy=\"") + m_oDy.ToString()
					           + _T("\" dz=\"") + m_oDz.ToString()
					           + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("dx"), m_oDx )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dy"), m_oDy )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("dz"), m_oDz )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType             m_eType;

			// Childs
			SimpleTypes::CCoordinate m_oDx;
			SimpleTypes::CCoordinate m_oDy;
			SimpleTypes::CCoordinate m_oDz;

		};
		//--------------------------------------------------------------------------------
		// CBackdrop 20.1.5.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBackdrop : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBackdrop)
			CBackdrop()
			{
			}
			virtual ~CBackdrop()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CBackdrop::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( !oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:anchor") == sName )
						m_oAnchor = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:norm") == sName )
						m_oNorm = oReader;
					else if ( _T("a:up") == sName )
						m_oUp = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:backdrop>");
				
				sResult += m_oAnchor.toXML();
				sResult += m_oNorm.toXML();
				sResult += m_oUp.toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:backdrop>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_backdrop;
			}

		public:

			// Childs
			OOX::Drawing::CPoint3D                          m_oAnchor;
			OOX::Drawing::CVector3D                         m_oNorm;
			OOX::Drawing::CVector3D                         m_oUp;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;

		};
		//--------------------------------------------------------------------------------
		// CBevel 20.1.4.2.5;20.1.5.3;20.1.5.4 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBevel : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBevel)
			CBevel()
			{
				m_eType = et_Unknown;
			}
			virtual ~CBevel()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: ����������� CBevel::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();

				if ( _T("a:bevel") == sName )
					m_eType = et_a_bevel;
				if ( _T("a:bevelB") == sName )
					m_eType = et_a_bevelB;
				else if ( _T("a:bevelT") == sName )
					m_eType = et_a_bevelT;
				else
					return;

				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( et_a_bevel == m_eType )
					sResult = _T("<a:bevel h=\"")    + m_oH.ToString()
					             + _T("\"  prst=\"") + m_oPrst.ToString()
					             + _T("\"  w=\"")    + m_oW.ToString()
					             + _T("\"/>");
				else if ( et_a_bevelB == m_eType )
					sResult = _T("<a:bevelB h=\"")    + m_oH.ToString()
					             + _T("\"  prst=\"") + m_oPrst.ToString()
					             + _T("\"  w=\"")    + m_oW.ToString()
					             + _T("\"/>");
				else if ( et_a_bevelT == m_eType )
					sResult = _T("<a:bevelT h=\"")    + m_oH.ToString()
					             + _T("\"  prst=\"") + m_oPrst.ToString()
					             + _T("\"  w=\"")    + m_oW.ToString()
					             + _T("\"/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("h"),    m_oH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("prst"), m_oPrst )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w"),    m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			EElementType                                                      m_eType;

			// Childs
			SimpleTypes::CPositiveCoordinate<76200>                           m_oH;
			SimpleTypes::CBevelPresetType<SimpleTypes::bevelpresettypeCircle> m_oPrst;
			SimpleTypes::CPositiveCoordinate<76200>                           m_oW;

		};
		//--------------------------------------------------------------------------------
		// CSphereCoords 20.1.5.11 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSphereCoords : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSphereCoords)
			CSphereCoords()
			{
			}
			virtual ~CSphereCoords()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CSphereCoords::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:rot lat=\"") + m_oLat.ToString()
									+ _T("\" lon=\"") + m_oLon.ToString()
									+ _T("\" rev=\"") + m_oRev.ToString()
									+ _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_rot;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("lat"), m_oLat )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("lon"), m_oLon )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rev"), m_oRev )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Childs
			SimpleTypes::CPositiveFixedAngle<> m_oLat;
			SimpleTypes::CPositiveFixedAngle<> m_oLon;
			SimpleTypes::CPositiveFixedAngle<> m_oRev;

		};
		//--------------------------------------------------------------------------------
		// CCamera 20.1.5.5 (Part 1)
		//--------------------------------------------------------------------------------	
		class CCamera : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCamera)
			CCamera()
			{
			}
			virtual ~CCamera()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CCamera::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:rot") == sName )
						m_oRot = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:camera prst=\"") + m_oPrst.ToString() + _T("\" zoom=\"") + m_oZoom.ToString();

				if ( m_oFov.IsInit() )
				{
					sResult += _T("\" fov=\"");
					sResult += m_oFov->ToString();
					sResult += _T("\">");
				}
				else
					sResult += _T("\">");

				if ( m_oRot.IsInit() )
					sResult += m_oRot->toXML();

				sResult += _T("</a:camera>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_camera;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ���������� �������� �� ���������
				m_oZoom.SetValue( 100 );

				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("fov"),  m_oFov )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("prst"), m_oPrst )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("zoom"), m_oZoom )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CFOVAngle<>>    m_oFov;
			SimpleTypes::CPresetCameraType<>      m_oPrst;
			SimpleTypes::CPositivePercentage      m_oZoom;

			// Childs
			nullable<OOX::Drawing::CSphereCoords> m_oRot;

		};
        //-----------------------------------------------------------------------
        // ContourColor 20.1.5.6
        //-----------------------------------------------------------------------				
		class CContourColor : public OOX::Drawing::CColor
		{
		public:
			WritingElement_AdditionConstructors(CContourColor)
			CContourColor()
			{
			}
			virtual ~CContourColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				CColor::fromXML( oNode );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				CColor::fromXML( oReader );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:contourClr>");

				sResult += CColor::toXML();

				sResult += _T("</a:contourClr>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_contourClr;
			}

		};

        //-----------------------------------------------------------------------
        // ExtrusionColor 20.1.5.7
        //-----------------------------------------------------------------------				
		class CExtrusionColor : public OOX::Drawing::CColor
		{
		public:
			WritingElement_AdditionConstructors(CExtrusionColor)
			CExtrusionColor()
			{
			}
			virtual ~CExtrusionColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				CColor::fromXML( oNode );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				CColor::fromXML( oReader );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:extrusionClr>");

				sResult += CColor::toXML();

				sResult += _T("</a:extrusionClr>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_extrusionClr;
			}

		};

		//--------------------------------------------------------------------------------
		// CFlatText 20.1.5.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFlatText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFlatText)
			CFlatText()
			{
			}
			virtual ~CFlatText()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CFlatText::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:flatTx z=\"") + m_oZ.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_flatTx;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ��������� �������� �� ���������
				m_oZ.SetValue( 0 );

				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("z"), m_oZ )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Childs
			SimpleTypes::CCoordinate m_oZ;

		};
		//--------------------------------------------------------------------------------
		// CLightRig 20.1.5.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CLightRig : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLightRig)
			CLightRig()
			{
			}
			virtual ~CLightRig()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CLightRig::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:rot") == sName )
						m_oRot = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:lightRig dir=\"") + m_oDir.ToString() + _T("\" rig=\"") + m_oRig.ToString() + _T("\">");

				if ( m_oRot.IsInit() )
					sResult += m_oRot->toXML();

				sResult += _T("</a:lightRig>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_lightRig;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("dir"), m_oDir )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rig"), m_oRig )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CLightRigDirection<>     m_oDir;
			SimpleTypes::CLightRigType<>          m_oRig;

			// Childs
			nullable<OOX::Drawing::CSphereCoords> m_oRot;

		};
		//--------------------------------------------------------------------------------
		// CShape3D 20.1.5.12 (Part 1)
		//--------------------------------------------------------------------------------	
		class CShape3D : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShape3D)
			CShape3D()
			{
			}
			virtual ~CShape3D()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: ����������� CShape3D::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();

					if ( _T("a:bevelB") == sName )
						m_oBevelB = oReader;
					else if ( _T("a:bevelT") == sName )
						m_oBevelT = oReader;
					else if ( _T("a:contourClr") == sName )
						m_oContourClr = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:extrusionClr") == sName )
						m_oExtrusionClr = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:sp3d contourW=\"")     + m_oContourW.ToString() 
					                 + _T("\" extrusionH=\"")   + m_oExtrusionH.ToString() 
					                 + _T("\" prstMaterial=\"") + m_oPrstMaterial.ToString() 
					                 + _T("\" z=\"")            + m_oZ.ToString() 
									 + _T("\">");

				if ( m_oBevelT.IsInit() )
					sResult += m_oBevelT->toXML();

				if ( m_oBevelB.IsInit() )
					sResult += m_oBevelB->toXML();

				if ( m_oContourClr.IsInit() )
					sResult += m_oContourClr->toXML();

				if ( m_oExtrusionClr.IsInit() )
					sResult += m_oExtrusionClr->toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:sp3d>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_sp3d;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ���������� �������� �� ���������
				m_oZ.SetValue( 0.0 );

				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("contourW"),     m_oContourW )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("extrusionH"),   m_oExtrusionH )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("prstMaterial"), m_oPrstMaterial )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("z"),            m_oZ )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CPositiveCoordinate<0>                                        m_oContourW;
			SimpleTypes::CPositiveCoordinate<0>                                        m_oExtrusionH;
			SimpleTypes::CPresetMaterialType<SimpleTypes::presetmaterialtypeWarmMatte> m_oPrstMaterial;
			SimpleTypes::CCoordinate                                                   m_oZ;

			// Childs
			nullable<OOX::Drawing::CBevel>                  m_oBevelB;
			nullable<OOX::Drawing::CBevel>                  m_oBevelT;
			nullable<OOX::Drawing::CContourColor>           m_oContourClr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CExtrusionColor>         m_oExtrusionClr;

		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_3D_INCLUDE_H_
