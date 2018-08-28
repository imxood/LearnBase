#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;

void Print(const boost::system::error_code &ec)
{
    cout << "Hello World!" << endl;
    cout << boost::this_thread::get_id() << endl;
}
int main()
{
    cout << boost::this_thread::get_id() << endl;
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

    t.async_wait(Print);
    cout << "to run" << endl;
    io.run();
    cout << "exit" << endl;
    return 0;
}
