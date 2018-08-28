#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>

#include <utils/LogUtil.h>
#include <utils/JsonUtil.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <ctime>
#include <cstring>
#include <boost/format.hpp>

#include <openssl/md5.h>
#include <thread>

#define AUDIO_PATH "/tmp"
#define ASR_FILE_NUM 20

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency;
using namespace pplx;
using namespace chrono;

using namespace rapidjson;

#define XFYUN_ASR_URL "http://api.xfyun.cn/v1/service/v1/iat"

#define ApiKey "a0967f3c564ad1c2137e4fbd20682651"
#define AppId "5aa8a5fc"

/** 科大讯飞ASR实现 */
class XfyunASR
{
  public:
	void saveAudio(string_t &audioFile, const string_t &data)
	{
		static auto run_num = 1;

		if (run_num > 30)
			run_num = 1;

		audioFile = (boost::format("/tmp/audio_%03d.pcm") % run_num).str();

		std::fstream f(audioFile, std::ios::out); //写,会覆盖原来的内容

		f << data;

		f.close();

		// streams::fstream::open_ostream(audioFile)

		// 	.then([&data](streams::ostream ostream) {
		// 		LOG(INFO) << "开始写入";
		// 		const size_t& size = ostream.print(data).get();
		// 		ostream.close().get();
		// 		LOG(INFO) << "写入长度: " << size;
		// 	})

		// 	.wait();

		run_num++;
	}

	/** 
	 * 声音转文字
	 * outText.size()==0, 则失败
	 */
	void audio2text(const string_t &inPCMData, string_t &outText)
	{
		Document docParam;
		docParam.SetObject();
		docParam.AddMember("engine_type", "sms16k", docParam.GetAllocator()); // 16k采样率普通话音频
		docParam.AddMember("aue", "raw", docParam.GetAllocator());			  // 未压缩的pcm或wav格式

		ostringstream curTime;
		curTime << std::time(0);

		const string_t &json = JsonUtil::stringify(docParam, std::make_shared<string_t>());

		LOG(INFO) << "param: " << json;

		// json数据, base64编码
		vector<unsigned char> v1;
		v1.resize(json.size());
		v1.assign(json.begin(), json.end());

		const string_t &param = utility::conversions::to_base64(v1);

		// 计算校验码
		// unsigned char md[16] = {0};
		string_t sum = string_t(ApiKey) + curTime.str() + param;
		// auto sp = std::shared_ptr(new int[len], [](char *p) { delete[] p; });
		// auto mdStr = std::shared_ptr<unsigned char>(new unsigned char[MD5_SIZE], [](unsigned char *p) { delete[] p; });
		LOG(INFO) << "sum: " << sum;

		string md = MD5(sum.c_str(), sum.size());

		string_t audioFile;
		try
		{
			create_task([&]() -> task<http_response> {
				http_client_config cfg;
				cfg.set_timeout(std::chrono::seconds(5)); /** 配置5s超时 */

				http_client client(XFYUN_ASR_URL, cfg);

				http_request request(methods::POST);

				http_headers &headers = request.headers();
				headers.add("X-Appid", AppId);
				headers.add("X-CurTime", curTime.str()); // 当前时间戳,单位s
				headers.add("X-Param", param);
				headers.add("X-CheckSum", md);

				LOG(INFO) << "AppId: " << AppId;
				LOG(INFO) << "CurTime: " << curTime.str();
				LOG(INFO) << "Param: " << param;
				LOG(INFO) << "CheckSum: " << md;

				// saveAudio(audioFile, inPCMData); // 保存pcm文件
				// json数据, base64编码
				vector<unsigned char> v2;
				v2.resize(inPCMData.size());
				v2.assign(inPCMData.begin(), inPCMData.end());

				const string_t &base64_inPCMData = utility::conversions::to_base64(v2);

				Document data;
				data.SetObject();
				Value audio(base64_inPCMData.c_str(), base64_inPCMData.size());
				data.AddMember("audio", audio, data.GetAllocator());

				const string_t& body = JsonUtil::stringify(data, make_shared<string_t>());

				const string_t& code_body = UrlEncode(body);

				request.set_body(code_body, "application/x-www-form-urlencoded; charset=utf-8");

				return client.request(request);
			})
				.then([](http_response response) -> task<string_t> {
					if (response.status_code() == status_codes::OK)
						return response.extract_string();

					LOG(ERROR) << "科大讯飞asr请求失败";

					// const json::value &v = response.extract_json().get();

					// LOG_IF(ERROR, v.has_field("code")) << v.at("err_msg").as_string();

					cancel_current_task();
				})

				.then([=, &outText, &audioFile](task<string_t> previousTask) {
					const string_t &v = previousTask.get();
					LOG(INFO) << v;

					// if (std::strcmp(v.at("code").as_string().c_str(), "0") == 0)
					// {
					// 	outText = v.at("data").as_string();
					// 	LOG(INFO) << audioFile;
					// }
					// else
					// 	LOG(ERROR) << "code: " << v.at("code").as_string() << ", desc: " << v.at("desc").as_string();
				})

				.wait();
		}
		catch (http_exception &e)
		{
			LOG(ERROR) << "科大讯飞asr请求失败";
			LOG(INFO) << e.what();
		}
	}

	void audioFile2Text(const string_t &inPCMFile, string_t &outText)
	{
		auto iFileStream = std::make_shared<streams::istream>();

		streams::fstream::open_istream(inPCMFile)

			.then([=, &outText](streams::istream inStream) {
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

				const string_t &inPCMData = sbuffer.collection();

				audio2text(inPCMData, outText);
			})

			.wait();
	}

	static string MD5(const char *data, size_t len)
	{
		MD5_CTX ctx;

		string md5_string;
		unsigned char md[16] = {0};
		char tmp[33] = {0};

		MD5_Init(&ctx);
		MD5_Update(&ctx, data, len);
		MD5_Final(md, &ctx);

		for (int i = 0; i < 16; ++i)
		{
			memset(tmp, 0x00, sizeof(tmp));
			sprintf(tmp, "%02X", md[i]);
			md5_string += tmp;
		}
		return md5_string;
	}

	static unsigned char ToHex(unsigned char x)
	{
		return x > 9 ? x + 55 : x + 48;
	}

	static std::string UrlEncode(const std::string &str)
	{
		std::string strTemp = "";
		size_t length = str.length();
		for (size_t i = 0; i < length; i++)
		{
			if (isalnum((unsigned char)str[i]) ||
				(str[i] == '-') ||
				(str[i] == '_') ||
				(str[i] == '.') ||
				(str[i] == '~'))
				strTemp += str[i];
			else if (str[i] == ' ')
				strTemp += "+";
			else if (str[i] == '=' || str[i] == '&')
				strTemp += str[i];
			else
			{
				strTemp += '%';
				strTemp += ToHex((unsigned char)str[i] >> 4);
				strTemp += ToHex((unsigned char)str[i] % 16);
			}
		}
		return strTemp;
	}
};

int main(int argc, char **argv)
{
	LogUtil log(argv[0]);

	XfyunASR asr;

    ifstream in("/tmp/test.pcm");
    ostringstream out;
    out << in.rdbuf();

    const string_t &data = out.str();

    LOG(INFO) << asr.MD5(data.c_str(), data.size());

    // std::this_thread::sleep_for(chrono::seconds(5));
    
	return 0;
}
