#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace oox {

class rels;

class mediaitems
{
public:
    enum Type { typeUnknown = 0, typeImage, typeChart, typeShape, typeTable, typeHyperlink, typeComment};

    mediaitems(const std::wstring & odfPacket) : odf_packet_(odfPacket)
    {
		count_charts	=0;
 		count_shape		=0;
 		count_image		=0;
 		count_tables	=0;
	}

    struct item 
    {
        item(
            std::wstring const & _href,
            Type _type,
             std::wstring const & _outputName,
            bool _mediaInternal,
			std::wstring const & _Id);

        std::wstring href;
        Type type;
        std::wstring outputName;
        bool mediaInternal;
        bool valid;
		std::wstring  Id;
		bool used_rels;
    };
    typedef std::vector< item > items_array;
	
	size_t count_charts;
	size_t count_image;
	size_t count_shape;
	size_t count_tables;

    std::wstring add_or_find(const std::wstring & href, Type type, bool & isInternal);//�������� ������ �� ���� � ��� �� ������
    std::wstring add_or_find(const std::wstring & href, Type type, bool & isInternal, std::wstring & ref);
    
	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

private:
    items_array items_;
    std::wstring odf_packet_;

};

}
}

