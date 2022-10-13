#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<sys/mman.h>
#include<stdbool.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/sendfile.h>
#include<syslog.h>
#include<shadow.h>
#include <crypt.h>

typedef void(*sighandler_t)(int);

#define st_mtime st_mtim.tv_sec

#define SIZE(x) (sizeof(x)/sizeof(x[0]))

#define ERROR_CHECK(info, stdd, tips) {\
    if(info == stdd){\
        perror(tips); \
        exit(1);\
    }\
}\

#define ARGS_CHECK(info, stdd){\
    if(info != stdd){\
        printf("ARGS not enough.");\
        exit(1);\
    }\
}\


