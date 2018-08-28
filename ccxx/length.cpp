#include <iostream>
#include <cstdio>
#include <string.h>

using namespace std;

int main(){
    string str = "abc";
    int len = str.size();
    char* buf = new char[len+1];
    *(buf+3) = '1';
    memcpy(buf, str.c_str(), len);
    printf("text:'%s', size:'%lu',%d\n", buf, strlen(buf), buf[3]);
    return 0;
}
