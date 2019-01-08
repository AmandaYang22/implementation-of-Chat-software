#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

int init(struct sockaddr_in addr);
void * ForRead(void *arg);
void *ForWrite(void *arg);


int main(int argc,char *argv[])
{
	pthread_t rid,wid;
	int sockfd,ret;
	struct sockaddr_in client_addr;
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(8000);
	inet_pton(AF_INET,"127.0.0.1",&client_addr.sin_addr);//函数名中的p和n非别代表表达（presentation）和数值（numeric）。
	sockfd=init(client_addr);
	pthread_create(&rid,0,ForRead,&sockfd);
	pthread_create(&wid,0,ForWrite,&sockfd);
	pthread_join(rid,0);
	pthread_join(wid,0);
	close(sockfd);
	return 0;	
}

int init(struct sockaddr_in addr)
{
	int sockfd;
	int ret;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	ret=connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	return sockfd;
}
//connect(client_fd, (struct sockaddr*)&server_addr, socklen)


void * ForRead(void *arg)//void*可以传入任意类型的指针,返回值也可以是任意类型的指针
{
	char talk[20];
	int *sockfd=(int *)arg;
	while(1)
	{
		memset(talk,0,20);
		fgets(talk,20,stdin);
		send(*sockfd,talk,strlen(talk)+1,0);
	}
}

void *ForWrite(void *arg)
{
	char talk[20];
	int *sockfd=(int *)arg;
	while(1)
	{
		memset(talk,0,20);
		recv(*sockfd,talk,20,0);
		printf("%s",talk);
	}
}








/*int pthread_create(pthread_t *restrict tidp,
                              const pthread_attr_t *restrict attr,
                              void *(*start_rtn)(void *),
                              void *restrict arg);
返回值：
若成功，返回0；否则，返回错误编码

参数说明：

tidp：新创建的线程ID会被设置成tidp指向的内存单元。
attr：用于定制各种不能的线程属性，默认为NULL
start_rtn：新创建的线程从start_rtn函数的地址开始运行，该函数只有一个void类型的指针参数即arg，如果start_rtn需要多个参数，可以将参数放入一个结构中，然后将结构的地址作为arg传入。
*/
