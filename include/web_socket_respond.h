//
// Created by 姚文锋 on 2018/10/14.
//

#ifndef _WEB_SOCKET_RESPOND_H
#define _WEB_SOCKET_RESPOND_H

#include <stdint.h>
#include <arpa/inet.h>
#include "debug_log.h"

//#define MSG_MAX_LEN 2048

class webSocketRespond {
public:
    webSocketRespond();
//    ~webSocketRespond();

public:
    int  pack_data(const unsigned char* message,size_t msg_len, uint8_t fin ,
                   uint8_t op_code, uint8_t mask, unsigned char** out, size_t* out_len);

private:
    uint8_t fin_;
    uint8_t op_code;
    uint8_t mask;
};

#endif //_WEB_SOCKET_RESPOND_H
