#include <iostream>
#include <utils/LogUtil.h>
#include <openssl/md5.h>

using namespace std;

string MD5(const char* data, size_t len)
{
    MD5_CTX ctx;

    string md5_string;
    unsigned char md[16] = {0};
    char tmp[33] = {0};

    MD5_Init(&ctx);
    MD5_Update(&ctx, data, len);
    MD5_Final(md, &ctx);

    LOG(INFO) << md;

    for (int i = 0; i < 16; ++i)
    {
        memset(tmp, 0x00, sizeof(tmp));
        sprintf(tmp, "%02X", md[i]);
        md5_string += tmp;
    }
    return md5_string;
}

/** 测试md5 */
void test1()
{
    uint8_t md[16] = {0};
    // unsigned char data[] = "abc";
    // MD5(data, sizeof(data) / sizeof(unsigned char), md);
    string data = "abc";
    LOG(INFO) << MD5(data.c_str(), data.size());
}

int main(int argc, char const *argv[])
{
    LogUtil log(argv[0]);

    test1();

    /* code */
    return 0;
}
