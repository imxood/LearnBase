#include <iostream>

int main(int argc, char const *argv[])
{
    int a = 1, b = 2;
    auto func = [=, &b]() mutable { a = 22; b = 333; std::cout << "a:" << a << " b:" << b << std::endl; };

    func();
    std::cout << "a:" << a << " b:" << b << std::endl;
    return 0;
}
