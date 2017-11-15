#ifndef __ARMserver__H
#define __ARMserver__H 1 

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>

#define ERR -1
typedef struct sockaddr_in SockAddr;

int init_server();
void server();

int recvmsg_test(int fd);
int sendmsg_test(int fd);

#endif