#include "../include/all.h"

int clientPasswdAuthentication(int clientfd)
{
    // 先给服务器发送密码验证是否成功
    send(clientfd, VALIDATION_FAILURE, strlen(VALIDATION_FAILURE), 0);

    printf("plese input user name(you have 3 times): ");
    char name[50] = {0}, cname[50] = {0};
    int m = 0;
    for(; m < 3; m++)
    {
        memset(name, 0, sizeof(name));
        memset(cname, 0, sizeof(cname));
        scanf("%s", name);
        send(clientfd, name, strlen(name), 0);

        recv(clientfd, cname, sizeof(cname), 0);
        if(strcmp(cname, USERNAME_IS_TRUE))
        {
            if(m == 2)
            {
                continue;
            }
            printf("Please re-enter username: ");
        }
        else
        {
            break;
        }
    }

    if(m == 3)
    {
        printf("Input times exceed three, the connection is disconnected\n");
        exit(1);
    }
    else
    {
        // 继续执行,输入密码
        struct spwd *sp;
        char *passwd;
        char salt[100] = {0}; 

        int n = 0;
        for(; n < 3; n++)
        {
            passwd = getpass("input your passwd: ");
            // 从输入的用户名中得到密码
            if((sp = getspnam(name)) == NULL)
            {
                perror("Get passwd failed!\n");
                continue;
            } 

            // 获取密文
            int i = 0, j = 0;
            // 取出salt, i ,记录密码字符下标, j记录$出现次数
            for(i = 0; sp->sp_pwdp[i] && j != 3; i++)
            {
                if(sp->sp_pwdp[i] == '$')
                    ++j;
            }
            strncpy(salt, sp->sp_pwdp, i - 1);
            char* cry = crypt(passwd, salt);

            // 发送密文
            send(clientfd, cry, strlen(cry), 0);

            // 接收服务器的确认消息并验证
            char cryp[50] = {0};
            recv(clientfd, cryp, sizeof(cryp), 0);
            if(strcmp(cryp, PASSWD_IS_TRUE))
            {
                if(n == 2)
                {
                    continue;
                }
                printf("Please re-enter password!\n");
            }
            else
            {
                break;
            }
        }

        if(n == 3)
        {
            printf("Input times exceed three, the connection is disconnected\n");
            exit(1);
        }
        else
        {
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
            struct epoll_event* alreadyEvent = calloc(QUEUE, sizeof(struct epoll_event));

            while(1)
            {
                int nready = epoll_wait(key, alreadyEvent, QUEUE, -1);
                puts("=====================================");
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
                    if(fd == STDIN_FILENO)
                    {
                        puts("New command: ");
                        sendCmd(clientfd);
                    }
                    else
                    {
                        //接收服务器返回内容
                        int return_type = 0;
                        int ret = recv(clientfd, &return_type, 4, MSG_WAITALL);
                        ERROR_CHECK(ret, -1, "send");
                        if(ret == 0)
                            exit(0);
                        else if(return_type < 100)
                        {
                            puts("Server: ");
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
        }
    }
}
