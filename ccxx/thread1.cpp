#include <thread>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <unistd.h>

using namespace std;

int main()
{
    condition_variable cv;
    mutex a;

    thread t1([&] {
        while (1)
        {
            sleep(1);
            unique_lock<mutex> g(a);
            cout << "notify one" << endl;
            cv.notify_one();
        }
    });

    thread t2([&] {
        //this_thread::sleep_for(chrono::milliseconds(100));
        while (1)
        {
            unique_lock<mutex> g(a);
            cv.wait(g);
            cout << "received notification" << endl
                 << endl;
            cv.wait(g);
            cout << "received notification" << endl
                 << endl;
        }
    });
    t1.join();
    t2.join();

    return 0;
}
