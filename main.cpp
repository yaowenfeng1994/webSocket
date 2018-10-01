//
// Created by 姚文锋 on 2018/9/30.
//
#include "socket_interface.h"
#include "debug_log.h"

int main() {
    SOCKET_INTERFACE->run();

/*    char a[32];
    sprintf(a, "./log/web_socket");
    cout << a << endl;
    struct tm* t;
    struct tm last_log_time;
    time_t tim;
    time(&tim);
    t = localtime(&tim);
    memcpy(&last_log_time, t, sizeof(struct tm));

    sprintf(a+16, "%02d_%02d.log",t->tm_mon + 1, t->tm_mday);
    cout << a << endl;*/

//    DEBUG_LOG("%s", "姚文锋");
}