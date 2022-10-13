#include "../include/all.h"

int serverHandle(int cmd, int fd, char* arguments)
{
    //命令分类
    int cmd_type = cmdClassification(cmd);
    if(cmd_type == 0)
        return 0;
    else if(cmd_type == 1)
    {
        //短命令
        switch(cmd)
        {
        case 1: sysCd(fd, arguments);              break;
        case 2: listDir(arguments, fd);            break;
        case 3: sendPwd(fd);                       break;
        case 6: removeAllFile(arguments);          break;
        case 7: doMkdir(fd, arguments);            break;
        default: puts("invalid cmd.");             break;
        }
    }
    else if(cmd_type == 101)
    {
        //长命令
        switch(cmd)
        {
        //case 4: recvFile(fd);                      break;
		case 4: recvFile1(fd, arguments);            break;
        //case 5: sendFile(fd, arguments);           break;
		case 5: sendFile1(fd, arguments);            break;
        default: break;
        }
    }
}

int cmd_ctoi(char* cmd)
{
    if(!strcmp(cmd, "cd"))
        return 1;
    else if(!strcmp(cmd, "ls"))
        return 2;
    else if(!strcmp(cmd, "pwd"))
        return 3;
    else if(!strcmp(cmd, "upload"))
        return 4;
    else if(!strcmp(cmd, "download"))
        return 5;
    else if(!strcmp(cmd, "rm"))
        return 6;
    else if(!strcmp(cmd, "mkdir"))
        return 7;
    else
    {
        puts("Invalid command.");
        return -1;
    }
}

char* cmd_itoc(int cmd)
{
    if(cmd == 1)
        return "cd";
    if(cmd == 2)
        return "ls";
    if(cmd == 3)
        return "pwd";
    if(cmd == 4)
        return "upload";
    if(cmd == 5)
        return "download";
    if(cmd == 6)
        return "rm";
    if(cmd == 7)
        return "mkdir";

    return "Invalid command";
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

