#include <pplx/pplxtasks.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <utils/LogUtil.h>

using namespace concurrency;
using namespace std;

using namespace pplx;

void test(int &i)
{
    auto t = create_task([&] {
        std::this_thread::sleep_for(std::chrono::seconds(3));/** 延时3s */
        i = 5;
        LOG(INFO) << "threadid: " << this_thread::get_id() << ", i: " << i;

        LOG(INFO) << "i: " << &i;
    });
}

int main(int argc, char** argv)
{
    LogUtil log(argv[0]);

    int i=0;

    LOG(INFO) << "i: " << &i;

    test(i);

    std::this_thread::sleep_for(std::chrono::seconds(5));/** 延时5s */

    LOG(INFO) << "i: " << &i;
    

    // Create a task that throws.
    auto t = create_task([]() -> int {
        throw exception();
        return 42;
    });

    auto c1 = t.then([](int n) {
        wcout << L"Received " << n << L'.' << endl;
    });

    auto c2 = t.then([](task<int> previousTask) {
        try
        {
            wcout << L"Received " << previousTask.get() << L'.' << endl;
        }
        catch (const exception &e)
        {
            wcout << L"Caught exception from previous task." << endl;
        }
    });

    // Wait for the continuations to finish.
    try
    {
        wcout << L"Waiting for tasks to finish..." << endl;
        (c1 && c2).wait();
    }
    catch (const exception &e)
    {
        wcout << L"Caught exception while waiting for all tasks to finish." << endl;
    }
    return 0;
}
