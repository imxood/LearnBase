#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(){

	string buf = "{01}{left 1}";

	//此处%s会截取后面的直到空格或换行
	const char *format = "{%d}{%s %d}";

	int num1 = 0, num2 = 0;

	char action[10] = {'\0'};

	sscanf(buf.c_str(), format, &num1, action, &num2);

	cout << "num1: " << num1 << ", action: " << action << ", num2: " << num2 << ", test: " << buf.substr(1,2) << endl;

	return 0;
}