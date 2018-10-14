//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef _WEB_SOCKET_REQUEST_H
#define _WEB_SOCKET_REQUEST_H

#include <stdint.h>
#include <arpa/inet.h>
#include "debug_log.h"

class webSocketRequest {
public:
    webSocketRequest();
    ~webSocketRequest();
    int fetch_web_socket_info(char *msg);
    void print();
    void reset();

    char* get_msg();
    uint64_t get_msg_length();
    uint8_t get_msg_op_code();

private:
    int fetch_fin(char *msg, int &pos);
    int fetch_op_code(char *msg, int &pos);
    int fetch_mask(char *msg, int &pos);
    int fetch_masking_key(char *msg, int &pos);
    int fetch_payload_length(char *msg, int &pos);
    int fetch_payload(char *msg, int &pos);

private:
    uint8_t fin;
    uint8_t op_code;
    uint8_t mask;
    uint8_t masking_key[4];
    uint64_t payload_length;
    char payload[2048];
};

#endif //_WEB_SOCKET_REQUEST_H
