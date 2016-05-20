#include "SST.h"

#include <utils.h>

namespace XLS
{

SST::SST(const unsigned short code_page)
:	code_page_(code_page)
{
}


SST::~SST()
{
}


BaseObjectPtr SST::clone()
{
	return BaseObjectPtr(new SST(*this));
}


void SST::writeFields(CFRecord& record)
{
	record << cstTotal << cstUnique;

	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	for (BiffStructurePtrVector::iterator it = rgb.begin(), itEnd = rgb.end(); it != itEnd ; ++it)
	{
		XLUnicodeRichExtendedStringPtr element = boost::dynamic_pointer_cast<XLUnicodeRichExtendedString>(*it);
		if(!element)
		{
			throw;// EXCEPT::LE::WrongAPIUsage("boost::shared_dynamic_cast failed to cast to XLUnicodeRichExtendedStringPtr", __FUNCTION__);
		}
		CFRecord& current_record = recs.size() ? *recs.back() : record; // Points to the original 'record' or the last created 'Continue'

		_UINT32 ssss;
		if(!current_record.checkFitWriteSafe(ssss = element->getNonVariablePartSize())) // If we cannot write non-variable part into the current record
		{
			current_record.commitData();
			CFRecordPtr cont_record(new CFRecord(rt_Continue, record.getGlobalWorkbookInfo()));
			recs.push_back(cont_record);
			*cont_record << *element;
		}
		else
		{
			current_record << *element;
		}
	}
	if(recs.size())
	{
		recs.back()->commitData();
	}
}


void SST::readFields(CFRecord& record)
{
	record >> cstTotal >> cstUnique;
	long total = cstTotal;
	long unique = cstUnique;
		
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	int count = 0 ;
	while(!record.isEOF() || !recs.empty())
	{
		XLUnicodeRichExtendedStringPtr element(new XLUnicodeRichExtendedString(recs));

		if (record.getRdPtr() + 3 > record.getDataSize()) // If the break is at the XLUnicodeRichExtendedString boundary
		{
			element->appendNextContinue(record, false);
		}

		if (record.getRdPtr() + 3 > record.getDataSize())
			break;

		record >> *element;
		rgb.push_back(element);
		count++;
	}
}


int SST::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		for (int i=0; i < rgb.size(); i++)
		{
			XLUnicodeRichExtendedString *richText = dynamic_cast<XLUnicodeRichExtendedString *>(rgb[i].get());

			if (richText == NULL) continue;

			CP_XML_NODE(L"si")
			{	
				try
				{
					//������ �� ����������- � ��������� ��������� ����������� ������ r-rPr-t � ��������� ���������� t
					if (richText->rgRun.size() >0)
					{							
						richText->serialize(CP_XML_STREAM());
					}
					else
					{
						CP_XML_NODE(L"t")
						{		
							CP_XML_ATTR(L"xml:space", L"preserve");
							CP_XML_STREAM() << STR::escape_ST_Xstring(xml::utils::replace_text_to_xml(richText->str_));
						}
					}
				}
				catch(...)
				{
					CP_XML_NODE(L"t");
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

