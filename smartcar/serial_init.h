#ifndef __serial_init__
#define __serial_init__

#include <stdio.h>      /*标准输入输出定义*/ 
#include <stdlib.h>     /*标准函数库定义*/  
#include <unistd.h>     /*Unix 标准函数定义*/  
#include <sys/types.h>   
#include <sys/stat.h>     
#include <fcntl.h>      /*文件控制定义*/  
#include <termios.h>    /*PPSIX 终端控制定义*/  
#include <errno.h>      /*错误号定义*/  
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int errno;

#include <poll.h>
#include <sys/ioctl.h>

#define FALSE  -1  
#define TRUE   0 

#define eprintf(x)  printf("%s   ===file:%s     ===line:%d   ===%s \n",x,__FILE__,__LINE__,strerror(errno))

int serial_Open(int comport);

void serial_Close(int fd);

int serial_init0(int comport);

int serial_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);

#endif