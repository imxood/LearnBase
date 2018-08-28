#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <condition_variable>
#include <atomic>

#define FREEDOM_STATE_SECOND 10

using namespace std;
using namespace chrono;

bool running = true;

// 模拟定时器, FREEDOM_STATE_SECOND s 的时间后进入自由模式
class Monitor
{
  public:
	typedef enum
	{
		Normal,
		Freedom
	} State;

	Monitor() : started(false) {}

	void run()
	{
		while (running)
		{
			if (!started)
			{ // 如果监控没有被start(), 则等待
				std::unique_lock<std::mutex> loc(mutStart);
				condStart.wait(loc);
			}

			this_thread::sleep_for(chrono::seconds(1)); // 休眠1s

			endTime = system_clock::now();
			// Sleep(1);
			duration<double> duration = endTime - startTime;

			cout << duration.count() << "s" << endl;

			if (duration.count() < FREEDOM_STATE_SECOND)
			{
				continue;
			}

			// .. 进入自由模式
			enterFreedomMode();
			started = false;
		}
	}

	// 开始监控
	void start()
	{
		endTime = startTime = system_clock::now();
		started = true;

		condStart.notify_one();
		cout << "condStart.notify_one()." << endl;
	}

	// 停止监控
	void stop()
	{
		started = false;
	}

	// 进入自由模式
	void enterFreedomMode()
	{
		cout << "enterFreedomMode()." << endl;
	}

	// 进入正常模式
	void enterNormalMode()
	{
		cout << "enterNormalMode()." << endl;
	}

  private:
	time_point<system_clock> startTime;
	time_point<system_clock> endTime;

	atomic_bool started;

	mutex mutStart;
	condition_variable condStart;
};

string getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

int main(int argc, char *argv[])
{
	Monitor monitor;
	// 创建一个线程
	thread t(&Monitor::run, &monitor);

	while (running)
	{
		cout << "time: " << getTime() << endl;
		monitor.start();
		monitor.enterNormalMode();

		this_thread::sleep_for(chrono::seconds(15));

		cout << endl;
	}

	return 0;
}
