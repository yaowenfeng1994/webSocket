//
// Created by 姚文锋 on 2018/10/1.
//
#include "debug_log.h"

debugLog* debugLog::m_debug_log = nullptr;

debugLog* debugLog::log(){
    if(m_debug_log == nullptr){
        m_debug_log = new debugLog();
    }
    return m_debug_log;
}

void debugLog::create_log_file(){
    if ((dir=opendir("../log/")) == nullptr)
    {
        system("mkdir -p ../log/");
    }
    if(fp != nullptr)
        fclose(fp);
    sprintf(file_path, "../log/web_socket");
    time(&tim);
    t = localtime(&tim);
    memcpy(&last_log_time, t, sizeof(struct tm));
    // +15表示从第15个字节开始, ./log/debug_log的长度刚好是15
    sprintf(file_path + 16, "%02d_%02d.log",t->tm_mon + 1, t->tm_mday);
    fp = fopen(file_path, "a+");
}

void debugLog::write_log(const char *msg){
    time(&tim);
    t = localtime(&tim);
    sprintf(message, "[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
#ifdef __WRITE_FILE__
    if(t->tm_mday != last_log_time.tm_mday || t->tm_mon != last_log_time.tm_mon
       || t->tm_year != last_log_time.tm_year)
        create_log_file();
    fwrite(message, strlen(message), 1, fp);
    fflush(fp);
#else
    printf("%s", message);
	fflush(stdout);
#endif
}

debugLog::debugLog():
// 构造函数成员变量初始化，初始化列表
        tim(0),
        t(nullptr),
        dir(nullptr),
        fp(nullptr),
        file_path(),
        message(),
        last_log_time()
{
#ifdef __WRITE_FILE__
    create_log_file();
#endif
}

debugLog::~debugLog(){
#ifdef __WRITE_FILE__
    fclose(fp);
#endif
}

void DEBUG_LOG(const char *msg, ...) {
    char message[256] = {0};
    // va_list、va_start和va_end宏（在STDARG.H中定义）帮助我们处理堆栈指针
    va_list args;
    va_start(args, msg);
    vsprintf(message, msg, args);
    va_end(args);
    debugLog::log()->write_log(message);
}
