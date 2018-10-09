//
// Created by 姚文锋 on 2018/10/1.
//
#include "web_socket_handler.h"

webSocketHandler::webSocketHandler(int fd):
        buff(),
        status(WEB_SOCKET_UN_CONNECT),
        header_map(),
        fd(fd),
        request(new webSocketRequest)
{
}

webSocketHandler::~webSocketHandler(){
}

int webSocketHandler::process(){
    if(status == WEB_SOCKET_UN_CONNECT){
        return hand_shark();
    }
    request->fetch_web_socket_info(buff);
    request->print();

    memcpy(receive_buff, request->payload, strlen(request->payload));
    request->reset();
    memset(buff, 0, sizeof(buff));
    return 0;
}

int webSocketHandler::hand_shark(){
    char request[1024] = {};
    status = WEB_SOCKET_HAND_SHARKED;
    fetch_http_info();
    parse_str(request);
    memset(buff, 0, sizeof(buff));
    return send_data(request);
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

    std::string header;
    std::string::size_type end;

    while (std::getline(s, header) && header != "\r") {
        if (header[header.size()-1] != '\r') {
            continue; //end
        } else {
            header.erase(header.end()-1);	//remove last char
        }

        end = header.find(": ",0);
        if (end != std::string::npos) {
            std::string key = header.substr(0,end);
            std::string value = header.substr(end+2);
            header_map[key] = value;
        }
    }

    return 0;
}

int webSocketHandler::send_data(char *buff){
    return write(fd, buff, strlen(buff));
}