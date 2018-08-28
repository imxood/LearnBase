#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
    auto time_now = std::chrono::system_clock::now().time_since_epoch();
    long timestamp = chrono::duration_cast<chrono::milliseconds>(time_now).count();
    cout << "current timestamp: " << timestamp;

    this_thread::sleep_for(chrono::seconds(5));

    time_now = std::chrono::system_clock::now().time_since_epoch();
    cout << "5s: " << chrono::duration_cast<chrono::milliseconds>(time_now).count() - timestamp;
    return 0;
}
