
#include "WorksheetSubstream.h"
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Index.h>
#include <Logic/Biff_unions/GLOBALS.h>
#include <Logic/Biff_unions/PAGESETUP.h>
//#include <Logic/Biff_records/HeaderFooter.h> // Moved inside PAGESETUP
#include <Logic/Biff_records/Dimensions.h>
#include <Logic/Biff_records/HFPicture.h>
#include <Logic/Biff_records/Note.h>
#include <Logic/Biff_records/DxGCol.h>
#include <Logic/Biff_records/MergeCells.h>
#include <Logic/Biff_records/LRng.h>
#include <Logic/Biff_records/CodeName.h>
#include <Logic/Biff_records/WebPub.h>
#include <Logic/Biff_records/CellWatch.h>
#include <Logic/Biff_records/SheetExt.h>
#include <Logic/Biff_records/EOF.h>
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_unions/BACKGROUND.h>
#include <Logic/Biff_unions/BIGNAME.h>
#include <Logic/Biff_unions/PROTECTION_COMMON.h> // special name to exclude ambiguous meaning
#include <Logic/Biff_unions/COLUMNS.h>
#include <Logic/Biff_unions/SCENARIOS.h>
#include <Logic/Biff_unions/SORTANDFILTER.h>
#include <Logic/Biff_unions/CELLTABLE.h>
#include <Logic/Biff_unions/OBJECTS.h>
#include <Logic/Biff_unions/PIVOTVIEW.h>
#include <Logic/Biff_unions/DCON.h>
#include <Logic/Biff_unions/WINDOW.h>
#include <Logic/Biff_unions/CUSTOMVIEW.h>
#include <Logic/Biff_unions/SORT.h>
#include <Logic/Biff_unions/QUERYTABLE.h>
#include <Logic/Biff_unions/PHONETICINFO.h>
#include <Logic/Biff_unions/CONDFMTS.h>
#include <Logic/Biff_unions/HLINK.h>
#include <Logic/Biff_unions/DVAL.h>
#include <Logic/Biff_unions/FEAT.h>
#include <Logic/Biff_unions/FEAT11.h>
#include <Logic/Biff_unions/RECORD12.h>
#include <Logic/Biff_unions/SHFMLA_SET.h>

namespace XLS
{;


WorksheetSubstream::WorksheetSubstream(const size_t ws_index)
:	ws_index_(ws_index)
{
}


WorksheetSubstream::~WorksheetSubstream()
{
}


BaseObjectPtr WorksheetSubstream::clone()
{
	return BaseObjectPtr(new WorksheetSubstream(*this));
}


/*
WORKSHEETCONTENT = [Uncalced] Index GLOBALS PAGESETUP [HeaderFooter] [BACKGROUND] *BIGNAME [PROTECTION] 
					COLUMNS [SCENARIOS] SORTANDFILTER Dimensions [CELLTABLE] OBJECTS *HFPicture *Note 
					*PIVOTVIEW [DCON] 1*WINDOW *CUSTOMVIEW *2SORT [DxGCol] *MergeCells [LRng] *QUERYTABLE 
					[PHONETICINFO] CONDFMTS *HLINK [DVAL] [CodeName] *WebPub *CellWatch [SheetExt] *FEAT 
					*FEAT11 *RECORD12 EOF
WORKSHEET = BOF WORKSHEETCONTENT
*/
const bool WorksheetSubstream::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();
	
	global_info->defaultColumnWidth		= 8;
	global_info->defaultRowHeight		= 14.4; 
	global_info->cmt_rules				= 0;

	int count = 0;

	if(!proc.mandatory<BOF>())
	{
		return false;
	}

	proc.optional<Uncalced>();
	proc.optional<Index>();			// OpenOffice Calc stored files workaround (Index is mandatory according to [MS-XLS])
	
