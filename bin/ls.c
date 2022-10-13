#include "chang.h"


int listDir(char* dirpathname, int peerfd)
{
<<<<<<< HEAD
    DIR* fdir;
    if(dirpathname[0] == '\0'){
=======
    DIR* fdir ;
    if(strlen(dirpathname) == 0){
>>>>>>> 33df049e25368951b4e292e9c73e92b4e8d27426
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
<<<<<<< HEAD

<<<<<<< HEAD

=======
        
        strcat(list_msg, pdirent->d_name);
        strcat(list_msg, "\t");
=======
>>>>>>> 33df049e25368951b4e292e9c73e92b4e8d27426
        strncat(list_msg, pdirent->d_name, strlen(pdirent->d_name));
        strncat(list_msg, "\t", 1);
>>>>>>> 72c5c7706f7e2923cce136655bdf0f35ce893533
    }
    strcat(list_msg, "\n");
    printf("list_msg = %s , len = %ld\n", list_msg, strlen(list_msg));
    closedir(fdir);
    sendMsg(peerfd, list_msg);
    return 0;
}
