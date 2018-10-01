//
// Created by 姚文锋 on 2018/10/1.
//
#include "debug_log.h"
#include "socket_interface.h"

socketInterface* socketInterface::m_socket_interface = nullptr;

socketInterface* socketInterface::get_share_socket_interface(){
    if(m_socket_interface == nullptr) {
        m_socket_interface = new socketInterface();
    }
    return m_socket_interface;
}

int socketInterface::init(){
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1){
        DEBUG_LOG("创建套接字失败!");
        return -1;
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(HOST);
    server_address.sin_port = htons(PORT);
    if(-1 == bind(listen_fd, (struct sockaddr *)(&server_address), sizeof(server_address))){
        DEBUG_LOG("绑定套接字失败!");
        return -1;
    }
    if(-1 == listen(listen_fd, 5)){
        DEBUG_LOG("监听失败!");
        return -1;
    }
//    epoll_fd = epoll_create(MAX_EVENTS_SIZE);
//
//    ctl_event(listen_fd, true);
    DEBUG_LOG("服务器启动成功!");
    return 0;
}

//void socketInterface::ctl_event(int fd, bool flag){
//    struct epoll_event ev;
//    ev.data.fd = fd;
//    ev.events = flag ? EPOLLIN : 0;
//    epoll_ctl(epollfd_, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
//    if(flag){
//        set_noblock(fd);
//        websocket_handler_map_[fd] = new Websocket_Handler(fd);
//        if(fd != listenfd_)
//            DEBUG_LOG("fd: %d 加入epoll循环", fd);
//    }
//    else{
//        close(fd);
//        delete websocket_handler_map_[fd];
//        websocket_handler_map_.erase(fd);
//        DEBUG_LOG("fd: %d 退出epoll循环", fd);
//    }
//}

void socketInterface::run(){
    init();
}