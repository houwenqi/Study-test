#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
  
#define DEFAULT_PORT 8000  
#define MAXPENDING 10  /* Max connection requests */  
#define BUFFSIZE 1024  
void Exit(char *msg)
{ 
	perror(msg);
	exit(1);
}  

void HandleClient(int socket){
	char buffer[BUFFSIZE];
	int received = -1;
	//������Ϣ
	received = recv(socket,buffer,BUFFSIZE,0);
	if( received <0 ){
		Exit("Failed to receive initial bytes from client");
	}
	//�����ֽڣ�����ѭ���м���Ƿ��и������������
	while(received >0 ){
		//�ѽ��յ��������·��͸��ͻ���
		if(send (socket,buffer,received,0)!=received){
			Exit("Failed to send bytes to client");
		}
		//���������������
		if( (received = recv(socket,buffer,BUFFSIZE,0)) <0 )
		{
			Exit("Failed to receive additional bytes from client");
		}
	}
	close(socket);
}  

int main(int argc,char **argv)
{
	int socket_fd,connect_fd;
	struct sockaddr_in  serveraddr,clientaddr;
	char buff[4096] = "";
	int n;
	int recode;
	//����TCP Socket
	socket_fd = socket( AF_INET,SOCK_STREAM,0);
	if ( socket_fd == -1 )
	{
		Exit("Failed to create socket\n");	
	}
	//�����ַ��ṹ
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP��ַ���ó�INADDR_ANY����ϵͳ�Զ���ȡ����IP��ַ
	serveraddr.sin_port = htons (DEFAULT_PORT); //���ö˿�ΪDEFAULT_PORT

	//����ַ��󶨵��׽�����
	recode = bind(socket_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if( recode == -1 )
	{
		Exit("Failed to bind the server socket");
	}

	//�����Ƿ��пͻ�������
	recode = listen(socket_fd,MAXPENDING);
	if( recode == -1 )
	{
		Exit("Failed to listen on server socket");
	}
	printf("=====waiting for client's request=====\n");

	//һֱ���У�ֱ��ȡ��
	while(1)
	{
		//����ֱ���пͻ������ӣ���ʡCPU��Դ
		unsigned int clientlen = sizeof(clientaddr);
		connect_fd = accept(socket_fd,(struct sockaddr *)&clientaddr,&clientlen);
		printf("connect_fd is %d\n",connect_fd);
		if( connect_fd == -1 )
		{
			printf("accept socket error :%s(errno:%d)",strerror(errno),errno);
			continue;
		}
		//fprintf(stdout, "Client connected: %s\n", inet_ntoa(clientaddr.sin_addr)); 
		
		//����ͻ�������
		HandleClient(connect_fd);
	}
}  
