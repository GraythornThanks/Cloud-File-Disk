#include"chang.h"

int sysCd(char* path)
{
    int ret = chdir(path);
    ERROR_CHECK(ret, -1, "chdir");
    return 0;
}


int recvFromServer(int fd, char* arguments)
{
    return 0;
}

int linkSocket(sockaddr_in* addr, char* ip, int port)
{
    memset(addr, 0, sizeof(sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);
    return 0;
}
