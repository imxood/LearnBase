#include <iostream>

#include <ros/ros.h>
#include <std_msgs/String.h>

#include <easylogging++.h>
#include <easylogging++.cc>

#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/voice_engine/include/voe_file.h"

#define NODE_NAME "play_node"

using namespace webrtc;

INITIALIZE_EASYLOGGINGPP

/**
 * 播放音频文件时，也分为输入端和输出端。
 * 输入端：音频文件
 * 输出端：系统音频回调
 */
class Player
{
  public:
	Player()
	{
		voe = VoiceEngine::Create();
		base = VoEBase::GetInterface(voe);
		file = VoEFile::GetInterface(voe);

		base->Init();

		channel = base->CreateChannel();

		// 播放输入文件audio_long16.pcm并将其录入到audio_long16_out.pcm中
		// iRet = file->StartPlayingFileLocally(ch, "audio_long16.pcm", true);
		// iRet = file->StartRecordingPlayout(ch, "audio_long16_out.pcm");
	}

	void start(const char *audioFile)
	{
		base->StopPlayout(channel);
		file->StopPlayingFileLocally(channel);

		// 首先调用VoEFileImpl::StartPlayingFileLocally设置音频输入端，该函数里面会获取一个Channel，然后指派该Channel来播放音频文件
		// iRet = file->StartRecordingMicrophone("audio_long16_from_microphone.pcm");
		int iRet = file->StartPlayingFileLocally(channel, audioFile, false);
		// iRet = g_file->StartPlayingFileAsMicrophone(channel, "audio_long16.pcm", true, true);
		LOG(INFO) << "iRet: " << iRet;

		// 开始播放
		base->StartPlayout(channel);
		LOG(INFO) << "start play";
	}

	~Player()
	{
		base->StopPlayout(channel);
		file->StopPlayingFileLocally(channel);
		// file->StopPlayingFileAsMicrophone(channel);

		base->DeleteChannel(channel);

		base->Terminate();
		base->Release();
		file->Release();

		VoiceEngine::Delete(voe);

		LOG(INFO) << "end play";
	}

  private:
	VoiceEngine *voe = NULL;
	VoEBase *base = NULL;
	VoEFile *file = NULL;
	int channel;
};

int main(int argc, char *argv[])
{
	el::Configurations conf("/home/peak/.easylogging-conf.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	ros::init(argc, argv, NODE_NAME);
	ros::NodeHandle nh("/");

	std::string sub_topic;
	Player player;

	ros::param::param<std::string>("/topic/voice/play_in", sub_topic, "/voice/tts");

	boost::function<void(const std_msgs::String::ConstPtr &)> callback =
		[&player](const std_msgs::String::ConstPtr &msg) {
			// player.start(msg->data.c_str());
			player.start("16k_test.pcm");
		};

	auto sub = nh.subscribe(sub_topic, 1, callback);

	ros::Rate rate(10);

	while (ros::ok())
	{
		ros::spinOnce();
		rate.sleep();
	}

	return 0;
}
