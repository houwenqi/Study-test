#include<stdio.h>  
#include<stdlib.h>    
#include<string.h>    
#include<errno.h>    
#include<sys/types.h>    
#include<sys/socket.h>    
#include<netinet/in.h>    
    
#define BUFFSIZE 1024  

void Exit(char *msg) {
	perror(msg);
	exit(1); 
}    

int main(int argc,char **argv)
{
	int sockfd,n,rec_len;
	int retcode;
	char recvline[BUFFSIZE],sendline[BUFFSIZE];
	char buf[BUFFSIZE];
	struct sockaddr_in serveraddr;
	if( argc != 2 ){
		printf("usage:./client<ipaddress>\n");
		exit(1);
	}
	//����socket
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if( sockfd < 0 ){
		Exit("Failed to create socket");
	}
	//�����������ַ��
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	if(inet_pton(AF_INET,argv[1],&serveraddr.sin_addr) <= 0 ){
		printf("inet_pton error for %s\n",argv[1]);
		exit(1);
	}

	//���ӷ�����
	retcode = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if( retcode < 0 ){
		Exit("Failed to connect with server");
	}
	printf("send msg to server: \n");

	fgets(sendline,BUFFSIZE,stdin);
	//�������ݵ�������
	retcode = send(sockfd,sendline,strlen(sendline),0);
	if( retcode < 0 ){
		Exit("Send msg error");
	}

	//�ӷ�������������
	rec_len = recv(sockfd,buf,BUFFSIZE,0);
	if( rec_len == -1 ){
		Exit("Receive msg error");
	}
	buf[rec_len] = '\0';

	printf("Received:%s\n",buf);

	//�ر��׽���
	close(sockfd);
	exit(0);
}  
