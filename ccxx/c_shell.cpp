/*c执行shell*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

int main()
{

	char result_buf[MAXLINE], command[MAXLINE];

	int rc = 0; // 用于接收命令返回值

	FILE *fp;

	/*将要执行的命令写入buf*/
	snprintf(command, sizeof(command), "ls aaaa.txt 2>&1");

	/*执行预先设定的命令，并读出该命令的标准输出*/
	fp = popen(command, "r");

	if (NULL == fp)
	{
		perror("popen执行失败！");
		exit(1);
	}

	printf("parent process output:\n");

	while (fgets(result_buf, sizeof(result_buf), fp) != NULL)
	{
		printf("%d  %s", getpid(), result_buf);
	}

	/*等待命令执行完毕并关闭管道及文件指针*/
	rc = pclose(fp);

	if (-1 == rc)
	{
		perror("关闭文件指针失败");
		exit(1);
	}
	else
	{
		printf("命令【%s】子进程结束状态【%d】命令返回值【%d】\r\n", command, rc, WEXITSTATUS(rc));
	}
	return 0;
}
