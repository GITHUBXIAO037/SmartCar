// 1 create a server
#include "ARMserver.h"
#include "serial_init.h"
#define serverip "192.168.1.1"
// #define serverip "192.168.5.111"
#include "car.h"

int videofd;
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

	printf("set sockopt reuse addr ...\n");

	SockAddr sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(5858);
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

int send_data(int sockfd, int size, char* picbuf){

	int ret = 0;
	int nbit = 0;
	
	char len[20] = {0};
	sprintf(len,"%dlen",size);
	//printf("picture size: %d\n", size);
	//printf("start picture\n" );
	write(sockfd,len,sizeof(len));
	
	int count = 0;
    while(count < size){

		nbit = write(sockfd,picbuf+count,size-count);
		printf("bit :%d\n",nbit );
		if(nbit == -1){

			printf("%s\n", "data send over");
			break;
		}

		if(nbit == 0){
			printf("%s\n", "data send end");
			break;
		}

		count += nbit;
	}

	return 0;
}

void send_picture(int sockfd){
	int i;
	int index,size,ret;
	char *jpeg;
	//bzero(&jpeg,1);
	for ( i = 0; i < 3; ++i)
	{
		dqbuf(videofd,(void **)&jpeg,&size,&index);
		qbuf(videofd,index);
	}

	ret = dqbuf(videofd,(void **)&jpeg,&size,&index);
	if(ret == -1)
	{
		printf("dqbuf error\n");
		return ;
	}
	printf("size %d\n", size);
	
	send_data(sockfd, size, jpeg);

	ret = qbuf(videofd,index);
	if(ret == -1)
	{
		printf("dqbuf error\n");
		return ;
	}

}

void server(){

	int fd = init_server();
	char buf[5] = {0};
	int n = 0;
	while(1){

		n = read(fd,buf,sizeof(buf));

		if (strncmp(buf,"PIC",3) == 0)
		{
			printf("send picture\n");
			send_picture(fd);
		}
		if (n == 0)
		{
			printf("%s\n", "client is closed");	
			return;
		}	
	}
	myexit(videofd);
	
}

int main(int argc, char const *argv[])
{
	printf("%s\n", "OK");
	videofd = open(CAMERA_DEVICE, O_RDWR, 0);
	if (videofd < 0) {
        printf("Open %s failed\n", CAMERA_DEVICE);
        return ;
    }

    init_camera(videofd);
	startcamera(videofd);
	server();

	return 0;
}
