#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <utils/LogUtil.h>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

/** 测试base64编码 */
void test_base64(){
    std::vector<unsigned char> v = {'1', '2'};
    const string_t &_inPCMData = utility::conversions::to_base64(v);
    LOG(INFO) << _inPCMData;
}

void TestRequest1()
{
    auto fileStream = std::make_shared<concurrency::streams::ostream>();
    concurrency::streams::ostream outFile = concurrency::streams::fstream::open_ostream(U("result11.html")).get();
    *fileStream = outFile;

    http_client client(U("http://www.bing.com/"));
    uri_builder builder(U("/search"));
    builder.append_query(U("q"), U("Casablanca CodePlex"));

    http_response response = client.request(methods::GET, builder.to_string()).get();
    response.body().read_to_end(fileStream->streambuf()).get();

    // LOG(INFO) << response.to_string();
    // LOG(INFO) << response.extract_string().get();
    fileStream->close().get();
}

void TestRequest2()
{
    const string_t searchTerm = "你好";      //args[1];
    const string_t outputFileName = "a.txt"; //args[2];

    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();

    uint16_t code;

    file_buffer<uint8_t>::open(outputFileName, std::ios::out)
        /** lambda表达式: 
         * 捕获外部变量的方式: 值捕获
         * 参数列表: streambuf<uint8_t> outFile
         * 返回值类型: pplx::task<http_response>
         * 函数主体: {}
         */
        .then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response> {
            *fileBuffer = outFile;

            http_client_config cfg;
            cfg.set_timeout(std::chrono::seconds(5)); //配置5s超时
            cfg.set_validate_certificates(false);     //不验证证书

            http_client client(U("http://baidu.com/"), cfg);

            uri_builder builder(U("/s"));
            builder.append_query(U("wd"), searchTerm);

            return client.request(methods::GET, builder.to_string());
        })

        .then([&](http_response response) -> pplx::task<size_t> {
            pplx::task<size_t> len;

            LOG(INFO) << "Server returned returned status code " << response.status_code();
            LOG(INFO) << "Content type: " << response.headers().content_type();
            LOG(INFO) << "Content length: " << response.headers().content_length() << " bytes";

            code = response.status_code();

            if (code == 200)
            {
                LOG(ERROR) << "请求成功 " << response.to_string();
                len = response.body().read_to_end(*fileBuffer);
            }
            else
            {
                LOG(ERROR) << "请求失败 " << response.to_string();

                stringstreambuf sbuffer;
                len = response.body().read_to_end(sbuffer);

                LOG(INFO) << "Response body: " << sbuffer.collection();
            }

            return len;
        })

        // Close the file buffer.
        .then([=](size_t) {
            return fileBuffer->close();
        })

        // Wait for the entire response body to be written into the file.
        .wait();
}

/** streambuf 文件读写 */
void test_streambuf()
{
    auto fileStream = std::make_shared<ostream>();//---basic_ostream
    //auto fileStream = std::make_shared<istream>();---basic_istream

    // Open stream to output file.
    pplx::task<void> requestTask =
        fstream::open_ostream(U("results.html"))
            .then([=](ostream outFile) {
                *fileStream = outFile;
                return outFile.print("你好").get();
            })

            // Close the file stream.
            .then([=] (size_t size) {
                LOG(INFO) << "来过";
                return fileStream->close();
            });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }
}

int main(int argc, char *args[])
{
    LogUtil log(args[0]);
    
    TestRequest1();
    TestRequest2();

    test_streambuf();

    test_base64();

    return 0;
}
