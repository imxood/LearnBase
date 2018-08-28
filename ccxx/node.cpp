#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glog/logging.h>

#include <robot_speech/baidu_voice/asr.h>
#include <robot_speech/baidu_voice/tts.h>
#include <robot_speech/baidu_voice/tts.h>
#include <robot_speech/turing/dialog.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace robot;
using namespace std;



//判断指定长度的字符串是否是全数字
bool AllIsNum(const char *str, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (str[i] < 48 || str[i] > 57)
		{
			return false;
		}
	}
	return true;
}

bool get_command_type(string &str, int &cmd)
{

	if (str.size() < 5 || str[0] != '{' || str[4] != '}')
	{
		return false;
	}

	if (!AllIsNum(str.c_str() + 1, 3))
	{
		return false;
	}

	cmd = atoi(str.c_str() + 1);

	str = str.substr(5);

	return true;
}

void get_command_text(string &str, string &content)
{
	if (str.size() == 0)
	{
		content = "";
		return;
	}

	int i = 0;

	for (; i < str.size(); i++)
	{
		if (str[i] == '{')
		{
			content = str.substr(0, i);
			str = str.substr(i);
			return;
		}
	}

	content = str;
	str = "";
}

void play_music(string &wav_path)
{
	string cmd = "aplay ";
	cmd.append("\"");
	cmd.append(wav_path);
	cmd.append("\"");

	FILE *fp = popen(cmd.c_str(), "r");

	if (NULL == fp)
	{
		LOG(ERROR) << "popen执行失败！" << strerror(errno);
		exit(1);
	}

	char result_buf[1024];
	while (fgets(result_buf, sizeof(result_buf), fp) != NULL)
	{
		printf("%d  %s", getpid(), result_buf);
	}

	int status = pclose(fp);

	status == 0 ? (LOG(INFO) << __FUNCTION__ << " ok") : (LOG(WARNING) << __FUNCTION__ << " error");
}

void fork_process(){
	
}

//执行命令
void exec_command(int &cmd1, string &text1, int &cmd2, string &text2)
{
	LOG(INFO) << cmd1;
	LOG(INFO) << text1;
	switch (cmd1)
	{
	case 101:
		LOG(INFO) << text1;
		break;
	case 102:
		LOG(INFO) << text1;
		break;
	case 103:
		LOG(INFO) << text1;
		break;
	case 104:
		if (cmd2 == 102)
		{
			LOG(INFO) << cmd2;
			LOG(INFO) << text2;
		}

		break;
	case 105:
		LOG(INFO) << text1;
		break;
	case 106:
		LOG(INFO) << text1;
		break;
	case 107:
		play_music(text1);
		break;
	}
}

//解析命令
void parse_command(string &text)
{
	int cmd1 = 0, cmd2 = 0;
	string text1 = "", text2 = "";

	if (get_command_type(text, cmd1))
	{
		get_command_text(text, text1);
	}
	else
	{
		return;
	}

	if (get_command_type(text, cmd2))
	{
		get_command_text(text, text2);
	}

	exec_command(cmd1, text1, cmd2, text2);
}

int main(int argc, char **argv)
{

	google::InitGoogleLogging(argv[0]);

	FLAGS_stderrthreshold = google::INFO;

	// string text_asr;
	// string audio_file = "/home/peak/robot/develop/catkin_ws/src/robot/robot_speech/res/robot_speech/16k_test.pcm";

	// //声音转文字
	// Asr asr;
	// RETURN_CODE rescode = asr.run(audio_file, text_asr);
	// if (rescode != robot::RETURN_OK)
	// {
	// 	LOG(ERROR) << "asr.run() return " << rescode;
	// 	google::ShutdownGoogleLogging();
	// 	return 0;
	// }

	// LOG(INFO) << "text_asr: " << text_asr;

	// //语义分析
	string text_semantic;

	// robot::Dialog dialog;

	// if (dialog.run(text_asr, text_semantic))
	// {
	// 	LOG(INFO) << "解析到文字: " << text_semantic;
	// }

	// //文字转声音
	// Tts tts;

	// rescode = tts.run(text_semantic);

	// if (rescode != RETURN_OK)
	// {
	// 	LOG(ERROR) << "tts run failed: " << rescode;
	// }

	text_semantic = "{104}不能携带易燃易爆物品{102}page/loseIdentity.html";
	text_semantic = "{107}/home/peak/Music/CloudMusic/Space - Just Blue.wav";

	parse_command(text_semantic);

	google::ShutdownGoogleLogging();

	return 0;
}
