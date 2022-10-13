#include "../include/all.h"

// 打印服务器当前工作目录
int sendPwd(int fd)
{
    char* pwd = getcwd(NULL, 0);
    ERROR_CHECK(pwd, NULL, "getcwd");
    
    // 向客户端发送当前工作目录
    sendMsg(fd, pwd);

    free(pwd);
}
