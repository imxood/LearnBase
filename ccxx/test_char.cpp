#include <iostream>
#include <string>
#include <codecvt>

using namespace std;

class Cpp11WideCharTest
{
  public:
    static void execute()
    {
        //汉字：你好啊
        const char *sz = u8"你好啊"; //Clion on mac 把这个直接转换成u8编码了, 在windowss vs上应该是GBK编码，取决于编译器和编辑器编码的设置
        char u8[] = u8"\u4f60\u597d\u554a";
        char16_t u16[] = u"\u4f60\u597d\u554a";
        char32_t u32[] = U"\u4f60\u597d\u554a";
        std::cout << "sz:" << sz <<std::endl;
        std::cout << "u8:" << u8 << std::endl;
        std::cout << "u16:" << u16 << std::endl;
        std::cout << "u32:" << u32 << std::endl;

        //两个2字节emoji字符
        char u8emoji[] = u8"\u1f4bb\u231a";     //\u形式无法输入超过2字节的unicode字符了
        char16_t u16emoji[] = u"\u1f4bb\u231a"; //用u16编码也无法正确输入超过2字符的unicode
        char32_t u32emoji[] = U"x\u231a";
        u32emoji[0] = 0x1f4bb; //笔记本电脑图标的emoji，用u32编码直接输入超过2字符的unicode字符.

        std::cout << "u8emoji:" << u8emoji << std::endl;
        std::cout << "u16emoji:" << u16emoji << std::endl;
        std::cout << "u32emoji:" << u32emoji << std::endl;

        // //utf32 to utf16
        // std::u32string u32wstr(u32emoji);
        // std::wstring_convert<std::codecvt_utf16<char32_t, 0x10ffff, std::little_endian>, char32_t> utf16le_cvt;
        // std::string stru16bytes = utf16le_cvt.to_bytes(u32wstr);
        // char16_t *p16 = (char16_t *)(stru16bytes.c_str()); //观察内存可以看到大于2字节的emoji字符，从u32转到u16，做了相应的编码

        // //u16 to u8
        // //std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;
        // std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8_ucs2_cvt;
        // std::string stru18emoji = utf8_ucs2_cvt.to_bytes(p16);
        // std::u16string stru16emoji = utf8_ucs2_cvt.from_bytes(stru18emoji);

        // //utf8 to u32
        // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> u8_u32_convert;
        // std::string u32tou8 = u8_u32_convert.to_bytes(u32emoji);             //u32 to u8
        // std::u32string stru32emoji = u8_u32_convert.from_bytes(stru18emoji); //u8 to u32

        // //uft8 to u16
        // std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u8_u16_convert;
        // std::u16string stru16emoji2 = u8_u16_convert.from_bytes(stru18emoji);

        // std::cout << "end u8u16u32" << std::endl;
    }
};


int main(int argc, char const *argv[])
{
    Cpp11WideCharTest::execute();
    return 0;
}
