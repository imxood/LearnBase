#include <fstream>
#include <string>
#include <iostream>
#include <json/json.h>

using namespace std;

int main()
{
    ifstream ifs;
    ifs.open("/home/maxu/develop/git/StudyNote/build/test.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        return -1;
    }

    Json::Value add_value = root["address"];
    for (int i = 0; i < add_value.size(); ++i)
    {
        Json::Value temp_value = add_value[i];
        string strName = temp_value["name"].asString();
        string strMail = temp_value["email"].asString();
        cout << "name: " << strName << " email: " << strMail << endl;
    }

    string name = "马许";

    Json::Value v_root;
    v_root["name"] = name;

    Json::FastWriter writer;
    //未格式化的json
    string json_append_file = writer.write(v_root);
    //格式化之后的json,有回车换行符
	string strOut = v_root.toStyledString();

    cout << json_append_file << ", " << strOut << endl;

    ofstream ofs;
	ofs.open("test_append.json");
	ofs << json_append_file;//未格式化
	ofs << strOut;//格式化

    return 0;
}