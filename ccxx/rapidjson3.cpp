// Hello World example
// This example shows basic usage of DOM-style API.

#include <iostream>
#include <utils/LogUtil.h>
#include <utils/JsonUtil.h>

using namespace rapidjson;
using namespace std;

/** 测试Error */
void test1()
{

    const char *fileName = "/home/peak/work/robot/source/src/robot/robot_common/param/tasks.json";
    Document d;
    JsonUtil::parseFile(d, fileName);

    if (d.HasParseError())
    { /** 有错误 */
        JsonUtil::printError(d);
        return;
    }

    LOG(INFO) << "执行成功";
}

/**  */
void test2()
{

    string json = "跳舞";

    Document d;
    d.Parse(json.c_str());

    if (d.HasParseError())
    { /** 有错误 */
        JsonUtil::printError(d);
        return;
    }

    LOG(INFO) << "执行成功";
}

/** 比较中文字符 */
void test3()
{
    string json = "{\"key\": {\"key1\": \"你好\"}}";
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    { /** 有错误 */
        JsonUtil::printError(d);
        return;
    }
    LOG(INFO) << d["key"]["key1"].GetString();

    if(d["key"]["key1"] == "你好"){
        LOG(INFO) << "d[\"key\"][\"key1\"] == \"你好\"" << true;
    }else{
        LOG(INFO) << "d[\"key\"][\"key1\"] == \"你好\"" << false;
    }
}

/** 判断为空 */
void test4()
{
    string json = "{\"key\": {\"key1\": \"你好\"}}";
    Document d;
    if(d.IsNull()){
        LOG(INFO) << "is null";
    }
    d.Parse(json.c_str());
    if (d.HasParseError())
    { /** 有错误 */
        JsonUtil::printError(d);
        return;
    }
    
    LOG(INFO) << "解析成功";

    d.SetNull();

    if(d.IsNull()){
        LOG(INFO) << "after SetNull(), is null";
    }else{
        LOG(INFO) << "after SetNull(), not is null";
    }
}

int main(int argc, char *argv[])
{
    LogUtil log(argv[0]);

    test1();

    test2();

    test3();
    test4();

    return 0;
}
