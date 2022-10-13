#include"chang.h"

int serverHandle(int cmd, int fd, char* arguments)
{
    //命令分类
    int cmd_type = cmdClassification(cmd);
    printf("cmd_type = %d\n", cmd_type);
    printf("cmd = %d\n", cmd);
    printf("pathname = %s\n", arguments);
    if(cmd_type == 0)
        return 0;
    else if(cmd_type == 1)
    {
        printf("pwd start\n");
        //短命令
        switch(cmd)
        {
        case 1: sysCd(arguments);                  break;
        case 2: //listDir(arguments, fd);            break;
        case 3: sendPwd(fd);                       break;
        case 6: removeAllFile(arguments);          break;
        case 7: break;
        default: puts("invalid cmd.");             break;
        }
    }
    else if(cmd_type == 101)
    {
        //长命令
        switch(cmd)
        {
        case 4: sendFile(fd, arguments);           break;
        case 5: recvFromServer(fd, arguments);     break;
        default: break;
        }
    }
}

int cmdClassification(int cmd)
{
    switch(cmd)
    {
    case 1:case 2: case 3:case 6:case 7:return 1;
    case 4:case 5: return 101;
    default:
                   puts("invalid cmd");
                   return 0;
    }
}

