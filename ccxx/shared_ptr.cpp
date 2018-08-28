#include <iostream>
#include <memory>
#include <fstream>

#include <utils/LogUtil.h>
#include <utils/JsonUtil.h>

using namespace std;
using namespace rapidjson;

enum _RunState
{
    SUCCESS = 0,
    IDLE,
    BUSY,
    RUNNING,
    FAILED,
    TIMEOUT
};

typedef _RunState RunState;

/** 设备状态 */
struct _DeviceState
{
    RunState state; /** 设备运行状态 */
    float percent;  /** 任务完成度 */

    uint64_t time_start;   /** 开始时间 */
    uint64_t time_feeback; /** 反馈时间 */

    _DeviceState() : state(IDLE), percent(0.0), time_start(0), time_feeback(0)
    {
        LOG(INFO) << this;
    }

    ~_DeviceState()
    {
        LOG(INFO) << "~";
    }

    _DeviceState &operator=(const _DeviceState &s)
    {
        state = s.state;
        percent = s.percent;
        time_start = s.time_start;
        time_feeback = s.time_feeback;
        return *this;
    }

    /** 序列化 */
    const char *stringify(shared_ptr<string> ptr)
    {
        Document doc;
        Document::AllocatorType& allocator = doc.GetAllocator();
        doc.SetObject();
        doc.AddMember("state", static_cast<int>(state), allocator);
        doc.AddMember("percent", time_start, allocator);
        doc.AddMember("time_start", time_start, allocator);
        doc.AddMember("time_feeback", time_feeback, allocator);

        return (JsonUtil::stringify(doc, ptr)).c_str();
    }

    /** 解析json字符串到对象中 */
    bool parse(const char *json)
    {
        Document doc;
        doc.Parse(json);
        if (!doc.IsObject())
        {
            LOG(WARNING) << "数据格式错误: 不是正确的json对象{}格式";
            return false;
        }
        else if (doc.HasMember("state") && doc["state"].IsInt() &&
                 doc.HasMember("percent") && doc["percent"].IsFloat() &&
                 doc.HasMember("time_start") && doc["time_start"].IsUint64() &&
                 doc.HasMember("time_feeback") && doc["time_feeback"].IsUint64())
        {
            state = static_cast<RunState>(doc["state"].GetInt());
            percent = doc["percent"].GetFloat();
            time_start = doc["time_start"].GetUint64();
            time_feeback = doc["time_feeback"].GetUint64();
            return true;
        }
        else
        {
            LOG(WARNING) << "数据格式错误: 请确定json数据是{}类型, 且有正确的key与正确类型的value";
            return false;
        }
    }
};

typedef struct _DeviceState DeviceState;

void test_shared_ptr(shared_ptr<DeviceState> deviceStatePtr)
{
    LOG(INFO) << deviceStatePtr;
}

int main(int argc, char const *argv[])
{
    LogUtil log(argv[0]);

    ifstream in("/home/peak/work/robot/source/src/robot/robot_common/param/actions.json");
    ostringstream out;
    out << in.rdbuf();

    Document d;

    d.SetObject();
    d.AddMember("a", 1, d.GetAllocator());

    d.Parse(out.str().c_str());
    if (d.HasParseError())
    {
        printf("HasParseError\n");
        return 1;
    }

    LOG(INFO) << JsonUtil::stringify(d, make_shared<string>());

    shared_ptr<DeviceState> ptr = make_shared<DeviceState>();
    test_shared_ptr(ptr);
    LOG(INFO) << ptr->stringify(make_shared<string>());

    LOG(INFO) << "hello";

    return 0;
}
