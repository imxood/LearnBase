#include <fstream>
#include <string>
#include <iostream>
#include <json/json.h>
#include <memory>

using namespace std;

int main()
{
	Json::Value root;

    ifstream ifs;
    ifs.open(JSON_FILE_PATH);

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	JSONCPP_STRING errs;
	//文件中的json字符串解析为json对象
	if (!parseFromStream(builder, ifs, &root, &errs)) //从ifs中读取数据到root
	{
		return 1;
	}

	Json::Value add_value = root["address"];
    for (int i = 0; i < add_value.size(); ++i)
    {
        Json::Value temp_value = add_value[i];
        string strName = temp_value["name"].asString();
        string strMail = temp_value["email"].asString();
        cout << "name: " << strName << " email: " << strMail << endl;
    }

	std::cout << JSON_FILE_PATH << std::endl;

	//将json对象解析成字符串
	Json::Value v_root;
	v_root["name"] = "马许";

	Json::StreamWriterBuilder wbuilder;
	wbuilder["indentation"] = "     ";
	wbuilder["commentStyle"] = "None";

	cout << "'" << Json::writeString(wbuilder, v_root) << "'" << endl;

	//json字符串写入文件
    ofstream ofs;
	ofs.open("test_append.json");
	ofs << Json::writeString(wbuilder, v_root);

    return 0;
}