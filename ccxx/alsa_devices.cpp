#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <utils/LogUtil.h>

#define FUNC_GET_NUM_OF_DEVICE 0
#define FUNC_GET_DEVICE_NAME 1
#define FUNC_GET_DEVICE_NAME_FOR_AN_ENUM 2

static const int kAdmMaxDeviceNameSize = 128;
static const int kAdmMaxFileNameSize = 512;
static const int kAdmMaxGuidSize = 128;

static const int kAdmMinPlayoutBufferSizeMs = 10;
static const int kAdmMaxPlayoutBufferSizeMs = 250;

int32_t GetCardName(int card, char *shortName, char *longName)
{
    int status = 0;
    if ((status = snd_card_get_name(card, &shortName)) != 0)
    {
        LOG(ERROR) << "cannot determine card shortName: " << snd_strerror(status);
    }
    if ((status = snd_card_get_longname(card, &longName)) != 0)
    {
        LOG(ERROR) << "cannot determine card longName: " << snd_strerror(status);
    }
    LOG(INFO) << "snd_card shortName: " << shortName << ", "
              << "longName: " << longName;
    return status;
}

int32_t GetDevicesInfo(const int32_t function,
                       const bool playback,
                       const int32_t enumDeviceNo = 0,
                       char *enumDeviceName = NULL,
                       const int32_t ednLen = 0)
{

    // Device enumeration based on libjingle implementation
    // by Tristan Schmelcher at Google Inc.

    const char *type = playback ? "Output" : "Input";
    // dmix and dsnoop are only for playback and capture, respectively, but ALSA
    // stupidly includes them in both lists.
    const char *ignorePrefix = playback ? "dsnoop:" : "dmix:";
    // (ALSA lists many more "devices" of questionable interest, but we show them
    // just in case the weird devices may actually be desirable for some
    // users/systems.)

    int err;
    int enumCount(0);

    // From Chromium issue 95797
    // Loop through the sound cards to get Alsa device hints.
    // Don't use snd_device_name_hint(-1,..) since there is a access violation
    // inside this ALSA API with libasound.so.2.0.0.
    int card = -1;
    int status;

    char *shortName = NULL;
    char *longName = NULL;

    while (!snd_card_next(&card) && (card >= 0))
    {
        LOG(INFO) << "snd_card index: " << card;

        if (GetCardName(card, shortName, longName) == 0)
        {
            // free((void*)shortName);
            // free((void*)longName);
        }

        void **hints;
        err = snd_device_name_hint(card, "pcm", &hints);
        if (err != 0)
        {
            LOG(INFO) << "GetDevicesInfo - device name hint error: " << snd_strerror(err);
            return -1;
        }

        enumCount++; // default is 0
        if ((function == FUNC_GET_DEVICE_NAME ||
             function == FUNC_GET_DEVICE_NAME_FOR_AN_ENUM) &&
            enumDeviceNo == 0)
        {
            strcpy(enumDeviceName, "default");

            err = snd_device_name_free_hint(hints);
            if (err != 0)
            {
                LOG(INFO) << "GetDevicesInfo - device name free hint error: " << snd_strerror(err);
            }

            return 0;
        }

        for (void **list = hints; *list != NULL; ++list)
        {
            char *actualType = snd_device_name_get_hint(*list, "IOID");
            if (actualType)
            { // NULL means it's both.
                bool wrongType = (strcmp(actualType, type) != 0);
                // if (wrongType)
                // {
                //     // Wrong type of device (i.e., input vs. output).
                //     continue;
                // }
            }

            char *name = snd_device_name_get_hint(*list, "NAME");
            if (!name)
            {
                LOG(INFO) << "Device has no name";
                // Skip it.
                continue;
            }

            // Yes, we do.
            char *desc = snd_device_name_get_hint(*list, "DESC");
            if (!desc)
            {
                // Virtual devices don't necessarily have descriptions.
                // Use their names instead.
                desc = name;
            }
            

            LOG(INFO) << "    Enum device: " << enumCount << ",  device name: " << name << ", IO: " << actualType << ", desc: " << desc;

            if (actualType)
            {
                free(actualType);
            }

            if(desc){
                free(desc);
            }

            ++enumCount;

            free(name);
        }

        err = snd_device_name_free_hint(hints);
        if (err != 0)
        {
            LOG(INFO) << "GetDevicesInfo - device name free hint error: " << snd_strerror(err);
        }
    }

    return 0;
}

int16_t RecordingDevices()
{
    return (int16_t)GetDevicesInfo(0, false);
}

int32_t RecordingDeviceName(
    uint16_t index,
    char name[kAdmMaxDeviceNameSize],
    char guid[kAdmMaxGuidSize])
{

    const uint16_t nDevices(RecordingDevices());

    if ((index > (nDevices - 1)) || (name == NULL))
    {
        return -1;
    }

    memset(name, 0, kAdmMaxDeviceNameSize);

    if (guid != NULL)
    {
        memset(guid, 0, kAdmMaxGuidSize);
    }

    return GetDevicesInfo(1, false, index, name, kAdmMaxDeviceNameSize);
}

void test()
{
    char **hints;
    /* Enumerate sound devices */
    int err = snd_device_name_hint(-1, "pcm", (void ***)&hints);
    if (err != 0)
        return; //Error! Just return

    int count = 0;

    char **n = hints;
    while (*n != NULL)
    {

        char *name = snd_device_name_get_hint(*n, "NAME");
        char *actualType = snd_device_name_get_hint(*n, "IOID");
        char *desc = snd_device_name_get_hint(*n, "DESC");

        if (name != NULL && 0 != strcmp("null", name))
        {
            //Copy name to another buffer and then free it
            printf("NAME: %s, IOID: %s, DESC: %s\n", name, actualType, desc);
            count++;
            free(name);
            free(actualType);
            free(desc);
        }
        n++;
    } //End of while

    printf("device count: %d\n", count);

    //Free hint buffer too
    snd_device_name_free_hint((void **)hints);
}

int main(int argc, char const *argv[])
{
    LogUtil log(argv[0]);
    // test();
    GetDevicesInfo(0, false);
    return 0;
}
