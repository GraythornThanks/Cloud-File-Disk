#include"../include/all.h"

//chdir
int sysCd(int fd, char* path)
{
    DIR* fdir = opendir(path);
    if(fdir == NULL)
    {
        sendMsg(fd, "Without this pathname.");
        return 0;
    }
    else
    {
        int ret = chdir(path);
        ERROR_CHECK(ret, -1, "chdir");
        closedir(fdir);
    }
    return 0;
}

//连接IP
int linkSocket(sockaddr_in* addr, char* ip, int port)
{
    memset(addr, 0, sizeof(sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);
    return 0;
}

//发送命令
int sendCmd(int fd)
{
    // 先发送一个标志,表示密码已经验证成功
    send(fd, VALIDATION_SUCCESS, strlen(VALIDATION_SUCCESS), 0);

    //命令字符转换整型
    char buf[1024] = {0};
    int ret = 0;
    int cmd_len = read(STDIN_FILENO, buf, sizeof(buf));

    char cmd[10] = {0};
    char arguments[1024] = {0};

    //把命令+参数的长字符串分别存放到两个数组中
    sscanf(buf, "%s %s ", cmd, arguments);

    //将命令字符转换成命令代码
    int cmd_num = cmd_ctoi(cmd);
    
    //发送命令代码
    ret = send(fd, &cmd_num, 4, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");

    //发送参数
    train_t train;
    memset(&train, 0, sizeof(train));
    train.len = strlen(arguments);

    printf("cmd_num: %d , arguments: %s , argument len = %d\n", cmd_num, arguments, train.len);
    if(train.len != 0)
    {
        strncpy(train.data, arguments, train.len);
        ERROR_CHECK(ret, -1, "send");
    }

    send(fd, &train, train.len + 4, MSG_WAITALL);

    //命令为上传文件单独处理
    if(cmd_num == 4)
    {
        //sendFile(fd, arguments);
		sendFile1(fd, arguments);
    }
    else if(cmd_num == 5)
    {
        //recvFile(fd);
		recvFile1(fd, arguments);
    }

    return 0;
}


//接收命令
int recvCmd(int fd, int key)
{
    //已连接客户端有消息发送
    
    //接收命令
    int cmd_num = 0;
    int ret = recv(fd, &cmd_num, 4, MSG_WAITALL);
    
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
    if(length != 0)
    {
        ret = recv(fd, arguments, length, MSG_WAITALL);
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
    }

    printf("Cmd from client: %d , %s , argument len = %d\n", cmd_num, arguments, length);

    serverHandle(cmd_num, fd, arguments);
    
    sendLogMsg("server", fd, cmd_itoc(cmd_num), FUNC_NOARGS);

    return 0;
}


int newMsg(int nready, struct epoll_event* alreadyEvent, int key, int listenfd)
{
    for(int i = 0; i < nready; i++)
    {
        int fd = alreadyEvent[i].data.fd; 
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
            sendConnectLog("server", fd);
        }
        else
        {
            //客户端有命令到来
            int ret = recvCmd(fd, key);
        }
        //for
    }
}
