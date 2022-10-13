#include "../include/all.h"

void doMkdir(int peerfd, const char * cmd_name) {
    char mkdir_name[256] = {0};
    sscanf(cmd_name, "%s", mkdir_name);
    int flag;
    if(opendir(mkdir_name) == NULL){
        flag = mkdir(mkdir_name, 0777);
        if(flag == -1){
            sendMsg(peerfd, "Error in mkdir : mkdir failed\n");
        } else {
            sendMsg(peerfd, "mkdir complete");
        }
    } else {
        sendMsg(peerfd, "Error in mkdir : folder exist\n");
    }
}
