
#include "FORMULA.h"
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Formula.h>
#include <Logic/Biff_records/Array.h>
#include <Logic/Biff_records/Table.h>
#include <Logic/Biff_records/ShrFmla.h>
#include <Logic/Biff_unions/SUB.h>
#include <Logic/Biff_records/String.h>
#include <Logic/Biff_records/Continue.h>

#include <utils.h>

namespace XLS
{

extern int cellStyleXfs_count;

FORMULA::FORMULA(std::vector<CellRangeRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
	m_sharedIndex = -1;
}


FORMULA::~FORMULA()
{
}


BaseObjectPtr FORMULA::clone()
{
	return BaseObjectPtr(new FORMULA(*this));
}


// FORMULA = [Uncalced] Formula [Array / Table / ShrFmla / SUB] [String *Continue]
const bool FORMULA::loadContent(BinProcessor& proc)
{
	proc.optional<Uncalced>();

	Formula *formula = NULL;
	if(!proc.mandatory<Formula>())
	{
		return false;
	}
	m_Formula = elements_.back();
	elements_.pop_back();

	formula = dynamic_cast<Formula *>(m_Formula.get());

	location = formula->getLocation();

	Array array_formula(location);
	if (proc.optional(array_formula))
	{
		m_ArrayFormula = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<Table>())
	{
		m_TableFormula = elements_.back();
		elements_.pop_back();
	}
	
	if(!m_ArrayFormula && !m_TableFormula)
	{
		//proc.optional<ShrFmla>(); // replaced with below:
		ShrFmla shr_formula(location);
		if(proc.optional(shr_formula))
		{
			m_SharedFormula = elements_.back();
			elements_.pop_back();

			m_sharedIndex = shared_formulas_locations_ref_.size();
			shared_formulas_locations_ref_.push_back(shr_formula.ref_);
		}
		//proc.optional<SUB>(); // I haven't found any mention about SUB in the docs
	}

	if ((formula) && (formula->fShrFmla.value()) && (formula->fShrFmla))
	{
		for (int i = 0; i < shared_formulas_locations_ref_.size(); i++)
		{
			if (shared_formulas_locations_ref_[i].inRange(location)) m_sharedIndex = i;
		}		
	}

	if(proc.optional<String>())
	{
		proc.repeated<Continue>(0, 0);
	}

	return true;
}

const CellRef FORMULA::getLocation() const
{
	return location;
}

int FORMULA::serialize(std::wostream & stream)
{
	Formula *formula	= dynamic_cast<Formula *>(m_Formula.get());
	ShrFmla *shared		= dynamic_cast<ShrFmla *>(m_SharedFormula.get());

	if (formula == NULL) return 0;

	CP_XML_WRITER(stream)    
    {
		std::wstring ref = formula->cell.getLocation().toString();
		
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if ((formula->cell.ixfe.value()) && (formula->cell.ixfe > cellStyleXfs_count))
			{
				CP_XML_ATTR(L"s", formula->cell.ixfe - cellStyleXfs_count);
			}
			switch (formula->val.getType())
			{
				case 0: CP_XML_ATTR(L"t", L"str");	break;
				case 1: CP_XML_ATTR(L"t", L"b");	break;
				case 2: CP_XML_ATTR(L"t", L"e");	break;
			}		

			const std::wstring & f_ = formula->formula.getAssembledFormula();
			if (!f_.empty())
			{
				CP_XML_NODE(L"f")
				{
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(f_);
				}
			}
			else if (shared)
			{
				const std::wstring & f_ = shared->formula.getAssembledFormula();
				CP_XML_NODE(L"f")
				{
					std::wstring shared_ref = shared->ref_.toString();
					CP_XML_ATTR(L"ref", shared_ref);	
					CP_XML_ATTR(L"t", L"shared");	
					CP_XML_ATTR(L"si", m_sharedIndex);	

					CP_XML_STREAM() << xml::utils::replace_text_to_xml(f_);
				}
			}
			else if ((formula->fShrFmla.value()) && (formula->fShrFmla == true) && m_sharedIndex >= 0)
			{
				CP_XML_NODE(L"f")
				{
					CP_XML_ATTR(L"t", L"shared");	
					CP_XML_ATTR(L"si", m_sharedIndex);	
				}
			}
			std::wstring str_val = formula->val.getValue();
			if (!str_val.empty())
			{
				CP_XML_NODE(L"v")
				{
					CP_XML_STREAM() << str_val;
				}
			}
		}			
	}
	return 0;
}

} // namespace XLS

