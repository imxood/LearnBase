#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

int main(int argc, char const *argv[])
{
    // struct shm_remove
    // {
    //     shm_remove()
    //     {
    //         shared_memory_object::remove("MySharedMemory");
    //     }
    //     ~shm_remove()
    //     {
    //         shared_memory_object::remove("MySharedMemory");
    //     }
    // } remover;

    /**
     * 创键共享内存
     * open_or_create,打开或创建
     * "MySharedMemory",共享内存的唯一标志
     * read_write, 可读可写
     */
    shared_memory_object shm(open_only, "MySharedMemory", read_write);
    /**
     * truncate() 函数只能在共享内存以 boost::interprocess::read_write 方式打开时调用
     * 分配1024字节内存
     */
    shm.truncate(1024);

    /** get_name() 和 get_size() 可以分别用来查询共享内存的名称和大小 */
    std::cout << shm.get_name() << std::endl;
    // std::cout << shm.get_size() << std::endl;

    /** 映射共享内存到自己的地址空间 */
    mapped_region region(shm, read_write);
    std::cout << std::hex << "0x" << region.get_address() << std::endl;
    std::cout << std::dec << region.get_size() << std::endl;

    boost::interprocess::mapped_region region2(shm, boost::interprocess::read_only);
    std::cout << std::hex << "0x" << region2.get_address() << std::endl;
    std::cout << std::dec << region2.get_size() << std::endl;

    int *i2 = static_cast<int *>(region2.get_address());
    std::cout << *i2 << std::endl;

    return 0;
}
