#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include <csignal>

#include <iostream>

namespace netlib
{
class ThreadPool
{
  public:
    static bool isRunning; //线程池是否在运行

  public:
    typedef std::function<void(void)> Task;
    ThreadPool(int threadNumber);
    ~ThreadPool();

    //往任务队列里添加任务
    bool append(Task task);

    //启动线程池
    bool start(void);

    //停止线程池
    bool stop(void);

  private:
    //线程所执行的工作函数
    void threadWork(void);

    std::mutex mutex_;                                  //互斥锁
    std::condition_variable_any condition_empty_;       //当任务队列为空时的条件变量
    std::list<Task> tasks_;                             //任务队列
    int threadNumber_;                                  //线程数
    std::vector<std::shared_ptr<std::thread>> threads_; //用来保存线程对象指针
};
} // namespace netlib

using namespace netlib;

bool ThreadPool::isRunning = false;

ThreadPool::ThreadPool(int threadNumber)
    : threadNumber_(threadNumber),
      threads_(threadNumber_)
{
    isRunning = true;
}

ThreadPool::~ThreadPool()
{
    if (isRunning)
    {
        stop();
    }
}

bool ThreadPool::start(void)
{
    for (int i = 0; i < threadNumber_; i++)
    {
        threads_.push_back(std::make_shared<std::thread>(std::bind(&ThreadPool::threadWork, this))); //循环创建线程
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    printf("线程池开始运行\n");
    return true;
}

bool ThreadPool::stop(void)
{
    isRunning = false;
    
    for (auto t : threads_)
    {
        t->join(); //循环等待线程终止
    }

    std::cout << "stop()" << std::endl;

    return true;
}

bool ThreadPool::append(Task task)
{
    std::lock_guard<std::mutex> guard(mutex_);
    tasks_.push_front(task);       //将该任务加入任务队列
    condition_empty_.notify_one(); //唤醒某个线程来执行此任务
    return true;
}

void ThreadPool::threadWork(void)
{
    Task task = NULL;
    while (isRunning)
    {
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (tasks_.empty())
            {
                condition_empty_.wait(mutex_); //等待有任务到来被唤醒
            }
            if (!tasks_.empty())
            {
                task = tasks_.front(); //从任务队列中获取最开始任务
                tasks_.pop_front();    //将取走的任务弹出任务队列
            }
            else
            {
                continue;
            }
        }
        task(); //执行任务
    }
}

void fun(std::string s)
{
    std::cout << "thread id: " << std::this_thread::get_id() << ", hello, " << s << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

using namespace std;

int main(int argc, char **argv)
{
    signal(SIGINT, [](int i) {
        ThreadPool::isRunning = false;
        std::cout << "run SIGINT" <<std::endl;
    });
    netlib::ThreadPool pool(2);
    pool.start();

    pool.append(std::bind(fun, std::string("小A")));
    pool.append(std::bind(fun, std::string("小B")));
    pool.append(std::bind(fun, std::string("小C")));
    pool.append(std::bind(fun, std::string("小D")));
    pool.append(std::bind(fun, std::string("小E")));
    pool.append(std::bind(fun, std::string("小F")));
    pool.append(std::bind(fun, std::string("小G")));

    while (ThreadPool::isRunning)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }
    pool.stop();
    return 0;
}
