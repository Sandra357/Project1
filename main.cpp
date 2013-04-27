#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "mongoose.h"

using namespace std;


string GetParam(string str , string param)
{
    size_t num = str.find(param);
    if (num == string::npos)
        return "";
    size_t k = num + param.size() + 1;
    string newStr = str.substr(k , str.size());
   // cout << newStr << "\n";
    size_t n = newStr.find('&');
    if (n != string::npos)
    {
     //   cout << n << "\n";
        newStr = newStr.substr(0, n);
    }
     return newStr;
}

string GetType(string action)
{
    size_t pos = action.find('.');
    if (pos == string::npos)
        return "text/plain";
    string type = action.substr(pos+1);
    if (type == "ico")
        return "image/" + type;
    else
        return "text/" + type;
}



// Функция, принимающая имя файла и возвращающее его содержимое
std::string ReadFromFile(const std::string& fileName) {
    ifstream MyFile(fileName.c_str());

    if (!MyFile.is_open()) {
        return "Failed to open file!";
    }

    std::string result;

    while (!MyFile.eof())
    {
        std::string tmp;
        getline(MyFile, tmp);
        result += tmp + "\n";
    }

    return result;
}

string GetHeader (string type)
{
    string newStr = "HTTP/1.0 200 OK\r\n"
            "Content-Type: " + type + "\r\n\r\n";
    return newStr;
}

static int begin_request_handler(struct mg_connection *conn) {
    const struct mg_request_info *ri = mg_get_request_info(conn);

    //string header = "HTTP/1.0 200 OK\r\n"
                    "Content-Type: text/html\r\n\r\n";

    // http://localhost:8080/asdfasdf
    // http - протокол
    // localhost - адрес
    // 8080 - порт
    // /asdfasdf - запрос
    // В программу приходит только запрос
    std::string action(ri->uri);

  /*  char post_data[1024];
    int post_data_len;
    post_data_len = mg_read(conn, post_data, sizeof(post_data));

*/


        //cout << "Site 1 requested\n";

        // User has submitted a form, show submitted data and a variable value

    cout << "action: " << action << "\n";

        if (action == "/site1"){
            string s = "Bootstrap" + action + ".html";

                cout << s <<"\n";

                std::string data = ReadFromFile(s);
                string type = GetType(action);
                std::string response = GetHeader("text/html") + data;
                mg_printf(conn, response.c_str());
        }
    else

        if (action == "/file1")
        {
        char post_data[1024];
        int post_data_len;
        post_data_len = mg_read(conn, post_data, sizeof(post_data));
        cout << "postDataLen = " << post_data_len << "\n";

        string postData(post_data , post_data_len);
        cout << "postData: " << postData << "\n";



        int count = 0;
        for (int i = 0; i < postData.size(); i++)
            if(postData[i] == '&')
                count++;

        count +=2;
        int num[10];
        int summ = 0;


        for (int i = 1; i < count ; i++)
        {
            string str1 = "inputNumber" + to_string(i);
            string input1 = GetParam(postData , str1);

            cout << "input: " << input1 << "\n";

            num[i] = stoi(input1);
            summ += num[i];
        }


        cout << "final summa " << summ << "\n";

        string s = to_string(summ);

        cout << "s: " << s << "\n";


        std::string data = ReadFromFile("Bootstrap/file1.html");
        std::string response = GetHeader("text/html") + data + s + "</h1> </div> </body> </html>"; //"</p> </font> </body> </html>";
        mg_printf(conn, response.c_str());

        //return 1;
    } else {
        string s = "Bootstrap" + action;

        cout << s <<"\n";

        std::string data = ReadFromFile(s);
        string type = GetType(action);
        std::string response = GetHeader(type) + data;
        mg_printf(conn, response.c_str());
    }


    return 1;  // Mark request as processed
}







int main(void) {



  struct mg_context *ctx;
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {"listening_ports", "8080", NULL};

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  // Start the web server.
  ctx = mg_start(&callbacks, NULL, options);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  mg_stop(ctx);

  return 0;
}
