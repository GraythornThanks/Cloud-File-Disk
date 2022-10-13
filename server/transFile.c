#include"../include/all.h"

int recvFile(int fd)
{ 
    //小火车式接收


    //接收文件名
    train_t train;
    memset(&train, 0, sizeof(train));
    recv(fd, &train.len, sizeof(train.len), MSG_WAITALL);
    recv(fd, &train.data, train.len, MSG_WAITALL);

    //接收文件大小
    int file_len = 0;
    recv(fd, &file_len, 4, MSG_WAITALL);

    printf("Recvfile from client:  Filename: %s   Size: %d\n", train.data, file_len);
    //打开文件
    int wfd = open(train.data, O_RDWR | O_CREAT | O_TRUNC, 0666);
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
    puts("redvfile over.");
    return 0;
}

int sendFile(int fd, char* pathname)
{
    //以train_t的形式发送文件

    //发送文件名
    train_t train;
    memset(&train, 0, sizeof(train));

    char* curr = pathname;
    while(*curr != 0)
        curr++;
    curr--;
    while(*curr != '/' && curr >= pathname)
        curr--;
    curr++;

    train.len = strlen(curr);
    strncpy(train.data, curr, train.len);

    int ret = send(fd, &train, train.len + 4, MSG_WAITALL);
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
    if(rfd == -1)
    {
        puts("Nonexistent file.");
        return -1;
    }

    //sendfile发送
    ret = sendfile(fd, rfd, NULL, statbuf.st_size);
    close(rfd);
    puts("sendfile over.");
    
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
    ret = recv(fd, buf, len, MSG_WAITALL);

    printf("%s\n", buf);

    return len;
}

//断点续传式接受
int recvFile1(int fd, char* argument)
{
    //接受方先检查是否已经存在文件
    
    //先得到文件名
    char name_buf[1024] = {0};
    char* curr = argument;
    while(*curr != '\0' ){
        curr++;
    }
    while(*curr != '/' && curr >= argument){
        curr--;
    }
    curr++;
    strcpy(name_buf, curr);

    //判断当前文件夹下是否已经有文件
    if(access(name_buf, W_OK | R_OK) == 0){
        //说明当前文件夹已经有文件
        //而且可以写
        puts("Break point receive continuing ...");

        int isExist = 1;
        int ret = send(fd, &isExist, 4, 0);
        ERROR_CHECK(ret, -1, "send");

        int fd_dest = open(name_buf, O_APPEND | O_RDWR, 0666);
        //获取本地文件大小
        struct stat statbuf;
        memset(&statbuf, 0, sizeof(statbuf));
        fstat(fd_dest, &statbuf);

        off_t offset = statbuf.st_size;
        send(fd, &offset, sizeof(offset), 0);
        printf("sended offset: %ld\n", offset);
        //接受来源文件大小
        off_t file_len;
        recv(fd, &file_len, sizeof(file_len), MSG_WAITALL);
        printf("received file_len: %ld\n", file_len);

        off_t cnt = offset;
        ret = 0;

        while(cnt < file_len){
            char readbuffer[BUFFSIZE] = {0};
            ret = read(fd, readbuffer, BUFFSIZE);
            ERROR_CHECK(ret, -1, "read");
            write(fd_dest, readbuffer, ret);
            cnt += ret;
        }

        close(fd_dest);
        puts("recvFile1 succeeded");
    }else{
        //说明没有文件
        int isExist = 0;
        int ret = send(fd, &isExist, 4, 0);
        ERROR_CHECK(ret, -1, "send");

        recvFile(fd);
    }
    return 0;
}


//断点续传式发送
int sendFile1(int fd, char* pathname)
{
    puts("sendFile1 began\n");
    //先接受信息,看接受方是否已经存在文件
    int isExist = -1;
    recv(fd, &isExist, 4, MSG_WAITALL);
    printf("isExist = %d\n", isExist);
    if(isExist == 1){
        //说明已经存在文件
        //默认开始断点续传
        
        //接受接受方的文件大小
        puts("breaking point send began...");
        off_t offset = 0;
        recv(fd, &offset, sizeof(offset), MSG_WAITALL);
        printf("received offset: %ld\n", offset);

        //发送文件内容
        int rfd = open(pathname, O_RDONLY);
        if(rfd == -1){
            perror("sendFile1: open failed");
            exit(-1);
        }
        
        //获取并发送文件大小
        struct stat statbuf;
        memset(&statbuf, 0, sizeof(statbuf));
        fstat(rfd, &statbuf);
        off_t file_len = statbuf.st_size;
        send(fd, &file_len, sizeof(file_len), 0);

        size_t count = statbuf.st_size - offset;
        
        //sendfile发送
        sendfile(fd, rfd, &offset, count);
        puts("sendFile1 succeeded");
        close(rfd);
    }else if(isExist == 0){
        //说明没有断点续传
        sendFile(fd, pathname);
    }else{
        perror("sendFile1: isExist Error!");
        exit(-1);
    }
    return 0;
}
