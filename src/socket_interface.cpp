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

socketInterface::socketInterface():
        epoll_fd(0),
        listen_fd(0),
        web_socket_handler_map()
{
    if(0 != init())
        exit(1);
}

socketInterface::~socketInterface(){
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
    epoll_fd = epoll_create(MAX_EVENTS_SIZE);

    ctl_event(listen_fd, true);
    DEBUG_LOG("服务器启动成功!");
    return 0;
}

int socketInterface::epoll_loop(){
    struct sockaddr_in client_address;
    socklen_t clilen;
    int nfds = 0;
    int fd = 0;
    int bufflen = 0;
    struct epoll_event events[MAX_EVENTS_SIZE];
    while(true){
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS_SIZE, TIME_WAIT);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == listen_fd){
                fd = accept(listen_fd, (struct sockaddr *)&client_address, &clilen);
                ctl_event(fd, true);
            }
            else if(events[i].events & EPOLLIN){
                if((fd = events[i].data.fd) < 0)
                    continue;
                webSocketHandler *handler = web_socket_handler_map[fd];
                if(handler == NULL)
                    continue;
                if((bufflen = read(fd, handler->get_buff(), BUFF_LEN)) <= 0){
                    ctl_event(fd, false);
                }
                else{
                    handler->process();
                }
            }
        }
    }
    return 0;
}

int socketInterface::set_noblock(int fd){
    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


void socketInterface::ctl_event(int fd, bool flag){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = flag ? EPOLLIN : 0;
    epoll_ctl(epoll_fd, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
    if(flag){
        set_noblock(fd);
        web_socket_handler_map[fd] = new webSocketHandler(fd);
        if(fd != listen_fd)
            DEBUG_LOG("fd: %d 加入epoll循环", fd);
    }
    else{
        close(fd);
        delete web_socket_handler_map[fd];
        web_socket_handler_map.erase(fd);
        DEBUG_LOG("fd: %d 退出epoll循环", fd);
    }
}

void socketInterface::run(){
    epoll_loop();
}