#include"chang.h"

//发送命令
int sendCmd(int clientfd)
{
    //命令字符转换整型
    char buf[1024];
    int ret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("buf: %s\n", buf);

    char cmd_num[10] = {0};
    char arguments[1024] = {0};

    //把命令+参数的长字符串分别存放到两个数组中
    sscanf(buf, "%s %s ", cmd_num, arguments);

    //将命令字符转换成命令代码
    int cmd = cmd_ctoi(cmd_num);
    //发送命令代码
    ret = send(clientfd, &cmd, 4, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");

    //发送参数
    train_t train;
    memset(&train, 0, sizeof(train));

    if(cmd == 3)
    {
        train.len = strlen(cmd_num);
        strncpy(train.data, arguments, train.len);
        ret = send(clientfd, &train, train.len + 4, MSG_WAITALL);
        ERROR_CHECK(ret, -1, "send");
    }
    else
    {
        train.len = strlen(arguments);
        //printf("train.len = %d\n", train.len);
        strncpy(train.data, arguments, train.len);
        //printf("train.data: %s\n", train.data);
        ret = send(clientfd, &train, train.len + 4, MSG_WAITALL);
        ERROR_CHECK(ret, -1, "send");
    }
    return 0;
}

//接收命令
int recvCmd(int fd, int key)
{
    //已连接客户端有消息发送

    //接收命令
    int cmd = 0;
    int ret = recv(fd, &cmd, 4, MSG_WAITALL);
    printf("recvCmd = %d\n", cmd);

    if(ret == -1)
    {
        perror("recv");
        return -1;
    }
    else if(ret == 0)
    {
        puts("link break");
        struct epoll_event evt;
        memset(&evt, 0, sizeof(evt));
        evt.data.fd = fd;
        evt.events = EPOLLIN;
        ret = epoll_ctl(key, EPOLL_CTL_DEL, fd, &evt);
        return 0;
    }

    //接收参数长度
    int length = 0;
    ret = recv(fd, &length, 4, MSG_WAITALL);
    printf("length: %d\n", length);
    if(ret == -1)
    {
        perror("recv");
        return 0;
    }
    else if(ret == 0)
    {
        puts("link break");
        return 0;
    }

    //接收参数
    char arguments[1024] = {0};
    ret = recv(fd, arguments, length, MSG_WAITALL);
    printf("arguments: %s\n", arguments);
    if(ret == -1)
    {
        perror("recv");
        return 0;
    }
    else if(ret == 0)
    {
        puts("link break");
        return 0;
    }

    serverHandle(cmd, fd, arguments);

    return 0;
}


int newMsg(int nready, struct epoll_event* alreadyEvent, int key, int listenfd)
{
    for(int i = 0; i < nready; i++)
    {
        int fd = alreadyEvent[i].data.fd; 
        printf("fd = %d\n", fd);
        if(fd == listenfd)
        {
            int peerfd = accept(listenfd, NULL, NULL);
            ERROR_CHECK(peerfd, -1, "accept");
            //有新连接到来
            struct epoll_event evt;
            memset(&evt, 0, sizeof(evt));
            evt.data.fd = peerfd;
            evt.events = EPOLLIN;
            int ret = epoll_ctl(key, EPOLL_CTL_ADD, peerfd, &evt);
        }
        else
        {
            //客户端有命令到来
            int ret = recvCmd(fd, key);
        }
        //for
    }
}

int cmd_ctoi(char* cmd)
{
    if(!strcmp(cmd, "cd"))
        return 1;
    else if(!strcmp(cmd, "ls"))
        return 2;
    else if(!strcmp(cmd, "pwd"))
        return 3;
    else if(!strcmp(cmd, "upload"))
        return 4;
    else if(!strcmp(cmd, "download"))
        return 5;
    else
    {
        puts("Invalid command.");
        return -1;
    }
}


