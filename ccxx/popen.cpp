#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv){

    string cmd = "ffmpeg -loglevel error -y -i /tmp/Space-JustBlue.mp3 -f wav test.wav";

    // string cmd = "pwd";

    FILE* fp = popen(cmd.c_str(), "r");

    if (NULL == fp)
	{
		printf("popen执行失败！\n");
        return -1;
	}

    char result_buf[1024];

    while (fgets(result_buf, sizeof(result_buf), fp) != NULL)
	{
		// printf("%d  %s", getpid(), result_buf);
	}

    int status = pclose(fp);

    printf("status: %d\n", status);

    if( status == 0 ){
		printf("popen ok\n");
	} else {
		printf("popen error\n");
	};

    return 0;
}
