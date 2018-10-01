//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef _SOCKET_INTERFACE_H
#define _SOCKET_INTERFACE_H
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
//#include <sys/epoll.h>

#define PORT 13389
#define HOST "172.16.213.82"
#define MAX_EVENTS_SIZE 20
#define SOCKET_INTERFACE socketInterface::get_share_socket_interface()

using namespace std;

//typedef std::map<int, Websocket_Handler *> WEB_SOCKET_HANDLER_MAP;

class socketInterface {

private:
    int epoll_fd;
    int listen_fd;
    static socketInterface* m_socket_interface;
//    WEB_SOCKET_HANDLER_MAP web_socket_handler_map;

private:
    // 构造函数
//    socketInterface();
    // 析构函数
//    ~socketInterface();

    int init();
    void ctl_event(int fd, bool flag);

public:
    void run();
    static socketInterface* get_share_socket_interface();
};

#endif //SOCKET_H
