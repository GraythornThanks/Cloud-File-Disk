#pragma once

#define USERNAME_IS_TRUE "1"
#define USERNAME_IS_ERROR "0"
#define PASSWD_IS_TRUE "1"
#define PASSWD_IS_ERROR "0"
#define VALIDATION_SUCCESS "1"
#define VALIDATION_FAILURE "0"

void serverPasswdAuthentication(int fd);    
int clientPasswdAuthentication(int clientfd);
