#pragma once
#ifndef OOX_WORKBOOK_FILE_INCLUDE_H_
#define OOX_WORKBOOK_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "BookViews.h"
#include "DefinedNames.h"
#include "Sheets.h"
#include "WorkbookPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//�������������� child:
		//<calcPr>
		//<customWorkbookViews>
		//<externalReferences>
		//<extLst>
		//<fileRecoveryPr>
		//<fileSharing>
		//<fileVersion>
		//<functionGroups>
		//<oleSize>
		//<pivotCaches>
		//<smartTagPr>
		//<smartTagTypes>
		//<webPublishing>
		//<webPublishObjects>
		//<workbookPr>
		//<workbookProtection>
		class CWorkbook : public OOX::File, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CWorkbook()
			{
			}
			CWorkbook(const CPath& oPath)
			{
				read(oPath);
			}
			virtual ~CWorkbook()
			{
			}
		public:

			virtual void read(const CPath& oPath)
			{
				m_oReadPath = oPath;
				IFileContainer::Read( oPath );

				XmlUtils::CXmlLiteReader oReader;

				if ( !oReader.FromFile( oPath.GetPath() ) )
					return;

				if ( !oReader.ReadNextNode() )
					return;

				CWCharWrapper sName = oReader.GetName();
				if ( _T("workbook") == sName )
				{
					if ( !oReader.IsEmptyNode() )
					{
						int nDocumentDepth = oReader.GetDepth();
						while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
						{
							sName = oReader.GetName();

							if ( _T("bookViews") == sName )
								m_oBookViews = oReader;
							else if ( _T("definedNames") == sName )
								m_oDefinedNames = oReader;
							else if ( _T("sheets") == sName )
								m_oSheets = oReader;
							else if ( _T("workbookPr") == sName )
								m_oWorkbookPr = oReader;
						}
					}
				}		
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				CStringWriter sXml;
				sXml.WriteStringC(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"));

				if(m_oWorkbookPr.IsInit())
					m_oWorkbookPr->toXML(sXml);
				if(m_oBookViews.IsInit())
					m_oBookViews->toXML(sXml);
				if(m_oSheets.IsInit())
					m_oSheets->toXML(sXml);
				if(m_oDefinedNames.IsInit())
					m_oDefinedNames->toXML(sXml);

				sXml.WriteStringC(_T("<calcPr calcId=\"145621\"/>"));
				sXml.WriteStringC(_T("</workbook>"));

				CDirectory::SaveToFile( oPath.GetPath(), sXml.GetCString() );

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write( oPath, oDirectory, oContent );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Workbook;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
			void PrepareToWrite()
			{
				//WorkbookPr
				if(false == m_oWorkbookPr.IsInit())
					m_oWorkbookPr.Init();
				//todo ���� ���� �������� ���������, �� ������� ����������� ����, � �� �� �����.
				//if(false == m_oWorkbookPr->m_oDefaultThemeVersion.IsInit())
				//{
				//	m_oWorkbookPr->m_oDefaultThemeVersion.Init();
				//	m_oWorkbookPr->m_oDefaultThemeVersion->SetValue(124226);
				//}
				//BookViews
				if(false == m_oBookViews.IsInit())
					m_oBookViews.Init();
				if(0 == m_oBookViews->m_arrItems.GetSize())
					m_oBookViews->m_arrItems.Add(new OOX::Spreadsheet::CWorkbookView());
				OOX::Spreadsheet::CWorkbookView* pWorkbookView = m_oBookViews->m_arrItems[0];
				if(false == pWorkbookView->m_oXWindow.IsInit())
				{
					pWorkbookView->m_oXWindow.Init();
					pWorkbookView->m_oXWindow->SetValue(360);
				}
				if(false == pWorkbookView->m_oYWindow.IsInit())
				{
					pWorkbookView->m_oYWindow.Init();
					pWorkbookView->m_oYWindow->SetValue(15);
				}
				if(false == pWorkbookView->m_oWindowWidth.IsInit())
				{
					pWorkbookView->m_oWindowWidth.Init();
					pWorkbookView->m_oWindowWidth->SetValue(20955);
				}
				if(false == pWorkbookView->m_oWindowHeight.IsInit())
				{
					pWorkbookView->m_oWindowHeight.Init();
					pWorkbookView->m_oWindowHeight->SetValue(9720);
				}
			}
		private:
			CPath									m_oReadPath;

		public:
			nullable<OOX::Spreadsheet::CBookViews> m_oBookViews;
			nullable<OOX::Spreadsheet::CDefinedNames> m_oDefinedNames;
			nullable<OOX::Spreadsheet::CSheets> m_oSheets;
			nullable<OOX::Spreadsheet::CWorkbookPr> m_oWorkbookPr;

		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKBOOK_FILE_INCLUDE_H_