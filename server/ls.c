#include "../include/all.h"


int listDir(char* dirpathname, int peerfd)
{
    DIR* fdir;
    if(!strlen(dirpathname)){
        fdir = opendir(".");
    }else{

        fdir = opendir(dirpathname);
    }

    if(fdir == NULL){
        char buf[] =  "Error in ls: opendir failed\n";
        sendMsg(peerfd, buf);
        return -1;
    }

    struct dirent* pdirent;
    struct stat statbuf;

    char list_msg[2048] = {0};
    while(pdirent = readdir(fdir)){
        //跳过以.开头的文件
        if(pdirent->d_name[0] == '.'){
            continue;
        }


        strcat(list_msg, pdirent->d_name);
        strcat(list_msg, "\t");
    }
    strcat(list_msg, "\n");
    closedir(fdir);
    sendMsg(peerfd, list_msg);
    return 0;
}
