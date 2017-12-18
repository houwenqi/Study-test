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
	//接收消息
	received = recv(socket,buffer,BUFFSIZE,0);
	if( received <0 ){
		Exit("Failed to receive initial bytes from client");
	}
	//发送字节，并在循环中检查是否有更多进来的数据
	while(received >0 ){
		//把接收的数据重新发送给客户端
		if(send (socket,buffer,received,0)!=received){
			Exit("Failed to send bytes to client");
		}
		//检查更多进来的数据
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
	//创建TCP Socket
	socket_fd = socket( AF_INET,SOCK_STREAM,0);
	if ( socket_fd == -1 )
	{
		Exit("Failed to create socket\n");	
	}
	//构造地址族结构
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY，让系统自动获取本机IP地址
	serveraddr.sin_port = htons (DEFAULT_PORT); //设置端口为DEFAULT_PORT

	//将地址族绑定到套接字上
	recode = bind(socket_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if( recode == -1 )
	{
		Exit("Failed to bind the server socket");
	}

	//监听是否有客户端连接
	recode = listen(socket_fd,MAXPENDING);
	if( recode == -1 )
	{
		Exit("Failed to listen on server socket");
	}
	printf("=====waiting for client's request=====\n");

	//一直运行，直到取消
	while(1)
	{
		//阻塞直到有客户端连接，节省CPU资源
		unsigned int clientlen = sizeof(clientaddr);
		connect_fd = accept(socket_fd,(struct sockaddr *)&clientaddr,&clientlen);
		printf("connect_fd is %d\n",connect_fd);
		if( connect_fd == -1 )
		{
			printf("accept socket error :%s(errno:%d)",strerror(errno),errno);
			continue;
		}
		//fprintf(stdout, "Client connected: %s\n", inet_ntoa(clientaddr.sin_addr)); 
		
		//处理客户端连接
		HandleClient(connect_fd);
	}
}  
