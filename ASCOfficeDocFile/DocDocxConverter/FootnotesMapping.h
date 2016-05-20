#pragma once

#include "ContentTypes.h"
#include "WordDocument.h"
#include "DocumentMapping.h"

namespace DocFileFormat
{
	class FootnotesMapping: public DocumentMapping
	{
	public: 
		FootnotesMapping (ConversionContext* ctx): DocumentMapping( ctx, this )
		{
		}

		virtual void Apply( IVisitable* visited )
		{
			m_document = static_cast<WordDocument*>( visited );

			if ( m_document && ( m_document->FIB->m_RgLw97.ccpFtn > 0 ) )
			{
				m_context->_docx->RegisterFootnotes();

				int id = 0;

				m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
				m_pXmlWriter->WriteNodeBegin( _T( "w:footnotes" ), TRUE );

				//write namespaces
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:v" ), OpenXmlNamespaces::VectorML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:o" ), OpenXmlNamespaces::Office );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w10" ), OpenXmlNamespaces::OfficeWord );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:r" ), OpenXmlNamespaces::Relationships );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				int cp = m_document->FIB->m_RgLw97.ccpText;

				while ( cp <= ( m_document->FIB->m_RgLw97.ccpText + m_document->FIB->m_RgLw97.ccpFtn - 2 ) )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:footnote" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString( id ).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					while ( ( cp - m_document->FIB->m_RgLw97.ccpText ) < (*m_document->IndividualFootnotesPlex)[id + 1] )
					{
						int fc =  m_document->FindFileCharPos(cp);
						if (fc < 0) break;

						ParagraphPropertyExceptions* papx = findValidPapx( fc );
						TableInfo tai( papx );

						if ( tai.fInTable )
						{
							//this PAPX is for a table
							Table table( this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ) );
							table.Convert( this );
							cp = table.GetCPEnd();
						}
						else
						{
							//this PAPX is for a normal paragraph
							cp = writeParagraph( cp );
						}
					}

					m_pXmlWriter->WriteNodeEnd( _T( "w:footnote" ) );
					id++;
				}

				m_pXmlWriter->WriteNodeEnd( _T( "w:footnotes" ) );

				m_context->_docx->FootnotesXML = wstring(m_pXmlWriter->GetXmlString());
			}
		}
	};
}