#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 4096

void Exit(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc,char **argv)
{
	int sock;
	struct sockaddr_in echoserver;
	struct sockaddr_in echoclient;
	char buffer[BUFFSIZE] = "";
	unsigned int echolen,clientlen;
	int received = 0;
	int retcode;

	if( argc != 4 ){
		fprintf(stderr,"USAGE:%s <server ip> <word> <port> \n",argv[0]);
		exit(1);
	}
	//创建UDP socket
	sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if( sock < 0 ){
		Exit("Failed to create socket");
	}
	//构造地址族
	memset(&echoserver,0,sizeof(echoserver));
	echoserver.sin_family = AF_INET;
	echoserver.sin_addr.s_addr = inet_addr(argv[1]);
	echoserver.sin_port = htons(atoi(argv[3]));

	//向服务器发送消息
	echolen = strlen(argv[2]);
	retcode = sendto(sock,argv[2],echolen,0,(struct sockaddr *)&echoserver,sizeof(echoserver));
	if( retcode != echolen ){
		Exit("Mismatch in number of sent bytes");
	}

	//从服务器的接收信息
	fprintf(stdout,"Received:");
	clientlen = sizeof(echoclient);
	retcode = recvfrom(sock,buffer,BUFFSIZE,0,(struct sockaddr *)&echoclient,&clientlen);
	if( retcode != echolen ){
		Exit("Received a packet from an unexpected server");
	}

	//检查接收的信息是否来自正确的服务器
	if( echoserver.sin_addr.s_addr != echoclient.sin_addr.s_addr ){
		Exit("Received a packet from an unexpected server");
	}

	buffer[received] = '\0';
	fprintf(stdout,buffer);
	fprintf(stdout,"\n");

	//关闭套接字
	close(sock);
	exit(0);
	
}
