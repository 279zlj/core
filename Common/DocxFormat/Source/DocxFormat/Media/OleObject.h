#pragma once
#ifndef OOX_OLE_OBJECT_INCLUDE_H_
#define OOX_OLE_OBJECT_INCLUDE_H_

#include "Media.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"

namespace OOX
{
	class OleObject : public Media
	{
	public:
		OleObject(bool bSpreadsheet_ = false)
		{
			bSpreadsheet	= bSpreadsheet_;
		}
		OleObject(const OOX::CPath& filename)
		{
			read(filename);
		}
		virtual ~OleObject()
		{
		}

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
		{
			//std::wstring newFilename = filename.filename();
			//boost::filesystem::wpath newFilePath = filename.parent_path();
			//boost::replace_all(newFilename, L" ", L"_");
			//if (m_filename.extension() != L".bin")
			//	newFilename += L".bin" ;
			//
			//boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
			//content.Default->add(newFilePath/newFilename);
		}

	public:
		virtual const FileType type() const
		{
			if (bSpreadsheet)	return OOX::Spreadsheet::FileTypes::OleObject;
			else				return OOX::FileTypes::OleObject;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	private:
		bool bSpreadsheet;
	};
} // namespace OOX

#endif // OOX_OLE_OBJECT_INCLUDE_H_
