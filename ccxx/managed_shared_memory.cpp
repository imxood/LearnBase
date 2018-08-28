#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

using namespace boost::interprocess;

int main(int argc, char const *argv[])
{
    shared_memory_object::remove("Managed_shared_memory");
    /** 创键一个1024字节的共享内存 */
    managed_shared_memory managed_shm(open_or_create, "Managed_shared_memory", 1024);

    /** 创键一个名叫Integer的int类型对象，并设置初始值99 */
    int *i = managed_shm.construct<int>("Integer1")(99);

    /**
     * 所有元素都被初始化为99
     * construct(),如果已经存在则返回0
     */
    int *j = managed_shm.construct<int>("Integer2")[128](98);

    std::cout << *i << std::endl;

    /** find根据名称查找对象 */
    std::pair<int *, std::size_t> p = managed_shm.find<int>("Integer1");
    if (p.first)
    {
        std::cout << *p.first << std::endl; //地址
        std::cout << p.second << std::endl; //长度
    }
    int *q = managed_shm.find_or_construct<int>("Integer2")(98);
    std::cout << *q << std::endl;

    /** 如果被找到总是被删除, 返回false表示给定名称的对象没有找到 */
    managed_shm.destroy<int>("Integer1");
    managed_shm.destroy<int>("Integer2");
    // managed_shm.delete_ptr();

    // typedef boost::interprocess::allocator<char, managed_shared_memory::segment_manager> CharAllocator;
    // typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> string;

    typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> string;

    string *s = managed_shm.find_or_construct<string>("String")("Hello!", managed_shm.get_segment_manager());
    s->insert(5, ", world!");

    std::cout << *s << std::endl;

    return 0;
}
