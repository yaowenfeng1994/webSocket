//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef _SOCKET_INTERFACE_H
#define _SOCKET_INTERFACE_H
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <list>
#include "web_socket_handler.h"
#include "web_socket_respond.h"

#define PORT 13389
#define TIME_WAIT 120
#define HOST "172.16.213.82"
#define MAX_EVENTS_SIZE 20
#define BUFF_LEN 2048
#define SOCKET_INTERFACE socketInterface::get_share_socket_interface()

using namespace std;

typedef map<int, webSocketHandler *> WEB_SOCKET_HANDLER_MAP;

struct clientSocketFd {
    int     socket_fd;
    char   user_id[512];
    char   user_name[512];
    char    send_buff[2048];
};

class socketInterface {

private:
    int epoll_fd;
    int listen_fd;
    static socketInterface* m_socket_interface;
    WEB_SOCKET_HANDLER_MAP web_socket_handler_map;

    list<clientSocketFd> connection_fds;

private:
    // 构造函数
    socketInterface();
    // 析构函数
    ~socketInterface();

    int init();
    int epoll_loop();
    int set_noblock(int fd);
    void ctl_event(int fd, bool flag);
    webSocketRespond *respond;      //应答处理类

public:
    void run();
    static socketInterface* get_share_socket_interface();
};

#endif //SOCKET_H
