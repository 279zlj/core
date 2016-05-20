#ifndef CPDOCCORE_COLOR_UTILS_H_
#define CPDOCCORE_COLOR_UTILS_H_

namespace cpdoccore { 

// ����� ���������� ����� �� 
// Black, Green, White, Blue, Magenta, Yellow, Cyan, Red
std::wstring RGBToString(int r, int g, int b);

// ������ HEX-������ � ������
bool HEXStringToRGB(std::wstring const & str, int &r, int &g, int &b);

}

#endif
