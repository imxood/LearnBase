#include <iostream>
// #include <gflags/gflags.h>
#include <glog/logging.h>

using namespace std;

class GLogHelper
{
  public:
    GLogHelper(char *programName)
    {
        google::InitGoogleLogging(programName);
        FLAGS_stderrthreshold = google::INFO;
        FLAGS_colorlogtostderr = true;
        // FLAGS_v = 3;
    }
    ~GLogHelper()
    {
        google::ShutdownGoogleLogging();
    }
};

int main(int argc, char *argv[])
{
    // 解析命令行参数
    // gflags::ParseCommandLineFlags(&argc, &argv, true);

    GLogHelper gh(argv[0]);

    char str[20] = "hello log!";
    LOG(INFO) << "Found " << google::COUNTER << endl;
    LOG(INFO) << str;               // << " cookies";
    LOG(WARNING) << "warning test"; // 会输出一个Warning日志
    LOG(ERROR) << "error test";     // 会输出一个Error日志

    DLOG(INFO) << "Found cookies";

    VLOG(1) << "I'm printed when you run the program with --v=1 or higher";
    VLOG(2) << "I'm printed when you run the program with --v=2 or higher";

    return 0;
}
