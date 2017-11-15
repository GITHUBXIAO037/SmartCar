/*
int send_data(int fd,char* filename){

	int readfd = open(filename,O_RDONLY);	
	char buf[1024*1024] = {0};
	int ret = 0;
	int nbit = 0;
	while(1){
		nbit = read(readfd,buf,sizeof(buf));
		printf("bit :%d\n",nbit );
		if(nbit == -1){

			printf("%s\n", "data send over");
			break;
		}
		if(nbit == 0){
			printf("%s\n", "data send end");
			break;
		}
		ret = write(fd,buf,nbit);
	}
	return 0;
}
*/


/*int send_data(int fd,char* filename){
	//printf("%s line:%d fd=%d\n",__FUNCTION__,__LINE__, fd);

	int readfd = open(filename,O_RDONLY);
	// send data
	char buf[1024] = {0};
	int ret = 0;
	int n = 0;

	while((n = read(readfd,buf,sizeof(buf))) > 0){

		// printf("%s fd=%d\n",__FUNCTION__, fd);
		ret = write(fd,buf,n);
		// printf("%s ret=%d\n",__FUNCTION__, ret);
		if (ret == -1)
		{
			perror("send");
			break;
		}
	
	}
	close(fd);
	return 0;
	send_protocol(fd,filename);
}
*/
// recv and send
int sendmsg_test(int fd){
	printf("%s\n", "send...");
	char buf[123] = {0};
	// sprintf(buf,"%s","name");
	int ret = 0;
	while(1){
		memset(buf,0,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);
		sleep(1);
		printf("%s\n", "1");

		ret = send(fd,buf,sizeof(buf),0);

		printf("%s\n", "2");
		if(ret < 0){
			printf("%s\n", "send erro\n");
			break;
		}
		else if (ret == 0)
		{
			printf("%s\n", "client close");
		}

		printf("%s\n", "3");
	}
	close(fd);
	return 0;
}

int recvmsg_test(int fd){
	printf("%s\n", "recv...");
	char buf[123] = {0};
	
	int ret = 0;
	while(1){
		sleep(1);
		printf("%s\n", "1");

		ret = recv(fd,buf,sizeof(buf),0);
		if(ret < 0){
			printf("%s\n", "recv erro\n");
			break;
		}
		printf("%s\n", buf);
		printf("%s\n", "2");
	}

	close(fd);
	return 0;
}

int send_protocol(int fd,char* filename){
	int filefd = open(filename,O_RDONLY);

	if (filefd < 0)
	{
		printf("%s\n", "file open error");
		return ERR;
	}
	
	int len = fileSize(filename);

	char protocol[20]={0};

	sprintf(protocol,"%dcar",len);


	printf("%s\n", protocol);
	printf("%d\n", strlen(protocol));

	int ret = write(fd,protocol,sizeof(protocol));
	if(ret < 0){
		perror("send error");
		return ERR;
	}

	printf("file size: %d\n", len);
	close(filefd);

	return 0;
}

int fileSize(char* filename){

	struct stat fileStat;
	
	int ret;
	ret = stat(filename,&fileStat);
	if(ret == -1){
		perror("stat");
		return -1;
	}

	int size;
	size = fileStat.st_size;

	return size;
}