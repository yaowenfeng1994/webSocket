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

// 该方法已废弃
//void socketInterface::respondClient(int sockClient, unsigned char receive_buff[],size_t length, bool finalFragment){
//    char buf[1024] = "";
//    char char_buf[1024] = "";
//    int first = 0x00;
//    int tmp = 0;
//    if (finalFragment) {
//        first = first + 0x80;
//        first = first + 0x1;
//    }
//    buf[0] = first;
//    tmp = 1;
//    cout <<"数组长度:"<< length << endl;
//    int nuNum = (unsigned)length;
//    if (length < 126) {
//        // buf[1] = length;
//        buf[1] = '0'; // server下发数据给client时一般不进行掩码处理
//        tmp = 2;
//    }else if (length < 65536) {
//        buf[1] = 126;
//        buf[2] = nuNum >> 8;
//        buf[3] = length & 0xFF;
//        tmp = 4;
//    }else {
//        //数据长度超过65536
//        buf[1] = 127;
//        buf[2] = 0;
//        buf[3] = 0;
//        buf[4] = 0;
//        buf[5] = 0;
//        buf[6] = nuNum >> 24;
//        buf[7] = nuNum >> 16;
//        buf[8] = nuNum >> 8;
//        buf[9] = nuNum & 0xFF;
//        tmp = 10;
//    }
//    for (int i = 0; i < length;i++){
//        buf[tmp+i]= receive_buff[i];
//    }
//    memcpy(char_buf, buf, strlen(buf));
//    send(sockClient, char_buf, 1024, 0);
//}

int socketInterface::epoll_loop(){
    struct sockaddr_in client_address;
    socklen_t client;
    int active_fds = 0;
    int fd = 0;
    struct epoll_event events[MAX_EVENTS_SIZE];
    while(true){
        if (!connection_fds.empty())
        {
            cout << "当前连接池里的用户有:(";
            for(list<clientSocketFd>::iterator it=connection_fds.begin() ; it!=connection_fds.end() ; it++){
                cout << it->user_id << ",";
            }
            cout << ")" << endl;
        }

        sleep(1);

        active_fds = epoll_wait(epoll_fd, events, MAX_EVENTS_SIZE, TIME_WAIT);
        for(int i = 0; i < active_fds; i++){
            if(events[i].data.fd == listen_fd){
                fd = accept(listen_fd, (struct sockaddr *)&client_address, &client);
                ctl_event(fd, true);
            }
            else if(events[i].events & EPOLLIN){
                if((fd = events[i].data.fd) < 0)
                    continue;
                webSocketHandler *handler = web_socket_handler_map[fd];
                if(handler == NULL)
                    continue;
                if((read(fd, handler->get_buff(), BUFF_LEN)) <= 0){
                    ctl_event(fd, false);
                    for(list<clientSocketFd>::iterator it=connection_fds.begin() ; it!=connection_fds.end() ; it++){
                        if (it->socket_fd == fd) {
                            it = connection_fds.erase(it);
                            break;
                        }
                    }
                } else {
                    handler->process();
                    if (handler->get_first() && handler->get_state() == 1 && strlen(handler->get_user_id())>0){
                        clientSocketFd fdObj;
                        fdObj.socket_fd = fd;
                        memcpy(fdObj.user_id, handler->get_user_id(), strlen(handler->get_user_id()));
                        printf("fdObj.socket_fd: %d\n", fdObj.socket_fd);
                        printf("fdObj.user_id: %s\n", fdObj.user_id);
                        connection_fds.push_front(fdObj);
                        handler->set_first(false);
                    } else if (handler->get_msg_op_code()==8) {
                        ctl_event(fd, false);
                        for(list<clientSocketFd>::iterator it=connection_fds.begin() ; it!=connection_fds.end() ; it++){
                            if (it->socket_fd == fd) {
                                it = connection_fds.erase(it);
                                break;
                            }
                        }
                    } else {
                        unsigned char* out;
                        size_t         out_len;
                        char* request_msg = handler->get_request_buff();
                        size_t request_msg_len = handler->get_request_msg_len();
                        uint8_t msg_op_code = handler->get_msg_op_code();
                        if (msg_op_code == 1) {
                            //组包
                            respond->pack_data((const unsigned char*)request_msg,request_msg_len , WEB_SOCKET_FIN_MSG_END ,
                                   WEB_SOCKET_TEXT_DATA , WEB_SOCKET_NEED_NOT_MASK , &out, &out_len);
                            handler->send_data(fd, (char*)out);          //回显
                            free(out);
                        } else if (msg_op_code == 0) {
                            //
                            cout << "连接断开?" << endl;
                        }
                    }
                    handler->reset();
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

    if (flag) {
        set_noblock(fd);
        web_socket_handler_map[fd] = new webSocketHandler(fd);
        if (fd != listen_fd) {
            DEBUG_LOG("fd: %d 加入epoll循环", fd);
        }
    } else {
        close(fd);
        delete web_socket_handler_map[fd];
        web_socket_handler_map.erase(fd);
        DEBUG_LOG("fd: %d 退出epoll循环", fd);
    }

}

void socketInterface::run(){
    epoll_loop();
}