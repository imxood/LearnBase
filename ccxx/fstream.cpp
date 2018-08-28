#include <iostream>
#include <fstream>

// #include <easylogging++.h>
#include <utils/LogUtil.h>

// INITIALIZE_EASYLOGGINGPP

int main(int argc, char const *argv[])
{
    LogUtil log(argv[0]);

    std::fstream f("你好.txt", std::ios::out); //写,会覆盖原来的内容
    f << 123;
    f.close();

    f.open("你好.txt", std::ios::in); //读
    int i;

    f >> i;
    LOG(INFO) << i;
    f.close();

    f.open("你好.txt", std::ios::out | std::ios::app); //文件末尾写
    f << "hello，你好";
    f.close();

    //    std::ifstream in("some.file");
    //    std::istreambuf_iterator<char> beg(in), end;
    //    std::string str(beg, end);

    //    std::ifstream in("some.file");
    //    std::ostringstream tmp;
    //    tmp << in.rdbuf();
    //    std::string str = tmp.str();

    return 0;
}
