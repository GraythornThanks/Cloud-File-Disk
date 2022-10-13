#include"chang.h"

int recvFile(int fd)
{ 
    //小火车式接收


    //接收文件名
    train_t train;
    memset(&train, 0, sizeof(train));
    recv(fd, &train.len, sizeof(train), MSG_WAITALL);

    //接收文件大小
    int file_len = 0;
    recv(fd, &file_len, 4, MSG_WAITALL);

    //打开文件
    int wfd = open(train.data, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    ERROR_CHECK(wfd, -1, "open");

    //用splice接收
    int fds[2];
    pipe(fds);

    off_t cnt = 0;
    int ret = 0;

    while(cnt < file_len)
    {
        ret = splice(fd, NULL, fds[1], NULL, BUFFSIZE, SPLICE_F_MORE);
        splice(fds[0], NULL, wfd, NULL, ret, SPLICE_F_MORE);
        cnt += ret;
    }

    close(wfd);
    return 0;
}

int sendFile(int fd, char* pathname)
{
    //以train_t的形式发送文件


    //发送文件名
    train_t train;
    memset(&train, 0, sizeof(train));

    train.len = strlen(pathname);
    strncpy(train.data, pathname, train.len);

    int ret = send(fd, &train, sizeof(train), MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");

    //发送文件大小
    struct stat statbuf;
    memset(&statbuf, 0, sizeof(statbuf));
    ret = stat(pathname, &statbuf);
    ERROR_CHECK(ret, -1, "stat");

    ret = send(fd, &statbuf.st_size, 4, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");

    //发送文件内容
    int rfd = open(pathname, O_RDONLY);
    ERROR_CHECK(rfd, -1, "open");


    //sendfile发送
    ret = sendfile(fd, rfd, NULL, statbuf.st_size);
    close(rfd);
    
    return 0;
}

int sendMsg(int fd, char* msg)
{
    int temp = 1;
    send(fd, &temp, 4, MSG_WAITALL);

    train_t train;
    memset(&train, 0, sizeof(train));
    train.len = strlen(msg);
    strncpy(train.data, msg, train.len);

    int ret = send(fd, &train, train.len + 4, MSG_WAITALL);
    ERROR_CHECK(ret, -1, "send");

    return 0;
}

int recvMsg(int fd)
{
    //返回的是字符串
    char buf[1024] = {0};
    int len = 0;
    int ret = recv(fd, &len, 4, MSG_WAITALL);
    printf("msglen = %d\n",len);
    ret = recv(fd, buf, len, MSG_WAITALL);

    printf("%s\n", buf);

    return len;
}

