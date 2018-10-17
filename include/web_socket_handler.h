//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef _WEB_SOCKET_HANDLER_H
#define _WEB_SOCKET_HANDLER_H

#include <map>
#include <sstream>
#include <unistd.h>

#include "sha1.h"
#include "base64.h"
#include "web_socket_request.h"
//#include "web_socket_respond.h"
#include "../utils/json/json.h"

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

using namespace std;

//握手时使用
enum WEB_SOCKET_STATUS {
    WEB_SOCKET_UN_CONNECT = 0,
    WEB_SOCKET_HAND_SHARKED = 1,
};

//用于描述消息是否结束
enum WEB_SOCKET_FIN_STATUS {
    WEB_SOCKET_FIN_MSG_END      =1,  //该消息为消息尾部
    WEB_SOCKET_FIN_MSG_NOT_END  =0,  //还有后续数据包
};

//用于描述消息类型
//目前websocket未定义的类型已省略
enum WEB_SOCKET_OP_CODE_TYPE {
    WEB_SOCKET_APPEND_DATA      =0X0, //表示附加数据
    WEB_SOCKET_TEXT_DATA        =0X1, //表示文本数据
    WEB_SOCKET_BINARY_DATA      =0X2, //表示二进制数据
    WEB_SOCKET_CONNECT_CLOSE    =0X8, //表示连接关闭
    WEB_SOCKET_PING_PACK        =0X9, //表示ping
    WEB_SOCKET_PANG_PACK        =0XA, //表示表示pong
};

//用于描述数据是否需要掩码
//客户端发来的数据必须要掩码处理
//服务器返回的数据不需要掩码处理
enum WEB_SOCKET_MASK_STATUS {
    WEB_SOCKET_NEED_MASK        =1,  //需要掩码处理
    WEB_SOCKET_NEED_NOT_MASK    =0,  //不需要掩码处理
};

typedef map<string, string> HEADER_MAP;

class webSocketHandler{

public:
    webSocketHandler(int fd);
    ~webSocketHandler();
    ssize_t process();

    inline char* get_buff();
    inline char* get_request_buff();
    inline char* get_user_id();
    inline bool get_first();
    inline uint8_t get_msg_op_code();
    inline WEB_SOCKET_STATUS get_state();
    inline size_t get_request_msg_len();

    void reset();
    void set_first(bool result);
    ssize_t send_data(int target_fd, char *buff);

private:
    ssize_t hand_shark();
    void parse_str(char *request);
    int fetch_http_info();
//    ssize_t send_data(int target_fd, char *buff);

private:
    char buff[2048];                //用来接收
    char request_buff[2048];        //用来发送
    size_t request_msg_len;
    char user_id[512];
    WEB_SOCKET_STATUS status;
    HEADER_MAP header_map;
    int fd;
    webSocketRequest *request;      //请求处理类
//    webSocketRespond *respond;      //应答处理类
    bool first;                     //表示构造函数是否是第一次发送消息，将user_id与fd绑定
    uint8_t op_code;
};

inline char* webSocketHandler::get_buff(){
    return buff;
}

inline char* webSocketHandler::get_request_buff(){
    return request_buff;
}

inline char* webSocketHandler::get_user_id(){
    return user_id;
}

inline bool webSocketHandler::get_first(){
    return first;
}

inline uint8_t webSocketHandler::get_msg_op_code(){
    return op_code;
}

inline WEB_SOCKET_STATUS webSocketHandler::get_state(){
    return status;
}

inline size_t webSocketHandler::get_request_msg_len(){
    return request_msg_len;
}

#endif //_WEB_SOCKET_HANDLER_H
