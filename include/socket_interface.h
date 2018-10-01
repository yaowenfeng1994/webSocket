//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
//#include <sys/epoll.h>

#define PORT 13389
#define HOST "172.16.213.82"
#define MAX_EVENTS_SIZE 20
#define SOCKET_INTERFACE socketInterface::get_share_socket_interface()

using namespace std;

class socketInterface {

private:
    // 构造函数
//    socketInterface();
    // 析构函数
//    ~socketInterface();
    int epoll_fd;
    int listen_fd;
    static socketInterface* m_socket_interface;
    int init();

public:
    void run();
    static socketInterface* get_share_socket_interface();
};

#endif //SOCKET_H
