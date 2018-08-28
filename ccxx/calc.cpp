#include <iostream>
#include <stdio.h>

using namespace std;

int main(){
	extern char** environ;

	char** env = environ;

	while(*env){
		printf("%s\n", *env);
		env++;
	}

	int a = 0;
	double b = 0.0;
	a = 8.5/1.1;
	b = 8.5/1.1;
	cout << a << "," << b << endl;
	return 0;
}