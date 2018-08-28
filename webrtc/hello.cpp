#include <iostream>
#include <modules/audio_processing/include/audio_processing.h>
#include <modules/audio_device/include/audio_device.h>

using namespace webrtc;

int main()
{
	rtc::scoped_refptr<AudioDeviceModule> adm_ =
	    webrtc::AudioDeviceModule::Create(webrtc::AudioDeviceModule::kPlatformDefaultAudio);

	return 0;
}