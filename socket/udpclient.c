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
	//����UDP socket
	sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if( sock < 0 ){
		Exit("Failed to create socket");
	}
	//�����ַ��
	memset(&echoserver,0,sizeof(echoserver));
	echoserver.sin_family = AF_INET;
	echoserver.sin_addr.s_addr = inet_addr(argv[1]);
	echoserver.sin_port = htons(atoi(argv[3]));

	//�������������Ϣ
	echolen = strlen(argv[2]);
	retcode = sendto(sock,argv[2],echolen,0,(struct sockaddr *)&echoserver,sizeof(echoserver));
	if( retcode != echolen ){
		Exit("Mismatch in number of sent bytes");
	}

	//�ӷ������Ľ�����Ϣ
	fprintf(stdout,"Received:");
	clientlen = sizeof(echoclient);
	retcode = recvfrom(sock,buffer,BUFFSIZE,0,(struct sockaddr *)&echoclient,&clientlen);
	if( retcode != echolen ){
		Exit("Received a packet from an unexpected server");
	}

	//�����յ���Ϣ�Ƿ�������ȷ�ķ�����
	if( echoserver.sin_addr.s_addr != echoclient.sin_addr.s_addr ){
		Exit("Received a packet from an unexpected server");
	}

	buffer[received] = '\0';
	fprintf(stdout,buffer);
	fprintf(stdout,"\n");

	//�ر��׽���
	close(sock);
	exit(0);
	
}
