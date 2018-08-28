#include <iostream>

using namespace std;

class Base
{
  public:
    virtual void print() {};
};

class Sub : public Base
{
  public:
    void print(){
        cout << "Sub.print()" << endl;
    }
};


int main(int argc, char const *argv[])
{
    Base* base = new Sub();
    base->print();
    return 0;
}
