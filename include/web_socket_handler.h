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

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

using namespace std;

enum WEB_SOCKET_STATUS {
    WEB_SOCKET_UN_CONNECT = 0,
    WEB_SOCKET_HAND_SHARKED = 1,
};

typedef map<string, string> HEADER_MAP;

class webSocketHandler{

public:
    webSocketHandler(int fd);
    ~webSocketHandler();
    int process();
    inline char *get_buff();

    WEB_SOCKET_STATUS status;
//    int send_data(char *buff);

private:
    int hand_shark();
    void parse_str(char *request);
    int fetch_http_info();
    int send_data(char *buff);

private:
    char buff[2048];
//    WEB_SOCKET_STATUS status;
    HEADER_MAP header_map;
    int fd;
    webSocketRequest *request;
};

inline char *webSocketHandler::get_buff(){
    return buff;
}

#endif //_WEB_SOCKET_HANDLER_H
