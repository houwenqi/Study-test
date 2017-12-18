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
	error(msg);
	exit(1); 
}  

int main(int argc,char **argv)
{
	int sock;
	int received = 0;
	struct sockaddr_in echoserver;
	struct sockaddr_in echoclient;
	char buffer[BUFFSIZE] = "";
	unsigned int echolen,clientlen,serverlen;
	if( argc != 2 ){
		fprintf(stderr,"USAGE:%s<port>\n",argv[0]);
		exit(1);
	}
	//����UDP socket
	sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if( sock < 0 ){
		Exit("Failed to create socket");
	}
	//������ַ��
	memset(&echoserver,0,sizeof(echoserver));
	echoserver.sin_family = AF_INET; //����IP
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY);//����IP��ַ
	echoserver.sin_port = htons(atoi(argv[1]));//�������˿�

	//��socket
	serverlen = sizeof(echoserver);
	if((bind(sock,(struct sockaddr *)&echoserver,serverlen))<0){
		Exit("Failed to bind server socket");
	}
	//����֪��ȡ��
	while(1){
		//�ӿͻ��˽���һ����Ϣ
		clientlen = sizeof(echoclient);
		if((received = recvfrom(sock,buffer,BUFFSIZE,0,(struct sockaddr *)&echoclient,&clientlen))<0){
			Exit("Failed to receive message");
		}
		fprintf(stderr,"Client connected:%s\n",inet_ntoa(echoclient.sin_addr));
		//��ͻ��˷�����Ϣ
		if(sendto(sock,buffer,received,0,(struct sockaddr *)&echoclient,sizeof(echoclient)) != received){
			Exit("Mismatch in number of echo'd bytes");
		}
	}
}   
