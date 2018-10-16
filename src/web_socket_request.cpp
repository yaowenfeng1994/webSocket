//
// Created by 姚文锋 on 2018/10/1.
//
#include "web_socket_request.h"

webSocketRequest::webSocketRequest():
        fin(),
        op_code(),
        mask(),
        masking_key(),
        payload_length(),
        payload()
{
}

webSocketRequest::~webSocketRequest(){

}

int webSocketRequest::fetch_web_socket_info(char *msg){
    int pos = 0;
    fetch_fin(msg, pos);
    fetch_op_code(msg, pos);
    fetch_mask(msg, pos);
    fetch_payload_length(msg, pos);
    fetch_masking_key(msg, pos);
    return fetch_payload(msg, pos);
}

void webSocketRequest::print(){
    DEBUG_LOG("WEBSOCKET PROTOCOL\n"
              "FIN: %d\n"
              "OPCODE: %d\n"
              "MASK: %d\n"
              "PAYLOADLEN: %d\n"
              "PAYLOAD: %s",
              fin, op_code, mask, payload_length, payload);
//    reset();
}

void webSocketRequest::reset(){
    fin = 0;
    op_code = 0;
    mask = 0;
    memset(masking_key, 0, sizeof(masking_key));
    payload_length = 0;
    memset(payload, 0, sizeof(payload));
}

int webSocketRequest::fetch_fin(char *msg, int &pos){
    fin = (unsigned char)msg[pos] >> 7;
    return 0;
}

int webSocketRequest::fetch_op_code(char *msg, int &pos){
    op_code = (unsigned char)(msg[pos] & 0x0f);
    pos++;
    return 0;
}

int webSocketRequest::fetch_mask(char *msg, int &pos){
    mask = (unsigned char)msg[pos] >> 7;
    return 0;
}

int webSocketRequest::fetch_masking_key(char *msg, int &pos){
    if(mask != 1)
        return 0;
    for(int i = 0; i < 4; i++)
        masking_key[i] = (unsigned char)msg[pos + i];
    pos += 4;
    return 0;
}

int webSocketRequest::fetch_payload_length(char *msg, int &pos){
    payload_length = ( unsigned long long)(msg[pos] & 0x7f);
    pos++;
    if(payload_length == 126){
        uint16_t length = 0;
        memcpy(&length, msg + pos, 2);
        pos += 2;
        payload_length = ntohs(length);
    }
    else if(payload_length == 127){
        uint32_t length = 0;
        memcpy(&length, msg + pos, 4);
        pos += 4;
        payload_length = ntohl(length);
    }
    return 0;
}

int webSocketRequest::fetch_payload(char *msg, int &pos){
    memset(payload, 0, sizeof(payload));
    if(mask != 1){
        memcpy(payload, msg + pos, payload_length);
    }
    else {
        for(uint i = 0; i < payload_length; i++){
            int j = i % 4;
            payload[i] = msg[pos + i] ^ masking_key[j];
        }
    }
    pos += payload_length;
    return 0;
}

uint64_t webSocketRequest::get_msg_length(){
    return payload_length;
}

uint8_t webSocketRequest::get_msg_op_code(){
    return op_code;
}