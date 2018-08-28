#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

static const int kTextSize = 10;

std::wstring s2ws(const std::string &str)
{
    if (str.empty())
    {
        return L"";
    }
    unsigned len = str.size() + 1;
    setlocale(LC_CTYPE, "");
    wchar_t *p = new wchar_t[len];
    mbstowcs(p, str.c_str(), len);
    std::wstring w_str(p);
    delete[] p;
    return w_str;
}

std::string ws2s(const std::wstring &w_str)
{
    if (w_str.empty())
    {
        return "";
    }
    unsigned len = w_str.size() * 4 + 1;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p, w_str.c_str(), len);
    std::string str(p);
    delete[] p;
    return str;
}

bool FormatText(std::string *txt)
{
    if (NULL == txt)
    {
        return false;
    }
    std::cout << "before:" << *txt << std::endl;
    std::wstring w_txt = s2ws(*txt);
    std::cout << "wstring size:" << w_txt.size() << std::endl;
    std::cout << "string size:" << (*txt).size() << std::endl;
    if (w_txt.size() > kTextSize)
    {
        w_txt = w_txt.substr(0, kTextSize);
        *txt = ws2s(w_txt);
        *txt += "...";
    }
    std::cout << "after:" << *txt << std::endl;
    return true;
}

int main()
{
    assert(L"" == s2ws(""));

    std::string txt = "龙之谷app好玩等你";
    assert(24 == txt.size());
    std::wstring w_txt = s2ws(txt);
    assert(10 == w_txt.size());

    assert("" == ws2s(L""));

    w_txt = L"龙之谷app好玩等你";
    assert(10 == w_txt.size());
    txt = ws2s(w_txt);
    assert(24 == txt.size());

    txt = "龙之谷app公测开启";
    std::string format_txt = txt;
    FormatText(&format_txt);
    assert(txt == format_txt);

    txt = "龙之谷app公测火爆开启";
    FormatText(&txt);
    format_txt = "龙之谷app公测火爆...";
    assert(format_txt == txt);

    return 0;
}