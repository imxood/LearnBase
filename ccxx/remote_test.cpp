#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
	static int num = 0;
	while(1){
		num++;
		cout << "num:" << num << endl;
		sleep(1);
	}
	return 0;
}
