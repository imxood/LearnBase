#include <ace/OS.h>
#include <ace/Reactor.h>
#include <iostream>
#include <robot_common/LogHelper.h>

class MyTimerHandler : public ACE_Event_Handler
{

  public:
    MyTimerHandler(int delay, int inteval) : stopped(false)
    {
        //注册回调器
        this->reactor(ACE_Reactor::instance());
        StartTimer(delay, inteval);
    }

    void StartTimer(const int &delay, const int &inteval)
    {
        //设置回调器多长时间回调一次
        time_handle_ = this->reactor()->schedule_timer(this,
                                                       0, //传递handle_timeout给的参数
                                                       ACE_Time_Value(delay),
                                                       ACE_Time_Value(inteval));
    }

    //定时器回调函数
    int handle_timeout(const ACE_Time_Value &t_time, const void *act /* = 0 */)
    {
        LOG(INFO) << "time out :: handle_timeout() ";
        this->reactor()->cancel_timer(this);
        this->reactor()->end_reactor_event_loop(); // 停止消息循环
        return 0;
    }

  private:
    long time_handle_;
    bool stopped;
};

int main(int argc, char *argv[])
{
    LogHelper log(argv[0]);

    LOG(INFO) << "hello, world";

    //创建一个定时器
    MyTimerHandler mt(1, 0);
    // while(!stopped){
    ACE_Reactor::instance()->run_reactor_event_loop();
    // }
    //捕捉定时器到时事件，到时后，执行计时器的handle_timeout()方法

    LOG(INFO) << "end, world.";

    return 0;
}
