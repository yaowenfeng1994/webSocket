//
// Created by 姚文锋 on 2018/10/1.
//

#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H

//#define __WRITE_FILE__

#include <iostream>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <dirent.h>

using namespace std;

void DEBUG_LOG(const char *msg, ...);

class debugLog {

private:
    static debugLog* m_debug_log;
    time_t tim;
    struct tm *t;
    DIR *dir;
    FILE *fp;
    char file_path[32];
    char message[256];
    struct tm last_log_time;

private:
    debugLog();
    ~debugLog();
    void create_log_file();

public:
    static debugLog* log();
    void write_log(const char *msg);
};

#endif //_DEBUG_LOG_H
