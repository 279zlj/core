// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Numbering.h"
#include "Exception/log_runtime_error.h"
#include "FileTypes.h"

namespace OOX
{
	Numbering::Numbering()
	{
	}

	Numbering::Numbering(const OOX::CPath& filename)
	{
		read(filename);
	}

	Numbering::~Numbering()
	{
	}

	void Numbering::read(const OOX::CPath& oPath)
	{
		clear();

		XmlUtils::CXmlNode oNumbering;
		oNumbering.FromXmlFile( oPath.GetPath(), true );

		if ( _T("w:numbering") == oNumbering.GetName() )
		{
			XmlUtils::CXmlNodes oAbstractNumList;
			oNumbering.GetNodes( _T("w:abstractNum"), oAbstractNumList );

			for ( int nIndex = 0; nIndex < oAbstractNumList.GetCount(); nIndex++ )
			{
				XmlUtils::CXmlNode oAbstractNumNode;
				if ( oAbstractNumList.GetAt( nIndex, oAbstractNumNode ) )
				{
					OOX::Numbering::AbstractNum oAbstractNum;
					oAbstractNum.fromXML(oAbstractNumNode);
					AbstractNums.push_back( oAbstractNum );
				}
			}

			XmlUtils::CXmlNodes oNumList;
			oNumbering.GetNodes( _T("w:num"), oNumList );

			for ( int nIndex = 0; nIndex < oNumList.GetCount(); nIndex++ )
			{
				XmlUtils::CXmlNode oNumNode;
				if ( oNumList.GetAt( nIndex, oNumNode ) )
				{
					OOX::Numbering::Num oNum;
					oNum.fromXML(oNumNode);
					Nums.push_back( oNum );
				}
			}
		}
	}

	void Numbering::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	void Numbering::clear()
	{
		AbstractNums.clear();
		Nums.clear();
	}

	const FileType Numbering::type() const
	{
		return FileTypes::Numbering;
	}

	const OOX::CPath Numbering::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Numbering::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} 

namespace OOX
{
	Numbering::Num::Num()
	{
		NumId			=	0;
		AbstractNumId	=	0;
	}

	Numbering::Num::~Num()
	{
	}

	Numbering::Num::Num(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::Num& Numbering::Num::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::Num::fromXML(XmlUtils::CXmlNode& oNode)
	{
		NumId = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:numId"))));

		XmlUtils::CXmlNode oChild;
		if ( oNode.GetNode( _T("w:startOverride"), oChild ) )
			AbstractNumId = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

		XmlUtils::CXmlNodes oLvlList;
		if ( oNode.GetNodes( _T("w:lvlOverride"), oLvlList ) )
		{
			XmlUtils::CXmlNode oLvlNode;
			for ( int nIndex = 0; nIndex < oLvlList.GetCount(); ++nIndex )
			{
				if ( oLvlList.GetAt( nIndex, oLvlNode ) )
				{
					OOX::Numbering::LevelOverride oLvl;
					oLvl.fromXML(oLvlNode);
					LevelOverrides.push_back(oLvl);
				}
			}
		}
	}
}

namespace OOX
{
	Numbering::LevelOverride::LevelOverride()
	{
		Ilvl			=	0;
		StartOverride	=	0;
	}

	Numbering::LevelOverride::~LevelOverride()
	{
	}

	Numbering::LevelOverride::LevelOverride(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::LevelOverride& Numbering::LevelOverride::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::LevelOverride::fromXML(XmlUtils::CXmlNode& oNode)
	{
		Ilvl = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:ilvl"))));

		XmlUtils::CXmlNode oChild;
		if ( oNode.GetNode( _T("w:startOverride"), oChild ) )
			StartOverride = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

		if ( oNode.GetNode( _T("w:lvl"), oChild ) )
		{
			OOX::Numbering::Level lvl;
			lvl.fromXML(oChild);
			Level = lvl;
		}
	}
} 

namespace OOX
{
	Numbering::Level::Level()
	{
		Tentative	=	0;
		Align		=	Logic::Align();
		NumFmt		=	OOX::Logic::NumFormat();
	}

	Numbering::Level::~Level()
	{
	}

	Numbering::Level::Level(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::Level& Numbering::Level::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::Level::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:lvl") == oNode.GetName() )
		{
			Ilvl = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:ilvl"))));
			Tentative = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:tentative"))));
			Tplc = std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:tentative"))));

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( _T("w:suff"), oChild ) )
				Suffix = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			if ( oNode.GetNode( _T("w:lvlText"), oChild ) )
				Text = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			if ( oNode.GetNode( _T("w:lvlJc"), oChild ) )
				Align = Logic::Align(std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val")))));

			if ( oNode.GetNode( _T("w:start"), oChild ) )
				Start = _wtoi(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			if ( oNode.GetNode( _T("w:numFmt"), oChild ) )
			{
				OOX::Logic::NumFormat fmt;
				fmt.fromXML(oChild);
				NumFmt = fmt;
			}
		}

		///NOTE:

		////ParagraphProperty	=	element.element("pPr");
		////RunProperty			=	element.element("rPr");
	}
}

namespace OOX
{
	Numbering::AbstractNum::AbstractNum()
	{
		Id	=	0;
	}

	Numbering::AbstractNum::~AbstractNum()
	{

	}

	Numbering::AbstractNum::AbstractNum(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
	}

	const Numbering::AbstractNum& Numbering::AbstractNum::operator =(XmlUtils::CXmlNode& oNode)
	{
		fromXML(oNode);
		return *this;
	}

	void Numbering::AbstractNum::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:abstractNum") == oNode.GetName() )
		{
			Id = _wtoi(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:abstractNumId"))));

			XmlUtils::CXmlNode oChild;
			if ( oNode.GetNode( _T("w:nsid"), oChild ) )
				Nsid = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:multiLevelType"), oChild ) )
				MultiLevelType = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:tmpl"), oChild ) )
				Tmpl = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:numStyleLink"), oChild ) )
				numStyleLink = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));

			XmlUtils::CXmlNodes oLvlList;
			if ( oNode.GetNodes( _T("w:lvl"), oLvlList ) )
			{
				XmlUtils::CXmlNode oLvlNode;
				for ( int nIndex = 0; nIndex < oLvlList.GetCount(); ++nIndex )
				{
					if ( oLvlList.GetAt( nIndex, oLvlNode ) )
					{
						OOX::Numbering::Level oLvl;
						oLvl.fromXML(oLvlNode);
						Levels.push_back(oLvl);
					}
				}
			}
		}
	}

	const Numbering::Level Numbering::AbstractNum::getLevel(const int numLevel) const
	{
		for (std::vector<Level>::const_iterator iter = Levels.begin(); iter != Levels.end(); ++iter)
		{
			if ((*iter).Ilvl == numLevel)
				return (*iter);
		}

		throw log_runtime_error("bad abstractNum");
	}
}