//#include "stdafx.h"
#include "CommonInclude.h"

#include "IFileContainer_Spreadsheet.h"
#include "FileFactory_Spreadsheet.h"

#include "../DocxFormat/Rels.h"
#include "../DocxFormat/ContentTypes.h"

#include "../DocxFormat/FileType.h"
#include "../DocxFormat/External/External.h"

#include "../DocxFormat/External/HyperLink.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CAtlMap<CString, size_t> IFileContainer::m_mapEnumeratedGlobal;
		UnknowTypeFile IFileContainer::Unknown;

		void IFileContainer::Read (const OOX::CPath& oPath)
		{
			// ������� �����(������) � ������ ������
			RELEASEOBJECT(m_pCurRels);
			m_pCurRels = new OOX::CRels(oPath);

			// ������ ��� ����� �� �������
			Read( *m_pCurRels, oPath.GetDirectory() );
		}


		void IFileContainer::Read (const OOX::CRels& oRels, const OOX::CPath& oPath)
		{
			unsigned int nCount = oRels.m_arrRelations.size();

			for (unsigned int nIndex = 0; nIndex < nCount; ++nIndex )
			{
				smart_ptr<OOX::File>& pFile = OOX::Spreadsheet::CreateFile( oPath, oRels.m_arrRelations[nIndex] );
				Add( oRels.m_arrRelations[nIndex]->rId(), pFile );
			}
		}


		void IFileContainer::Write(const OOX::CPath& oFileName, const OOX::CPath& oDirectory, OOX::CContentTypes& oContent) const
		{
			OOX::CPath oCurrent = oFileName.GetDirectory();
			OOX::CRels oRels;

			Write( oRels, oCurrent, oDirectory, oContent );
			oRels.Write( oFileName );
		}
		void IFileContainer::Write(OOX::CRels& oRels, const OOX::CPath& oCurrent, const OOX::CPath& oDir, OOX::CContentTypes& oContent) const
		{
			CAtlMap<CString, size_t> mNamePair;

			POSITION pos = m_mContainer.GetStartPosition();

			while ( NULL != pos )
			{
				const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );

				smart_ptr<OOX::File>     pFile = pPair->m_value;
				smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

				if ( !pExt.IsInit() )
				{
					OOX::CPath oDefDir = pFile->DefaultDirectory();
					OOX::CPath oName   = pFile->DefaultFileName();
					if(false == pFile->m_sFilename.IsEmpty())
						oName.SetName(pFile->m_sFilename, false);

					OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );
					pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
					if(true != pFile->m_bDoNotAddRels)
						oRels.Registration( pPair->m_key, pFile->type(), oDefDir / oName );
				}
				else
				{
					oRels.Registration( pPair->m_key, pExt );
				}
			}
		}


		void IFileContainer::Commit  (const OOX::CPath& oPath)
		{
			CAtlMap<CString, size_t> mNamepair;

			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );

				smart_ptr<OOX::File>     pFile = pPair->m_value;
				smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

				if (!pExt.IsInit())
				{
					OOX::CPath oDefDir = pFile->DefaultDirectory();
					OOX::CPath oName   = pFile->DefaultFileName();

					CAtlMap<CString, size_t>::CPair* pNamePair = mNamepair.Lookup( oName.m_strFilename );
					if (NULL == pNamePair)
						mNamepair.SetAt( oName.m_strFilename, 1 );
					else
						oName = oName + pNamePair->m_key;

					OOX::CSystemUtility::CreateDirectories( oPath / oDefDir );

					smart_ptr<OOX::IFileBuilder> pFileBuilder = pPair->m_value.smart_dynamic_cast<OOX::IFileBuilder>();
					if ( pFileBuilder.is_init() )
						pFileBuilder->Commit( oPath / oDefDir / oName );
				}
			}
		}


		void IFileContainer::Finalize(const OOX::CPath& oFileName, const OOX::CPath& oDirectory, OOX::CContentTypes& oContent)
		{
			OOX::CPath oCurrent = oFileName.GetDirectory();
			OOX::CRels oRels;

			Finalize( oRels, oCurrent, oDirectory, oContent );
			oRels.Write( oFileName );
		}	
		void IFileContainer::Finalize(OOX::CRels& oRels, const OOX::CPath& oCurrent, const OOX::CPath& oDir, OOX::CContentTypes& oContent)
		{
			CAtlMap<CString, size_t> mNamepair;

			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );

				smart_ptr<OOX::File>     pFile = pPair->m_value;
				smart_ptr<OOX::External> pExt  = pFile.smart_dynamic_cast<OOX::External>();

				if ( !pExt.IsInit() )
				{
					OOX::CPath oDefDir = pFile->DefaultDirectory();
					OOX::CPath oName   = pFile->DefaultFileName();

					CAtlMap<CString, size_t>::CPair* pNamePair = mNamepair.Lookup( oName.m_strFilename );
					if ( NULL == pNamePair )
						mNamepair.SetAt( oName.m_strFilename, 1 );
					else
						oName = oName + pNamePair->m_key;

					OOX::CSystemUtility::CreateDirectories( oCurrent / oDefDir );				
					smart_ptr<OOX::IFileBuilder> pFileBuilder = pFile.smart_dynamic_cast<OOX::IFileBuilder>(); 
					if ( pFileBuilder.is_init() )
					{
						pFileBuilder->Finalize( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
					}
					else
					{
						pFile->write( oCurrent / oDefDir / oName, oDir / oDefDir, oContent );
					}

					oRels.Registration( pPair->m_key, pFile->type(), oDefDir / oName );
				}
				else
				{
					oRels.Registration( pPair->m_key, pExt );
				}
			}
		}

		const bool IFileContainer::IsExist(const FileType& oType) const
		{
			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );
				if ( oType == pPair->m_value->type() )
					return true;
			}
			return false;
		}
		const bool IFileContainer::IsExist(const RId& rId) const
		{
			const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.Lookup(rId.get());
			return ( NULL != pPair );
		}


		template<typename T>
		const bool IFileContainer::IsExist() const
		{
			T oFile;
			return IsExist( oFile.type() );
		}
		CString IFileContainer::IsExistHyperlink(smart_ptr<OOX::HyperLink>& pHyperLink)
		{
			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );
				if(OOX::FileTypes::HyperLink == pPair->m_value->type())
				{
					smart_ptr<OOX::HyperLink> pCurHyperlink = pPair->m_value.smart_dynamic_cast<OOX::HyperLink>();
					if(pCurHyperlink->Uri().GetPath() == pHyperLink->Uri().GetPath())
						return pPair->m_key;
				}
			}
			return CString();
		}
		const bool IFileContainer::IsExternal(const OOX::RId& rId) const
		{
			const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.Lookup(rId.get());

			if ( NULL != pPair )
			{
				CString sType = pPair->m_value->type().RelationType();
				CString sName = pPair->m_value->type().DefaultFileName().m_strFilename;

				return (( ( sType == OOX::FileTypes::ExternalAudio.RelationType() ) || ( sType == OOX::FileTypes::ExternalImage.RelationType() ) || ( sType == OOX::FileTypes::ExternalVideo.RelationType() ) ) && ( sName == _T("") ) );
			}
			return true;
		}


		smart_ptr<OOX::File> IFileContainer::Get(const FileType& oType)
		{
			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );
				if ( oType == pPair->m_value->type() )
					return pPair->m_value;
			}
			return smart_ptr<OOX::File>(new UnknowTypeFile( Unknown ));
		}


		const RId IFileContainer::Add(smart_ptr<OOX::File>& pFile)
		{
			const RId rId = GetMaxRId().next();
			Add( rId, pFile );
			return rId;
		}


		void      IFileContainer::Add(const OOX::RId& rId, smart_ptr<OOX::File>& pFile)
		{
			bool bEnumerated = pFile->type().Enumerated();
			bool bEnumeratedGlobal = pFile->type().EnumeratedGlobal();

			if(true == bEnumeratedGlobal || true == bEnumerated)
			{
				CAtlMap<CString, size_t>::CPair* pNamePair = NULL;
				if(true == bEnumeratedGlobal)
					pNamePair = m_mapEnumeratedGlobal.Lookup( pFile->type().OverrideType() );
				else
					pNamePair = m_mapAddNamePair.Lookup( pFile->type().OverrideType() );
				int nIndex = 0;
				if(NULL != pNamePair)
					nIndex = pNamePair->m_value;
				nIndex++;

				if(true == bEnumeratedGlobal)
				{
					if(pFile.is<OOX::FileGlobalEnumerated>())
					{
						OOX::FileGlobalEnumerated& oFileGlobalEnumerated = pFile.as<OOX::FileGlobalEnumerated>();
						oFileGlobalEnumerated.SetGlobalNumber(nIndex);
					}
				}

				CString sPath = pFile->DefaultFileName().GetPath();
				int nDotIndex = sPath.ReverseFind('.');
				if(-1 != nDotIndex && nDotIndex > 0)
				{
					CString sDigit;sDigit.Format(_T("%d"), nIndex);
					sPath.Insert(nDotIndex, sDigit);
				}
				pFile->m_sFilename = sPath;
				if(true == bEnumeratedGlobal)
					m_mapEnumeratedGlobal.SetAt( pFile->type().OverrideType(), nIndex );
				else
					m_mapAddNamePair.SetAt( pFile->type().OverrideType(), nIndex );
			}

			m_lMaxRid = max( m_lMaxRid, rId.getNumber() );
			m_mContainer.SetAt( rId.get(), pFile );
		}
		smart_ptr<OOX::File> IFileContainer::Find(const FileType& oType) const
		{
			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );
				if ( oType == pPair->m_value->type() )
					return pPair->m_value;
			}
			return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
		}
		void IFileContainer::FindAllByType(const FileType& oType, CAtlMap<CString, smart_ptr<OOX::File>>& aOutput) const
		{
			POSITION pos = m_mContainer.GetStartPosition();
			while ( NULL != pos )
			{
				const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.GetNext( pos );
				if ( oType == pPair->m_value->type() )
				{
					aOutput.SetAt(pPair->m_key, pPair->m_value);
				}
			}
		}
		smart_ptr<OOX::File> IFileContainer::Find(const OOX::RId& rId) const
		{
			const CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.Lookup(rId.get());
			if ( NULL != pPair )
				return pPair->m_value;

			return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
		}


		template<typename T>
		T&                   IFileContainer::Find()
		{
			T oFile;
			return dynamic_cast<T&>( Find( oFile.type() ) );
		}	
		smart_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId)
		{
			CAtlMap<CString, smart_ptr<OOX::File>>::CPair* pPair = m_mContainer.Lookup(rId.get());
			if ( NULL != pPair )
				return pPair->m_value;

			return smart_ptr<OOX::File>( (OOX::File*)new UnknowTypeFile() );
		}


		smart_ptr<OOX::File> IFileContainer::operator [](const FileType& oType)
		{
			return Find( oType );
		}

		const RId IFileContainer::GetMaxRId()
		{
			return RId( m_lMaxRid );
		}
		void IFileContainer::SetGlobalNumberByType(const CString& sOverrideType, int val)
		{
			m_mapEnumeratedGlobal.SetAt( sOverrideType, val );
		}
		int IFileContainer::GetGlobalNumberByType(const CString& sOverrideType)
		{
			CAtlMap<CString, size_t>::CPair* pNamePair = m_mapEnumeratedGlobal.Lookup( sOverrideType );
			int nRes = 0;
			if(NULL != pNamePair)
				nRes = pNamePair->m_value;
			return nRes;
		}
	}
} // namespace OOX