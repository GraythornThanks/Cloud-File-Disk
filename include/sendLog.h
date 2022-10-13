#pragma once

typedef enum {
    FUNC_FAILURE = -1,
    FUNC_SUCCESS = 0,
    FUNC_NOARGS = 1,
} FuncStatus;

void sendConnectLog(const char* prog_name, int clientid);
void sendLogMsg(const char* prog_name, int clientid, const char* msg, FuncStatus funcstat);
