#include <iostream>
#include <codecvt>
#include <cstring>
#include <locale>

//把字符串转换成
const std::wstring s2ws(const std::string &s)

{

    std::locale old_loc = std::locale::global(std::locale(""));

    const char *src_str = s.c_str();

    const size_t buffer_size = s.size() + 1;

    wchar_t *dst_wstr = new wchar_t[buffer_size];

    wmemset(dst_wstr, 0, buffer_size);

    mbstowcs(dst_wstr, src_str, buffer_size);

    std::wstring result = dst_wstr;

    delete[] dst_wstr;

    std::locale::global(old_loc);

    return result;
}

const std::string ws2s(const std::wstring &ws)

{

    std::locale old_loc = std::locale::global(std::locale(""));

    const wchar_t *src_wstr = ws.c_str();

    size_t buffer_size = ws.size() * 4 + 1;

    char *dst_str = new char[buffer_size];

    memset(dst_str, 0, buffer_size);

    wcstombs(dst_str, src_wstr, buffer_size);

    std::string result = dst_str;

    delete[] dst_str;

    std::locale::global(old_loc);

    return result;
}

// std::wstring s2ws(const std::string &str)
// {
//     using convert_typeX = std::codecvt_utf8<wchar_t>;
//     std::wstring_convert<convert_typeX, wchar_t> converterX;

//     return converterX.from_bytes(str);
// }

// std::string ws2s(const std::wstring &wstr)
// {
//     using convert_typeX = std::codecvt_utf8<wchar_t>;
//     std::wstring_convert<convert_typeX, wchar_t> converterX;

//     return converterX.to_bytes(wstr);
// }

int main()
{

    std::wstring wstr = L"{\"name\":\"马许\"}";

    std::string str = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);

    //std::cout << "str.data():" << str.data() << ", wstr.data():" << wstr.c_str() << ", ws2s(ws):" << ws2s(wstr) << std::endl;

    printf("%s\n", str.c_str());

    return 0;
}
//编译: g++ -g -std=c++11 wstring_convert1.cpp