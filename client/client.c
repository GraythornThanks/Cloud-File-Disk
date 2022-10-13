#include"../include/all.h"

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

    clientPasswdAuthentication(clientfd);
    
    close(clientfd);

    return 0;
}
