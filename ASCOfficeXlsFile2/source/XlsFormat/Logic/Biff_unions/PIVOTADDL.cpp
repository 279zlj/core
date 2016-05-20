
#include "PIVOTADDL.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCALCMEMBER.h>
#include <Logic/Biff_unions/SXADDLHIERARCHY.h>
#include <Logic/Biff_unions/SXADDLFIELD.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>
#include <Logic/Biff_unions/SXADDLCONDFMTS.h>
#include <Logic/Biff_unions/SXADDLSXFILTERS12.h>

namespace XLS
{;


PIVOTADDL::PIVOTADDL()
{
}


PIVOTADDL::~PIVOTADDL()
{
}
/*
int b = 5;
boost::shared_ptr<int> by;

void raw_ptr(const int* ptr) // ��������� �� ����������� ������.
{
	ptr = &b;  // ����������, ������ ��� ��������� �� �����������
	*ptr = 4;  // �� ����������, ������ ��� ������ �����������
}
// �� �� ����� ��� shared_ptr
void raw_ptr(boost::shared_ptr<const int> ptr) // ��������� �� ����������� ������.
{
	ptr = by;  // ����������, ������ ��� ��������� �� �����������
	*ptr = 4;  // �� ����������, ������ ��� ������ �����������
}
// ===================== ������ ��������
void raw_ptr2(int* const ptr) // ����������� ��������� �� ������������� ������.
{
	ptr = &b;  // �� ����������, ������ ��� ��������� - �����������
	*ptr = 4;  // ����������, ������ ��� ������ �� �����������
}
// �� �� ����� ��� shared_ptr
void raw_ptr2(const boost::shared_ptr<int> ptr) // ����������� ��������� �� �� ����������� ������.
{
	ptr = by;  // �� ����������, ������ ��� ��������� - �����������
	*ptr = 4;  // ����������, ������ ��� ������ �� �����������
}
*/


BaseObjectPtr PIVOTADDL::clone()
{
	return BaseObjectPtr(new PIVOTADDL(*this));
}


/*
PIVOTADDL = SXAddl_SXCView_SXDId *Continue_SxaddlSxString [SXAddl_SXCView_SXDVer10Info] 
			[SXAddl_SXCView_SXDVer12Info] *SXADDLCALCMEMBER *SXADDLHIERARCHY *SXADDLFIELD *UNKNOWNFRT 
			[SXAddl_SXCView_SXDTableStyleClient] [SXAddl_SXCView_SXDCompactRwHdr *Continue_SxaddlSxString] 
			[SXAddl_SXCView_SXDCompactColHdr *Continue_SxaddlSxString] [SXAddl_SXCView_SXDVerUpdInv] 
			[SXADDLCONDFMTS] [SXADDLSXFILTERS12] *SXAddl_SXCView_SXDVerUpdInv *SXAddl_SXCView_SXDSXPIIvmb 
			[SXAddl_SXCView_SXDVerUpdInv] SXAddl_SXCView_SXDEnd
*/
const bool PIVOTADDL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCView_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVer10Info>();
	proc.optional<SXAddl_SXCView_SXDVer12Info>();
	proc.repeated<SXADDLCALCMEMBER>(0, 0);
	proc.repeated<SXADDLHIERARCHY>(0, 0);
	proc.repeated<SXADDLFIELD>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.optional<SXAddl_SXCView_SXDTableStyleClient>();

	if(proc.optional<SXAddl_SXCView_SXDCompactRwHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	if(proc.optional<SXAddl_SXCView_SXDCompactColHdr>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.optional<SXADDLCONDFMTS>();
	proc.optional<SXADDLSXFILTERS12>();
	proc.repeated<SXAddl_SXCView_SXDVerUpdInv>(0, 0);
	proc.repeated<SXAddl_SXCView_SXDSXPIIvmb>(0, 0);
	proc.optional<SXAddl_SXCView_SXDVerUpdInv>();
	proc.mandatory<SXAddl_SXCView_SXDEnd>();
	return true;
}

} // namespace XLS

