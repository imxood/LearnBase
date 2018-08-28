#pragma once

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

struct trace_queue
{
    enum
    {
        LineSize = 6400     /** 数据长度: 320 *100*20, 一次读取320byte, 100Hz, 20s 缓存20s */
    };

    trace_queue()
        : message_in(false)
    {
    }

    //Mutex to protect access to the queue
    boost::interprocess::interprocess_mutex mutex;

    //Condition to wait when the queue is empty
    boost::interprocess::interprocess_condition cond_empty;

    //Condition to wait when the queue is full
    boost::interprocess::interprocess_condition cond_full;

    //Items to fill
    char items[LineSize];

    //Is there any message
    bool message_in;
};
