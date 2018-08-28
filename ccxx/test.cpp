#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <ctime>

#include <utils/LogUtil.h>

using namespace std;

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

class A
{
  public:
    static void test1() { printf("test\n"); };

    typedef void (*PFunc)();
};

A::PFunc f = &(A::test1);

void test_array_len(char buf[]){
    // printf("char buf[]: %d\n", ARRAY_LEN(buf));
}

int main(int argc, char const *argv[])
{
    LogUtil log(argv[0]);

    /** 测试char*转string */
    const char *abc = "abc";
    string buf(abc, abc + strlen(abc));

    std::cout << "abc: " << abc << ", " << buf << ", " << buf.size() << endl;
    // abc: abc, abc, 3


    /** 测试std::move() */

    std::string str = "Hello";
    std::vector<std::string> v;
    //调用常规的拷贝构造函数，新建字符数组，拷贝数据
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
    //调用移动构造函数，掏空str，掏空后，最好不要使用str
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0]
              << "\", \"" << v[1] << "\"\n";
    // After copy, str is "Hello"
    // After move, str is ""
    // The contents of the vector are "Hello", "Hello"


    /** 测试结构体赋值 */

    typedef struct Struct_T
    {
        int a;
        int b;
    } Struct_T;

    Struct_T d;

    d.a = 123;

    d.b = 456;

    Struct_T e;
    e = d;

    cout << e.a << "," << e.b << endl;
    // 123,456


    /** 测试正数与小数的比较 */
    int a = 1;
    printf("1equal1.0: %d\n", 1 == 1.0);
    // 1equal1.0: 1

    /** test memcpy size */
    const char *str1 = "abc";
    char *str2 = new char[3];
    memcpy(str2, str1, 3);

    std::cout << str1 << ", strlen(str1): " << strlen(str1) << std::endl;
    std::cout << str2 << ", strlen(str2): " << strlen(str2) << std::endl;
    // abc, strlen(str1): 3
    // abc, strlen(str2): 3

    // 函数指针
    (*f)();

    /** ctime时间测试 */
    std::time_t t = std::time(0);
    printf("%ld seconds since 01-Jan-1970\n", t);
    // 1531621646 seconds since 01-Jan-1970

    /** unsigned char *指针 输出 */
    uint8_t buf1[16] = {0};
    memcpy(buf1, "abc", 3);

    ostringstream o;
    o << buf1;

    LOG(INFO) << "buf1: " << buf1 << ", o.str(): " << o.str() << ", strlen(buf1): " << strlen((char*)buf1);
    // abc


    const char haystack[20] = "RUNOOB";
    const char needle[10] = "R";
    const char *ret;
    ret = strstr(haystack, needle);
    printf("子字符串是： %s\n", ret);
    // 子字符串是： RUNOOB

    int pos = strncmp(haystack, needle, strlen(needle));
    printf("pos: %d\n", pos);
    // pos: 0

    char buf2[] = {'a', 'b', 'c'};

    char* buf3 = buf2;

    int len2 = ARRAY_LEN(buf2);
    int len3 = ARRAY_LEN(buf3);

    printf("len2: %d, len3: %d\n", len2, len3);

    test_array_len(buf2);

    float b = -2.0*60/0.5338 + 0.5;
    int b1 = b;
    unsigned short b2 = b;
    int b3 = b2;

    printf("b: %f, b1: %x, b2: %x, b3: %d\n", b, b1, b2, b3);

    return 0;
}
