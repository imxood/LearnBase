#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;

class CPrinter
{
  public:
    CPrinter(boost::asio::io_service &io)
        : m_strand(io), m_timer1(io, boost::posix_time::seconds(5)), m_timer2(io, boost::posix_time::seconds(5)), m_count(0)
    {
        m_timer1.async_wait(m_strand.wrap(boost::bind(&CPrinter::Print1, this)));
        m_timer2.async_wait(m_strand.wrap(boost::bind(&CPrinter::Print2, this)));
    }
    ~CPrinter()
    {
        cout << "m_count = " << m_count << endl;
    }

    void Print1()
    {
        if (m_count < 10)
        {
            cout << "Timer1 count = " << m_count << endl;
            cout << boost::this_thread::get_id() << endl;
            m_count++;

            m_timer1.expires_at(m_timer1.expires_at() + boost::posix_time::seconds(1));

            m_timer1.async_wait(m_strand.wrap(boost::bind(&CPrinter::Print1, this)));
        }
    }

    void Print2()
    {
        if (m_count < 10)
        {
            cout << "Timer2 count = " << m_count << endl;
            cout << boost::this_thread::get_id() << endl;
            m_count++;

            m_timer2.expires_at(m_timer2.expires_at() + boost::posix_time::seconds(1));

            m_timer2.async_wait(m_strand.wrap(boost::bind(&CPrinter::Print2, this)));
        }
    }

  private:
    boost::asio::strand m_strand;
    boost::asio::deadline_timer m_timer1;
    boost::asio::deadline_timer m_timer2;
    int m_count;
};

int main()
{
    cout << boost::this_thread::get_id() << endl;
    boost::asio::io_service io;
    CPrinter cp(io);

    cout << "to run" << endl;

    boost::thread td(boost::bind(&boost::asio::io_service::run, &io));

    io.run();

    cout << "exit" << endl;
    return 0;
}
