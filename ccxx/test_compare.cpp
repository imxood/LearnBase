#include <iostream>

using namespace std;

int main()
{

    string str1("green apple");
    string str2("red apple");
    string str3("apple");

    if (str3.compare("apple") == 0)
        cout << str3 << " is an apple!" << endl;

    if (str1.compare(str2) != 0)
        cout << str1 << " is not " << str2 << endl;

    if (str1.compare(6, 5, "apple") == 0)
        cout << "still, " << str1 << " is an apple!" << endl;

    if (str2.compare(str2.size() - 5, 5, "apple") == 0)
        cout << "and " << str2 << " is also an apple!" << endl;

    if (str1.compare(6, 5, str2, 4, 5) == 0)
        cout << "therefore, both are apples!" << endl;

    return 0;
}