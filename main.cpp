//
// Created by 姚文锋 on 2018/9/30.
//
#include "socket_interface.h"
#include "debug_log.h"
#include "base64.h"
#include "utils/json/json.h"

// 输入二进制
void out_bin(char c){
    unsigned char k = 0x80;
    for (int i=0; i<8; i++, k >>= 1){
        if (c & k){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf(" ");
}

int main() {
    SOCKET_INTERFACE->run();

/*    string test="{\"id\":1,\"name\":\"test\"}";
    Json::Reader  reader;
    Json::Value   value;

    if(reader.parse(test,value))
    {
        if(!value["id"].isNull())
        {
            cout<<value["id"].asInt()<<endl;
            cout<<value["name"].asString()<<endl;
        }
    }*/

/*    char a[32];
    sprintf(a, "./log/web_socket");
    cout << a << endl;
    struct tm* t;
    struct tm last_log_time;
    time_t tim;
    time(&tim);
    t = localtime(&tim);
    memcpy(&last_log_time, t, sizeof(struct tm));

    sprintf(a+16, "%02d_%02d.log",t->tm_mon + 1, t->tm_mday);
    cout << a << endl;*/

//    DEBUG_LOG("%s", "姚文锋");

// 字符数组，字符数组和字符串最明显的区别就是字符串会被默认的加上结束符'\0'
/*    char a[4] = "abc";
    char b[4] = {'a', 'b', 'c', 'd'};
    cout << a << endl;

    uint8_t op_code;
    const char *msg = "abc";
    int pos = 0;
    op_code = (unsigned char)msg[pos];
    op_code = (unsigned char)(msg[pos] & 0x0f);
    out_bin(msg[pos]);
    out_bin(op_code);
    cout << msg[pos] << endl;
    cout << op_code << endl;

    const char *ww = "yaowenfeng";
    string b = base64_encode(reinterpret_cast<const unsigned char*>(ww), 10);
    cout <<  b << endl;
    string server_key1 = base64_decode(b);
    cout <<  server_key1 << endl;*/
}