#include"../include/all.h"

void sendConnectLog(const char* prog_name, int clientid) {
    openlog(prog_name, LOG_CONS | LOG_PID, 0);
    syslog(LOG_INFO, "client %d has connected.\n", clientid);
    closelog();
}

void sendLogMsg(const char* prog_name, int clientid, const char* msg, FuncStatus funcstat) {

    openlog(prog_name, LOG_CONS | LOG_PID, 0);

    if (funcstat == FUNC_SUCCESS)
        syslog(LOG_INFO, "client %d %s successfully.\n", clientid, msg);
    else if (funcstat == FUNC_FAILURE) 
        syslog(LOG_WARNING, "client %d %s failed.\n", clientid, msg);
    else if (funcstat == FUNC_NOARGS)
        syslog(LOG_INFO, "client %d %s.\n", clientid, msg);
    closelog();

}
