#include "serial_init.h"
#include "ARMserver.h"
#define serverip "192.168.1.1"

#define STOP "0"
#define FRONT "1"
#define BACK "2"
#define LEFT "3"
#define RIGHT "4"

#define LEFT_FRONT "5"
#define LEFT_BACK "6"
#define RIGHT_FRONT "7"
#define RIGHT_BACK "8"


#define HORIZON_ROTATE_LEFT "HL"
#define HORIZON_ROTATE_RIGHT "HR"
#define VETICAL_ROTATE_VU "VU"
#define VETICAL_ROTATE_VD "VD"

int init_server(){
	//create a socket
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if (fd < 0)
	{
		perror("socket erro");
		return ERR;
	}	
	printf("%s\n","create..." );

	int on = 1;
	if (0 > setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	SockAddr sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(5959);
	sa.sin_addr.s_addr = inet_addr(serverip);

	//bind
	if(bind(fd,(struct sockaddr*)(&sa),sizeof(sa))<0){
		perror("bind erro");
		return ERR;
	}
	printf("%s\n", "bind...");

	//listen
	if (listen(fd,360) < 0)
	{
		perror("listen");
		return ERR;
	}
	printf("%s\n", "listen...");

	//accept
	SockAddr client;
	int clientlen = sizeof(client);
	memset(&client,0,clientlen);
	int connfd = accept(fd, (struct sockaddr*)&client, &clientlen);
	if ( connfd < 0)
	{
		perror("accept");
		return ERR;
	}
	printf("%s\n", "accept...");

	return connfd;

}

int action_stop(int fd){
	char stop[5] = {0xFF,0x00,0x00,0x00,0xFF};
	int n = write(fd,stop,sizeof(stop));
	return n;	
}

int action_forward(int fd){

	char forward[5] = {0xFF,0x00,0x01,0x00,0xFF};
	int n = write(fd,forward,sizeof(forward));

	return n;
}

int action_back(int fd){
	char back[5] = {0xFF,0x00,0x02,0x00,0xFF};
	int n = write(fd,back,sizeof(back));
	return n;	
}

int action_left(int fd){
	char left[5] = {0xFF,0x00,0x03,0x00,0xFF};
	int n = write(fd,left,sizeof(left));
	return n;	
}

int action_right(int fd){
	char right[5] = {0xFF,0x00,0x04,0x00,0xFF};
	int n = write(fd,right,sizeof(right));
	return n;	
}
int action_left_forward(int fd){
	char left_forward[5] = {0xFF,0x00,0x05,0x00,0xFF};
	int n = write(fd,left_forward,sizeof(left_forward));
	return n;	
}

int action_left_back(int fd){
	char left_back[5] = {0xFF,0x00,0x06,0x00,0xFF};
	int n = write(fd,left_back,sizeof(left_back));
	return n;	
}
int action_right_forward(int fd){
	char right_forward[5] = {0xFF,0x00,0x07,0x00,0xFF};
	int n = write(fd,right_forward,sizeof(right_forward));
	return n;	
}
int action_right_back(int fd){
	char right_back[5] = {0xFF,0x00,0x08,0x00,0xFF};
	int n = write(fd,right_back,sizeof(right_back));
	return n;	
}


int action_horizontal(int fd, unsigned char degree){

	char horizontal[5] = {0xFF,0x1,0x7,degree,0xFF};
	int n = write(fd,horizontal,sizeof(horizontal));
	return n;
}

int action_vetical(int fd, unsigned char degree){
	char vetical[5] = {0xFF,0x1,0x8,degree,0xFF};
	int n = write(fd,vetical,sizeof(vetical));
	return n;
}

void server(int actionfd){
	int fd = init_server();
	// send_data(fd,"photo.jpg");

	static unsigned char horizon_rotate = 0x10;
	static unsigned char vetical_rotate = 0x10;

	// printf("%s\n", "server");
	char buf[5] = {0};
	int n = 0;
	while(1){
		sleep(1);
		n = read(fd,buf,sizeof(buf));
		printf("%s\n", buf);

		if (n == 0)
		{
			printf("client is close\n");
			return;
		}
		if (strncmp(buf,STOP,1) == 0)
		{
			action_stop(actionfd);
		}

		else if (strncmp(buf,FRONT,1) == 0)
		{
			printf("forward %s\n",FRONT );
			action_forward(actionfd);
		}

		else if (strncmp(buf,BACK,1) == 0)
		{
			printf("back %s\n",BACK );
			action_back(actionfd);
		}

		else if (strncmp(buf,LEFT,1) == 0)
		{
			printf("left %s\n",LEFT );
			action_left(actionfd);
		}

		else if (strncmp(buf,RIGHT,1) == 0)
		{
			printf("right %s\n",RIGHT );
			action_right(actionfd);
		}
			else if (strncmp(buf,LEFT_FRONT,1) == 0)
		{
			printf("left front %s\n",LEFT_FRONT );
			action_left_forward(actionfd);
		}

		else if (strncmp(buf,LEFT_BACK,1) == 0)
		{
			printf("left back %s\n",LEFT_BACK );
			action_left_back(actionfd);
		}

		else if (strncmp(buf,RIGHT_FRONT,1) == 0)
		{
			printf("right front %s\n",RIGHT_FRONT );
			action_right_forward(actionfd);
		}

		else if (strncmp(buf,RIGHT_BACK,1) == 0)
		{
			printf("right back %s\n",RIGHT_BACK );
			action_right_back(actionfd);
		}
/*******************camera rotate******************************/

// #define HORIZON_ROTATE_LEFT "HL"
// #define HORIZON_ROTATE_RIGHT "HR"
// #define VETICAL_ROTATE_VU "VU"
// #define VETICAL_ROTATE_VD "VD"

		else if (strncmp(buf,HORIZON_ROTATE_LEFT,2) == 0)
		{
			printf("HORIZON_ROTATE_LEFT %s\n",buf );

			horizon_rotate -= 0x10;
			if (horizon_rotate < 0x0)
			{
				horizon_rotate = 0x0;
			}
			action_horizontal(actionfd,horizon_rotate);
		}
		
		else if (strncmp(buf,HORIZON_ROTATE_RIGHT,2) == 0)
		{
			printf("HORIZON_ROTATE_RIGHT %s\n",buf );
			horizon_rotate += 0x10;
			if (horizon_rotate > 0xB4)
			{
				horizon_rotate = 0xB4;
			}
			action_horizontal(actionfd,horizon_rotate);
		}

		else if (strncmp(buf,VETICAL_ROTATE_VU,2) == 0)
		{
			printf("VETICAL_ROTATE_VU %s\n",buf );

			vetical_rotate += 0x10;
			if (vetical_rotate > 0xB4)
			{
				vetical_rotate = 0xB4;
			}
			action_vetical(actionfd,vetical_rotate);
		}

		else if (strncmp(buf,VETICAL_ROTATE_VD,2) == 0)
		{
			printf("VETICAL_ROTATE_VD %s\n",buf );

			vetical_rotate -= 0x10;
			if (vetical_rotate < 0x0)
			{
				vetical_rotate = 0x0;
			}
			action_vetical(actionfd,vetical_rotate);
		}

	}

	close(fd);	
}


int main(int argc, char const *argv[])
{
	int fd = serial_init0(0);
	printf("%s\n", "serial_init ok");
	server(fd);
	return 0;
}
/*
	小车指令数据协议:
	stop:			FF 00 00 00 FF
	forward:		FF 00 01 00 FF
	back:			FF 00 02 00 FF
	left:			FF 00 03 00 FF
	right:			FF 00 04 00 FF
	left-forward:	FF 00 05 00 FF
	left-back:		FF 00 06 00 FF
	right-forward:	FF 00 07 00 FF
	right-back:		FF 00 08 00 FF

	舵机:
	level:			FF 01 07 0x3C FF 60
	vetical:		FF 01 08 0x78 FF 120
	5a 90
*/
