//
// Created by 姚文锋 on 2018/10/1.
//
#include "web_socket_handler.h"

webSocketHandler::webSocketHandler(int fd):
        buff(),
        user_id(),
        status(WEB_SOCKET_UN_CONNECT),
        header_map(),
        fd(fd),
        request(new webSocketRequest),
        first(true),
        request_buff(),
        op_code()
{
}

webSocketHandler::~webSocketHandler(){
}

ssize_t webSocketHandler::process(){
    if(status == WEB_SOCKET_UN_CONNECT){
        return hand_shark();
    }
    request->fetch_web_socket_info(buff);
    request->print();

//    unsigned char* out;
//    size_t         out_len;
//    char* request_msg = request->get_msg();
//    size_t request_msg_len = request->get_msg_length();
//    uint8_t msg_op_code = request->get_msg_op_code();

    if (!first) {
        memcpy(request_buff, request->get_msg(), strlen(request->get_msg()));
        op_code = request->get_msg_op_code();
//        if (msg_op_code == 1) {
//            //组包
//            respond->pack_data((const unsigned char*)request_msg,request_msg_len , WEB_SOCKET_FIN_MSG_END ,
//                               WEB_SOCKET_TEXT_DATA , WEB_SOCKET_NEED_NOT_MASK , &out, &out_len);
//            send_data(fd, (char*)out);          //回显
//            free(out);
//        } else if (msg_op_code == 0) {
//            request->print();
//        }
    } else {
        memcpy(user_id, request->get_msg(), strlen(request->get_msg()));
    }
    request->reset();
    return 0;
}

ssize_t webSocketHandler::hand_shark(){
    char request[1024] = {};
    status = WEB_SOCKET_HAND_SHARKED;
    fetch_http_info();
    parse_str(request);
    memset(buff, 0, sizeof(buff));
    return send_data(fd, request);
}

void webSocketHandler::parse_str(char *request){
    strcat(request, "HTTP/1.1 101 Switching Protocols\r\n");
    strcat(request, "Connection: upgrade\r\n");
    strcat(request, "Sec-WebSocket-Accept: ");
    string server_key = header_map["Sec-WebSocket-Key"];
    server_key += MAGIC_KEY;

    SHA1 sha;
    unsigned int message_digest[5];
    sha.Reset();
    sha << server_key.c_str();

    sha.Result(message_digest);
    for (int i = 0; i < 5; i++) {
        message_digest[i] = htonl(message_digest[i]);
    }
    server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
    server_key += "\r\n";
    strcat(request, server_key.c_str());
    strcat(request, "Upgrade: websocket\r\n\r\n");
}

int webSocketHandler::fetch_http_info(){
    istringstream s(buff);
    string request;

    getline(s, request);
    if (request[request.size()-1] == '\r') {
        request.erase(request.end()-1);
    } else {
        return -1;
    }

    string header;
    string::size_type end;

    while (std::getline(s, header) && header != "\r") {
        if (header[header.size()-1] != '\r') {
            continue; //end
        } else {
            header.erase(header.end()-1);	//remove last char
        }

        end = header.find(": ",0);
        if (end != std::string::npos) {
            string key = header.substr(0,end);
            string value = header.substr(end+2);
            header_map[key] = value;
        }
    }

    return 0;
}

ssize_t webSocketHandler::send_data(int target_fd, char *buff){
    return write(target_fd, buff, strlen(buff));
}

void webSocketHandler::set_first(bool result){
    first = result;
}

void webSocketHandler::reset(){
    op_code = 0;
    memset(buff, 0, sizeof(buff));
    memset(user_id, 0, sizeof(user_id));
    memset(request_buff, 0, sizeof(request_buff));
}