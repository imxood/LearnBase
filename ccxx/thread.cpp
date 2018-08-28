#include <iostream>
#include <sstream>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <utils/LogUtil.h>

#define WORK_NUMBER 10

using namespace std;

class TaskSchedule
{

  public:
    TaskSchedule() : stoped(false), m_changed(false)
    {
        initState();

        /** 监控线程,如果m_mspState有改变则通知 */
        scanThead = new thread([=] {
            LOG(WARNING) << "监控线程开始运行..." << endl
                         << endl;

            while (!stoped)
            {
                unique_lock<mutex> lock(m_mutex);

                while (!m_changed)
                    m_cond.wait(lock);

                LOG(WARNING) << "state已改变";

                ostringstream out;

                for (int i = 0; i < WORK_NUMBER; i++)
                {
                    out << m_mspState[i] << "  ";
                }

                out << endl
                    << endl;

                LOG(WARNING) << out.str();

                m_changed = false;

                LOG(WARNING) << "监控线程正在等待..." << endl
                             << endl;
            }
            LOG(INFO) << "监控线程已退出..." << endl;
        });

        /** 工作线程 */
        m_workerThead = new thread([=] {
            LOG(INFO) << "工作线程开始运行..." << endl
                      << endl;

            while (!stoped)
            {
                this_thread::sleep_for(std::chrono::seconds(1));

                unique_lock<mutex> lock(m_mutex);

                /** 改变值 */
                int index = rand() % WORK_NUMBER;
                m_mspState[index] = !m_mspState[index];
                LOG(INFO) << "设置index为" << index << "的state: " << m_mspState[index] << endl;

                m_changed = true;

                m_cond.notify_all();
            }
            LOG(INFO) << "工作线程已退出..." << endl;
        });

        scanThead->join();
        m_workerThead->join();
    }

    ~TaskSchedule()
    {
        LOG(WARNING) << "进程停止..";
    }

    /**
     * 初始化state为false
     */
    void initState()
    {
        for (int i = 0; i < WORK_NUMBER; i++)
        {
            m_mspState[0] = false;
        }
    }

  private:
    /** m_mspState会被多线程访问，使用时加锁 */
    map<int, bool> m_mspState;
    mutex m_mutex;
    condition_variable m_cond;
    bool m_changed;

    /** 监控m_mspState的状态改变 */
    thread *scanThead;
    thread *m_workerThead;
    bool stoped;
};

int main(int argc, char const *argv[])
{

    LogUtil logUtil(argv[0]);

    TaskSchedule();

    // std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
