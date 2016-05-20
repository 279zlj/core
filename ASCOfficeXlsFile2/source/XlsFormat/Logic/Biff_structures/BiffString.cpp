
#include "BiffString.h"
#include <utils.h>

namespace XLS
{

BiffString::BiffString()
:	struct_size(0)
{
}


BiffString::BiffString(const size_t size)
:	struct_size(0),
	cch_(size)
{
}


BiffString::BiffString(const std::wstring & str)
:	struct_size(0),
	str_(str),
	cch_(str.length())
{
}


BiffString::~BiffString() 
{
}




BiffString::operator std::wstring  () const
{
	return str_;
}


BiffString BiffString::operator=(const std::wstring & str)
{
	str_ = str;
	cch_ = str_.length();
	return *this;
}


BiffStructurePtr BiffString::clone()
{
	return BiffStructurePtr(new BiffString(*this));
}

void BiffString::store(CFRecord& record)
{
	throw;// EXCEPT::LE::WhatIsTheFuck("Wrong usage of BiffString. The function must not be called.", __FUNCTION__);
}


void BiffString::load(CFRecord& record)
{
	throw;// EXCEPT::LE::WhatIsTheFuck("Wrong usage of BiffString. Stack overflow stopped.", __FUNCTION__);
//	record >> *this; // :-)
}


void BiffString::load(CFRecord& record, const size_t cch1, const bool is_wide1)
{
	bool is_wide	= is_wide1;

	size_t cch = cch1;
	if ((cch_) && (*cch_ != cch1) && cch1 < 1)
	{
		cch =  cch_.get();
	}
	size_t raw_length = cch << (is_wide ? 1 : 0);
	
	if (record.checkFitRead(raw_length)==false)
	{
		//������ - ��������� Continue records - ����� ����� ����� ��� ������ � ������������
		return;
	}

	if(is_wide)
	{
#if defined(_WIN32) || defined(_WIN64)
		std::wstring inp_str(record.getCurData<wchar_t>(), cch);
		str_ = inp_str.c_str();
#else
		str_= convertUtf16ToWString(record.getCurData<UTF16>(), cch);
#endif
	}
	else
	{
		std::string inp_str(record.getCurData<char>(), cch);

		if (record.getGlobalWorkbookInfo()->CodePage == 1200)
		{
			int inp_str_size = inp_str.length();
            UTF16 *out_str = new UTF16[inp_str_size + 1];
			char* out_str_char = (char*) out_str;
			for (int i = 0; i < inp_str_size; i++)
			{
				out_str_char[2*i+0] = inp_str.c_str()[i];
				out_str_char[2*i+1] = 0;
			}
			out_str[inp_str_size] = 0;

#if defined(_WIN32) || defined(_WIN64)
                str_ = std::wstring((wchar_t*)out_str, inp_str_size);
#else
                str_ = convertUtf16ToWString(out_str, inp_str_size);
#endif
			delete []out_str;
		}
		else
		{
			str_ = STR::toStdWString(inp_str, record.getGlobalWorkbookInfo()->CodePage).c_str();
		}

	}
	record.skipNunBytes(raw_length);
}


void BiffString::store(CFRecord& record, const bool is_wide)
{
	size_t raw_length = getSize() << (is_wide ? 1 : 0);
	record.checkFitWrite(raw_length);

	if(is_wide)
	{
		std::wstring int_str = str_;
		for(std::wstring::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
	else
	{
		std::string int_str = STR::toStdString(str_, record.getGlobalWorkbookInfo()->CodePage);
		for(std::string::iterator it = int_str.begin(), itEnd = int_str.end(); it != itEnd; ++it)
		{
			record << *it;
		}
		// no trailing zero - it is OK
	}
}


const size_t BiffString::getSize() const
{
	if(!cch_)
	{
		throw;// EXCEPT::LE::StructureSizeNotSet("BiffString", __FUNCTION__/*__FUNCDNAME__*/); 
	}
	return *cch_;
}


void BiffString::setSize(const size_t size)
{
	cch_ = size;
}


const size_t BiffString::getStructSize() const
{
	return struct_size;
}


// Set number of unsigned chars read while loading. Must be used by >> operator only
void BiffString::setStructSize(const size_t size)
{
	struct_size = size;
}


const bool BiffString::isConformToOleLink() const
{
#pragma message("####################### BiffString::isConformToOleLink is not implemented")
	Log::info("BiffString::isConformToOleLink is not implemented.");
	size_t len = str_.length();
	for(size_t i = 0; i < len; ++i)
	{
		
	}
	return true;
}


const bool BiffString::isConformToVirtPath() const
{
#pragma message("####################### BiffString::isConformToVirtPath is not implemented")
	Log::info("BiffString::isConformToVirtPath is not implemented.");
	return true;
}

const std::wstring  BiffString::getEscaped_ST_Xstring() const
{
	if(!str_.length())
	{
		return str_;
	}
	std::wstring copy_str(str_);
	return STR::escape_ST_Xstring(copy_str).c_str();
}


void BiffString::Escape_ST_Xstring()
{
	str_ = getEscaped_ST_Xstring();
}


const bool BiffString::getWideRecommendation() const
{
	std::wstring str(str_);
	for(std::wstring::const_iterator it = str.begin(), itEnd = str.end(); it != itEnd; ++it)
	{
		if(0xff00 & *it)
		{
			return true;
		}
	}
	return false;
}

} // namespace XLS
