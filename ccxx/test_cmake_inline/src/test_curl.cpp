#include <curl/curl.h>
#include <string>
#include <cstring>
#include <exception>
#include <assert.h>

class Utils
{
  public:
    static unsigned char ToHex(unsigned char x)
    {
        return x > 9 ? x + 55 : x + 48;
    }

    static unsigned char FromHex(unsigned char x)
    {
        unsigned char y;
        if (x >= 'A' && x <= 'Z')
            y = x - 'A' + 10;
        else if (x >= 'a' && x <= 'z')
            y = x - 'a' + 10;
        else if (x >= '0' && x <= '9')
            y = x - '0';
        else
            assert(0);
        return y;
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

    static std::string UrlDecode(const std::string &str)
    {
        std::string strTemp = "";
        size_t length = str.length();
        for (size_t i = 0; i < length; i++)
        {
            if (str[i] == '+')
                strTemp += ' ';
            else if (str[i] == '%')
            {
                assert(i + 2 < length);
                unsigned char high = FromHex((unsigned char)str[++i]);
                unsigned char low = FromHex((unsigned char)str[++i]);
                strTemp += high * 16 + low;
            }
            else
                strTemp += str[i];
        }
        return strTemp;
    }
};

int main(int argc, char *argv[])
{
    std::string strJson = "username=马许&password=123456";
    std::string str1 = Utils::UrlEncode(strJson);

    struct curl_slist *headers = NULL;
    CURL *pCurl = NULL;
    CURLcode res;

    try
    {

        // get a curl handle
        pCurl = curl_easy_init();
        if (NULL != pCurl)
        {
            /* set content type */
            headers = curl_slist_append(headers, "Accept: application/json");
            //headers = curl_slist_append(headers, "Content-Type: application/json;charset=UTF-8");

            curl_easy_setopt(pCurl, CURLOPT_URL, "http://127.0.0.1:8080/test");

            /* set curl options */
            curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);

            // 设置要POST的JSON数据
            curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, str1.data());

            // Perform the request, res will get the return code
            res = curl_easy_perform(pCurl);
            // Check for errors
            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));
            }

            /* cleanup curl handle */
            curl_easy_cleanup(pCurl);

            /* free headers */
            curl_slist_free_all(headers);
        }
    }
    catch (std::exception &ex)
    {
        printf("curl exception %s.\n", ex.what());
    }
    return 0;
}