    GLOBALS globals(false);
    if (proc.mandatory(globals))
	{
		m_GLOBALS = elements_.back();
		elements_.pop_back();
	}
//------------------------------������� ���� SchetPrintForm.xls
	if (proc.optional<Dimensions>())
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();
	}
	count = proc.repeated<WINDOW>(0, 0);
	while(count > 0)
	{
		m_arWINDOW.insert(m_arWINDOW.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
//----------------------------------------------

	// OpenOffice Calc stored files workaround (DefColWidth is mandatory and located inside COLUMNS according to [MS-XLS])
	if (proc.optional<COLUMNS>())
	{
		m_COLUMNS = elements_.back();
		elements_.pop_back();
	}
	if (proc.mandatory<PAGESETUP>())
	{
		m_PAGESETUP = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<BACKGROUND>())
	{
		m_BACKGROUND = elements_.back();
		elements_.pop_back();
	}

	proc.repeated<BIGNAME>(0, 0);
	proc.optional<PROTECTION_COMMON>();
	
	if (proc.optional<COLUMNS>())
	{
		if (!m_COLUMNS)
		{
			m_COLUMNS = elements_.back();
			elements_.pop_back();
		}
	}

	proc.optional<SCENARIOS>();
	
	if (proc.optional<SORTANDFILTER>())// Let it be optional
	{
		m_SORTANDFILTER = elements_.back();
		elements_.pop_back();
	}		
	
	if (proc.mandatory<Dimensions>())
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();
	}

    std::vector<CellRangeRef>	shared_formulas_locations;
    CELLTABLE        cell_table(shared_formulas_locations);
    if (proc.optional(cell_table))
	{
		m_CELLTABLE = elements_.back();
		elements_.pop_back();
	}
	
	if(0 != shared_formulas_locations.size())
	{
        SHFMLA_SET shfmla_set(shared_formulas_locations);
       
		if (proc.optional(shfmla_set))
		{
			m_SHFMLA_SET = elements_.back();
			elements_.pop_back();
		}
	}
    OBJECTS objects(false);
    if (proc.optional(objects))
	{
		m_OBJECTS = elements_.back();
		elements_.pop_back();
	}
	
	proc.repeated<HFPicture>(0, 0);
	
	count = proc.repeated<Note>(0, 0);
	while(count > 0)
	{
		m_arNote.insert(m_arNote.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	proc.repeated<PIVOTVIEW>(0, 0);
	proc.optional<DCON>		();
	
	count = proc.repeated<WINDOW>(1, 0);//??�� ������������ 1 ����
	while(count > 0)
	{
		if (elements_.back()->get_type() == typeWINDOW)
		{
			m_arWINDOW.insert(m_arWINDOW.begin(), elements_.back());
			elements_.pop_back();
		}
		count--;
	}

	proc.optional<SheetExt>(); //BulletinSearch.xls ??? ���� ��� ����

	count = proc.repeated<CUSTOMVIEW>(0, 0);
	while(count > 0)
	{
		m_arCUSTOMVIEW.insert(m_arCUSTOMVIEW.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	count = proc.repeated<SORT>(0, 2);
	while(count > 0)
	{
		m_arSORT.insert(m_arSORT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	proc.optional<DxGCol>		();
	
	count = proc.repeated<MergeCells>(0, 0);
	while(count > 0)
	{
		m_arMergeCells.insert(m_arMergeCells.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	proc.optional<LRng>			();
	proc.repeated<QUERYTABLE>	(0, 0);
	proc.optional<PHONETICINFO>	();
	
	if (proc.optional<CONDFMTS>())  // Let it be optional
	{
		m_CONDFMTS = elements_.back();
		elements_.pop_back();
	}
	
	count = proc.repeated<HLINK>(0, 0) ;
	while(count > 0)
	{
		m_arHLINK.insert(m_arHLINK.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	proc.optional<DVAL>();
	
	if (proc.optional<CodeName>	())
	{
		m_CodeName = elements_.back();
		elements_.pop_back();
	}

	proc.repeated<WebPub>	(0, 0);
	proc.repeated<CellWatch>(0, 0);
	
	if (proc.optional<SheetExt>())
	{
		m_SheetExt = elements_.back();
		elements_.pop_back();
	}

	proc.repeated<FEAT>		(0, 0);
	proc.repeated<FEAT11>	(0, 0);
	proc.repeated<RECORD12>	(0, 0);

#pragma message("####################### Some trash records may be skipped here")
	proc.SeekToEOF(); // Thus we skip problems with the trash at the end of the stream (found in Domens.xls)
	
	proc.mandatory<EOF_T>();

	global_info->customColumnsWidth.clear();
	global_info->customRowsHeight.clear();

	return true;
}


} // namespace XLS

