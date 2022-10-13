#include "my_head.h"
#include "ls.h"
#include "rm.h"
#include "mkdir.h"
#include "passwd.h"
#include "mkdir.h"
#include "sendLog.h"

typedef struct sockaddr_in sockaddr_in;

#define IP "127.0.0.1"
#define PORT 8888
#define QUEUE 5
#define BUFFSIZE 1024


typedef struct{
    int len;
    char data[BUFFSIZE];
}train_t;



//cd命令
int sysCd(int fd, char* path);

//连接套接字和IP
int linkSocket(sockaddr_in* addr, char* ip, int port);

//把字符串型命令转换为数字
int cmd_ctoi(char* cmd);

//命令转字符串
char* cmd_itoc(int cmd);

//客户端发送命令
int sendCmd(int clientfd);

//处理客户端发来的命令
int recvCmd(int fd, int key);

//发送字符串
int sendMsg(int fd, char* msg);
//接收字符串
int recvMsg(int fd);

//发送文件
int sendFile(int fd, char* pathname);
//接收文件
int recvFile(int fd);

//断点式发送
int sendFile1(int fd, char* pathname);

//断点式接受
int recvFile1(int fd, char* pathname);

//服务器信息处理
int serverHandle(int cmd, int fd, char* arguments);

//命令分类
int cmdClassification(int cmd);

// 打印服务器当前工作目录
int sendPwd(int fd);

//服务器处理epoll就序列表
int newMsg(int nready, struct epoll_event* alreadyEvent, int key, int listenfd);
