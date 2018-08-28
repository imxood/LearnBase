#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

// #include <easylogging++.h>
// #include <easylogging++.cc>
#include <utils/LogUtil.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <boost/format.hpp>

// INITIALIZE_EASYLOGGINGPP

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency;
using namespace pplx;

using namespace rapidjson;

#define BAIDU_TOKEN_URL "http://openapi.baidu.com/oauth/2.0/token"
#define BAIDU_TTS_URL "http://tsn.baidu.com/text2audio" //可改为https
#define BAIDU_ASR_URL "http://vop.baidu.com/server_api" // 可改为https

#define APIKEY "Qu4leXX7Lxn6eAlRieOiUjcB"
// #define APIKEY "Qu4leXX7Lxn6eAlRieOiUjcD"
#define SECRETKEY "ace8829c777735a4f27b0594cd473b52"

#define AUDIO_PATH "/tmp"

#define TTS_FILE_NUM 20

/** 请求百度token */
string_t getToken(const char *apiKey, const char *secretKey, string_t scope = "tts")
{
    static string_t token;

    if (token.size() > 0)
        return token;

    try
    {
        http_client_config cfg;
        cfg.set_timeout(std::chrono::seconds(5)); //配置5s超时

        http_client client(BAIDU_TOKEN_URL, cfg);
        uri_builder builder;

        builder.append_query("grant_type", "client_credentials"); //必须参数，固定为“client_credentials”
        builder.append_query("client_id", apiKey);                //必须参数，应用的 API Key
        builder.append_query("client_secret", secretKey);         //必须参数，应用的 Secret Key

        http_response response = client.request(methods::GET, builder.to_string()).get();

        const json::value &v = response.extract_json().get();

        if (v.has_field("error"))
        {
            LOG(ERROR) << v.at("error").as_string();
            LOG(INFO) << v.serialize();
        }
        else
        {
            LOG(INFO) << "get_token success!";
            token = v.at("access_token").as_string();
        }
    }
    catch (std::exception &e)
    {
        LOG(ERROR) << e.what();
    }
    return token;
}

/** 文字转声音 */
void text2audio(string_t &token, string_t &inText, string_t &outAudioFile)
{
    auto fileStream = std::make_shared<streams::ostream>();

    static int run_num = 1;

    if (run_num > TTS_FILE_NUM)
        run_num = 1;

    outAudioFile = (boost::format("%s/audio_%03d.mp3") % AUDIO_PATH % run_num).str();

    streams::fstream::open_ostream(outAudioFile)

        .then([=](task<streams::ostream> preTask) -> task<http_response> {
            *fileStream = preTask.get();

            http_client_config cfg;
            cfg.set_timeout(std::chrono::seconds(5)); //配置5s超时

            http_client client(BAIDU_TTS_URL, cfg);

            uri_builder builder;

            builder.append_query("tex", inText);      //合成的文本，使用UTF-8编码。小于512个中文字或者英文数字。
            builder.append_query("tok", token);       //开放平台获取到的开发者access_token
            builder.append_query("cuid", "1234567C"); //用户唯一标识，用来区分用户，计算UV值。建议填写能区分用户的机器 MAC 地址或 IMEI 码，长度为60字符以内
            builder.append_query("ctp", 1);           //客户端类型选择，web端填写固定值1
            builder.append_query("lan", "zh");        //固定值zh。语言选择,目前只有中英文混合模式，填写固定值zh

            builder.append_query("spd", 5); //语速，取值0-9，默认为5中语速
            builder.append_query("pit", 5); //音调，取值0-9，默认为5中语调
            builder.append_query("vol", 5); //音量，取值0-9，默认为5中音量
            builder.append_query("per", 4); //发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声

            return client.request(methods::GET, builder.to_string());
        })

        .then([fileStream](task<http_response> preTask) -> size_t {
            const http_response &response = preTask.get();

            if (boost::algorithm::equals(response.headers().content_type(), "audio/mp3"))
                return response.body().read_to_end((*fileStream).streambuf()).get();

            const json::value &v = response.extract_json().get();

            LOG_IF(ERROR, v.has_field("err_msg")) << v.at("err_msg").as_string();
            LOG(INFO) << v.serialize();

            return 0;
        })

        .then([fileStream, &outAudioFile](size_t size) {
            if (size == 0)
                outAudioFile = "";
            else
                run_num++;
            return fileStream->close();
        })

        .wait();
}

/** pcm声音转文字 */
void audio2text(string_t &token, string_t &inAudio, string_t &outText)
{
    auto iFileStream = std::make_shared<streams::istream>();

    static u_char run_num = 0;

    streams::fstream::open_istream(inAudio)

        .then([&token](streams::istream inStream) -> task<http_response> {
            streams::stringstreambuf sbuffer; /** 保存pcm文件数据 */

            size_t len = inStream.read_to_end(sbuffer).get(); /** 读到的字节数 */

            inStream.close();

            if (len == 0)
            {
                LOG(ERROR) << "pcm文件读取失败~";

                /** 取消当前任务，会标记当前任务已取消
                 * 由于基于值延续的继承其前面的任务的标记,延续也会立即进入已取消状态
                 */
                cancel_current_task();
            }

            http_client_config cfg;

            cfg.set_timeout(std::chrono::seconds(5)); /** 配置5s超时 */

            http_client client(BAIDU_ASR_URL, cfg);

            uri_builder builder;
            builder.append_query("token", token);
            builder.append_query("cuid", "1234567C");

            builder.append_query("dev_pid", 1536); /** 普通话(支持简单的英文识别), 识别语言及模型选择 */

            return client.request(methods::POST, builder.to_string(), U(sbuffer.collection()), "audio/pcm;rate=16000");
        })

        .then([](http_response response) -> task<json::value> {
            if (response.status_code() == status_codes::OK)
                return response.extract_json();

            const json::value &v = response.extract_json().get();

            LOG_IF(ERROR, v.has_field("err_msg")) << v.at("err_msg").as_string();
            LOG(INFO) << v.serialize();
            cancel_current_task();
        })

        .then([&outText](task<json::value> previousTask) {
            const json::value &v = previousTask.get();

            if (v.at("err_no").as_integer() == 0)
                outText = v.at("result").as_array().at(0).as_string();
            else
                LOG(ERROR) << v.at("err_msg").as_string();
        })

        .wait();
}

int main(int argc, char **argv)
{

    LogUtil log(argv[0]);

    // START_EASYLOGGINGPP(argc, argv);

    // el::Configurations conf("/home/peak/.easylogging-conf.conf");

    // el::Loggers::reconfigureLogger("default", conf);

    // el::Loggers::reconfigureAllLoggers(conf);

    string_t token = getToken(APIKEY, SECRETKEY);

    if (token.size() == 0)
        return -1;

    LOG(INFO) << token;

    string_t inText = "你好撒", outAudioFile;

    text2audio(token, inText, outAudioFile);

    LOG_IF(INFO, outAudioFile.size()) << "outAudioFile: " << outAudioFile;

    text2audio(token, inText, outAudioFile);

    LOG_IF(INFO, outAudioFile.size()) << "outAudioFile: " << outAudioFile;

    string_t outText, inAudio = "/home/peak/test/speech-demo-speech-rest-api-v1.0.0-beta/rest-api-asr/linux_c/16k_test.pcm";

    audio2text(token, inAudio, outText);

    LOG_IF(INFO, outText.size()) << "outText: " << outText.size();

    return 0;
}
