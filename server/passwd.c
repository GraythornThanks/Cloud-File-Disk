#include "../include/all.h"

// 密码验证,失败则退出服务器
void serverPasswdAuthentication(int fd)
{
    // 获取本机的用户
    char* username = getlogin();
    char name[50] = {0};

    // 3次机会
    for(int i = 0; i < 3; i++)
    {
        memset(name, 0, sizeof(name));
        int ret = recv(fd, name, sizeof(name), 0);
        ERROR_CHECK(ret, -1, "recv");
        if(strcmp(username, name))
        {
            send(fd, USERNAME_IS_ERROR, strlen(USERNAME_IS_ERROR), 0);
        }
        else
        {
            send(fd, USERNAME_IS_TRUE, strlen(USERNAME_IS_TRUE), 0);
            break;
        }
    }

    for(int i = 0; i < 3; i++)
    {
        char cry[512] = {0};
        int ret = recv(fd, cry, sizeof(cry), 0);
        ERROR_CHECK(ret, -1, "recv");
        struct spwd* sp = getspnam(name);

        if(strcmp(sp->sp_pwdp, cry))
        {
            send(fd, PASSWD_IS_ERROR, strlen(PASSWD_IS_ERROR), 0);
        }
        else
        {
            send(fd, PASSWD_IS_TRUE, strlen(PASSWD_IS_TRUE), 0);
            break;
        }
    }
}
