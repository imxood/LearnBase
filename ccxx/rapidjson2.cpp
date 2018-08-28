// Hello World example
// This example shows basic usage of DOM-style API.

#include <iostream>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <time.h>
#include <utils/LogUtil.h>
#include <utils/JsonUtil.h>

#include <fstream>

using namespace rapidjson;
using namespace std;

int main(int argc, char *argv[])
{
    LogUtil log(argv[0]);

    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("%.24s %ld Nanoseconds\n", ctime(&ts.tv_sec), ts.tv_nsec);

    string str1 = "{\"key1\": {\"key\": \"100\"}}";

    Document doc;
    doc.Parse(str1.c_str());
    LOG(INFO) << JsonUtil::stringify(doc, make_shared<string>());

    if (doc.HasParseError())
    {
        printf("HasParseError\n");
        return 1;
    }

    if (doc.HasMember("key1"))
    {
        Value &value = doc["key1"];
        if (value.IsObject())
        {
            printf("value.IsObject: true\n");
        }
    }

    ifstream in("/home/peak/work/robot/source/src/robot/robot_common/param/tasks.json");
    ostringstream out;
    out << in.rdbuf();

    // LOG(INFO) << out.str();

    Document d;
    d.Parse(out.str().c_str());

    if (d.HasParseError())
    {
        LOG(INFO) << "解析失败: ";
        return -1;
    }

    if (d.HasMember("task_test") && d["task_test"].IsObject())
    {
        Value v(d["task_test"], d.GetAllocator());
        uint64_t nsec = (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;

        Value v1;
        v1.SetUint64(nsec);

        v.AddMember("timestamp", nsec, doc.GetAllocator());

        // LOG(INFO) << JsonUtil::stringify(v);
    }

    Value v;
    LOG(INFO) << v.IsNull();
    v.CopyFrom(d, d.GetAllocator());
    LOG(INFO) << v.IsNull();

    Document d1;
    Value v1;
    v1.CopyFrom(v, d1.GetAllocator());

    Value array;
    array.SetArray();
    array.PushBack(v1, d1.GetAllocator());

    LOG(INFO) << JsonUtil::stringify(array, make_shared<string>());

    return 0;
}
