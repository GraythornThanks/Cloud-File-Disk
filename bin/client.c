#include"chang.h"

int main(int argc, char* argv[])
{
    // IP  +   PORT
    ARGS_CHECK(argc, 3);

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(clientfd, -1, "socket");

    sockaddr_in addr;
    int port = atoi(argv[2]);
    linkSocket(&addr, argv[1], port);

    int ret = connect(clientfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect1");

    puts("connect seccess");

    int key = epoll_create1(0);

    //将socktefd写入红黑树
    struct epoll_event evt1;
    memset(&evt1, 0, sizeof(evt1));
    evt1.data.fd = clientfd;
    evt1.events = EPOLLIN;
    epoll_ctl(key, EPOLL_CTL_ADD, clientfd, &evt1);

    //将STDIN写入红黑树
    struct epoll_event evt2;
    memset(&evt2, 0, sizeof(evt2));
    evt2.data.fd = STDIN_FILENO;
    evt2.events = EPOLLIN;
    epoll_ctl(key, EPOLL_CTL_ADD, STDIN_FILENO, &evt2);

    puts("epoll success");
    //就绪队列
    struct epoll_event* alreadyEvent = (struct epoll_event*)calloc(QUEUE, sizeof(struct epoll_event));

    while(1)
    {
        int nready = epoll_wait(key, alreadyEvent, QUEUE, -1);
        printf("nready = %d\n", nready);
        if(nready == -1 && errno == EINTR)
            continue;
        else if(nready == -1)
        {
            perror("epoll_wait");
            exit(1);
        }
        else if(nready == 0)
        {
            puts("link break.");
            sleep(1);
            continue;
        }

        for(int i = 0; i < nready; i++)
        {
            //客户端输入命令
            int fd = alreadyEvent[i].data.fd;
            printf("fd = %d\n", fd);
            if(fd == STDIN_FILENO)
            {
                sendCmd(clientfd);
            }
            else
            {
                //接收服务器返回内容
                int return_type = 0;
                ret = recv(clientfd, &return_type, 4, MSG_WAITALL);
                ERROR_CHECK(ret, -1, "send");

                if(return_type < 100)
                {
                    recvMsg(clientfd);
                }
                else
                {
                    //返回的是文件
                    ret = recvFile(clientfd);
                    ERROR_CHECK(ret, -1, "send");
                }
            }
            //for
        }
        //while(1)
    }

    close(clientfd);

    return 0;
}
