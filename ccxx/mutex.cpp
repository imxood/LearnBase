#include <iostream>
#include <mutex>
#include <thread>
#include <utils/LogUtil.h>
#include <condition_variable>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    LogUtil log(argv[0]);

    mutex m;
    condition_variable cond;

    thread t1([&] {
        LOG(INFO) << "开始运行";
        unique_lock<mutex> lok(m);
        LOG(INFO) << "取到锁";
        this_thread::sleep_for(chrono::seconds(5));
        LOG(INFO) << "...5s";
    });

    thread t2([&] {
        LOG(INFO) << "开始运行";
        unique_lock<mutex> lok(m);
        LOG(INFO) << "取到锁";
        this_thread::sleep_for(chrono::seconds(3));
        LOG(INFO) << "...3s";
    });

    t1.join();
    t2.join();

    return 0;
}
