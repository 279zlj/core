#pragma once
#include "PathShape.h"
#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Base.h"


class CHandle_
{
public:
	std::wstring position;
	std::wstring xrange;
	std::wstring yrange;
	std::wstring switchHandle;
	std::wstring polar;
	std::wstring radiusrange; 

public:
	CHandle_()
	{
		position = _T("");
		xrange = _T("");
		yrange = _T("");
		switchHandle = _T("");
		polar = _T("");
		radiusrange = _T(""); 
	}
	CHandle_& operator =(const CHandle_& oSrc)
	{
		position		= oSrc.position;
		xrange			= oSrc.xrange;
		yrange			= oSrc.yrange;
		switchHandle	= oSrc.switchHandle;
		polar			= oSrc.polar;
		radiusrange		= oSrc.radiusrange;
		return (*this);
	}
};
//
namespace NSGuidesVML
{
class CBaseShape
{
public:
	std::vector<long>		m_arAdjustments;
	std::vector<double>		m_arGuides;

	LONG					m_eJoin;
	bool					m_bConcentricFill;

    std::vector<SimpleTypes::CPoint>	m_arConnectors;
	std::vector<LONG>					m_arConnectorAngles;

    std::vector<Aggplus::RECT>			m_arTextRects;
	
	std::vector<CHandle_>				m_arHandles;

	std::wstring						m_strTransformXml;

	std::wstring						m_strPath;
	std::wstring						m_strRect;

	LONG								m_lLimoX;
	LONG								m_lLimoY;
	
	CPath								m_oPath;
public:
	CBaseShape()
	{
	}

	//virtual bool LoadFromXML(const std::wstring& xml)			= 0;
	//virtual bool LoadFromXML(XmlUtils::CXmlNode& xmlNode)		= 0;
	//virtual bool LoadAdjustValuesList(const std::wstring& xml)	= 0;
	//virtual bool LoadGuidesList(const std::wstring& xml)		= 0;
	//virtual bool LoadAdjustHandlesList(const std::wstring& xml)	= 0;
	//virtual bool LoadConnectorsList(const std::wstring& xml)	= 0;
	virtual bool LoadTextRect(const std::wstring& xml)			= 0;
	virtual bool LoadPathList(const std::wstring& xml)			= 0;
	//virtual bool SetAdjustment(long index, long value)			= 0;
	//virtual void ReCalculate()									= 0;


	//virtual void AddGuide(const std::wstring& strGuide)	{}

	static CBaseShape* CreateByType(int ShapeType);
	
	bool SetType(int ShapeType);

	//virtual bool SetProperties(CBaseShape* Shape)
	//{
	//	if( Shape == NULL)
	//		return false;

	//	m_oPath		= Shape->m_oPath;
	//	m_strPath	= Shape->m_strPath;
	//	m_strRect	= Shape->m_strRect;
	//	
	//	m_arAdjustments.clear();
	//	for(int i = 0; i < Shape->m_arAdjustments.size(); i++)
	//		m_arAdjustments.push_back(Shape->m_arAdjustments[i]);

	//	Guides.clear();
	//	for(int i = 0; i < Shape->Guides.size(); i++)
	//		Guides.push_back(Shape->Guides[i]);

	//	m_eJoin				= Shape->m_eJoin;
	//	m_bConcentricFill	= Shape->m_bConcentricFill;

	//	m_arConnectors.clear();
	//	for(int i = 0; i < Shape->m_arConnectors.size(); i++)
	//		m_arConnectors.push_back(Shape->m_arConnectors[i]);

	//	m_arConnectorAngles.clear();
	//	for(int i = 0; i < Shape->m_arConnectorAngles.size(); i++)
	//		m_arConnectorAngles.push_back(Shape->m_arConnectorAngles[i]);

	//	m_arTextRects.clear();
	//	for(int i = 0; i < Shape->m_arTextRects.size(); i++)
	//		m_arTextRects.push_back(Shape->m_arTextRects[i]);

	//	m_arHandles.clear();
	//	for(int i = 0; i < Shape->m_arHandles.size(); i++)
	//		m_arHandles.push_back(Shape->m_arHandles[i]);


	//	m_strRect = Shape->m_strRect;
	//	m_strTransformXml = Shape->m_strTransformXml;
	//	return true;
	//}

	//virtual bool SetToDublicate(CBaseShape* Shape)
	//{
	//	if( Shape == NULL)
	//		return false;

	//	Shape->m_oPath		= m_oPath;
	//	Shape->m_strPath	= m_strPath;
	//	Shape->m_strRect	= m_strRect;
	//	
	//	Shape->m_arAdjustments.clear();
	//	for(int i = 0; i < m_arAdjustments.size(); i++)
	//		Shape->m_arAdjustments.push_back(m_arAdjustments[i]);

	//	Shape->Guides.clear();
	//	for(int i = 0; i < Guides.size(); i++)
	//		Shape->Guides.push_back(Guides[i]);

	//	Shape->m_eJoin				= m_eJoin;
	//	Shape->m_bConcentricFill	= m_bConcentricFill;

	//	Shape->m_arConnectors.clear();
	//	for(int i = 0; i < m_arConnectors.size(); i++)
	//		Shape->m_arConnectors.push_back(m_arConnectors[i]);

	//	Shape->m_arConnectorAngles.clear();
	//	for(int i = 0; i < m_arConnectorAngles.size(); i++)
	//		Shape->m_arConnectorAngles.push_back(m_arConnectorAngles[i]);

	//	Shape->m_arTextRects.clear();
	//	for(int i = 0; i < m_arTextRects.size(); i++)
	//		Shape->m_arTextRects.push_back(m_arTextRects[i]);
	//	Shape->m_strRect = m_strRect;

	//	Shape->m_arHandles.clear();
	//	for(int i = 0; i < m_arHandles.size(); i++)
	//		Shape->m_arHandles.push_back(m_arHandles[i]);

	//	Shape->m_strTransformXml = m_strTransformXml;
	//	return true;
	//}
};
}