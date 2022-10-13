#include"../include/all.h"

int main(void)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listenfd, -1, "socket");

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on));

    sockaddr_in addr;
    linkSocket(&addr, IP, PORT);

    int ret = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(listenfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    puts("connect seccess");

    int key = epoll_create1(0);
    struct epoll_event evt;
    memset(&evt, 0, sizeof(evt));
    evt.data.fd = listenfd;
    evt.events = EPOLLIN;
    ret = epoll_ctl(key, EPOLL_CTL_ADD, listenfd, &evt);

    struct epoll_event* alreadyEvent = calloc(QUEUE, sizeof(struct epoll_event));

    int nready = 0;
    while(1)
    {
        nready = epoll_wait(key, alreadyEvent, QUEUE, -1);
        puts("====================================");

        if(nready == -1 && errno == EINTR)
            continue;
        else if(nready == -1)
        {
            perror("epoll_wait");
            continue;
        }
        else if(nready == 0)
        {
            puts("epoll_wait timeout");
            continue;
        }
        else
        {
            newMsg(nready, alreadyEvent, key, listenfd);
        }
        //while(1)
    }
    close(listenfd);
    return 0;
}
