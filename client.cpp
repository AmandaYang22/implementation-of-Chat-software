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
	inet_pton(AF_INET,"127.0.0.1",&client_addr.sin_addr);//�������е�p��n�Ǳ�����presentation������ֵ��numeric����
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


void * ForRead(void *arg)//void*���Դ����������͵�ָ��,����ֵҲ�������������͵�ָ��
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
����ֵ��
���ɹ�������0�����򣬷��ش������

����˵����

tidp���´������߳�ID�ᱻ���ó�tidpָ����ڴ浥Ԫ��
attr�����ڶ��Ƹ��ֲ��ܵ��߳����ԣ�Ĭ��ΪNULL
start_rtn���´������̴߳�start_rtn�����ĵ�ַ��ʼ���У��ú���ֻ��һ��void���͵�ָ�������arg�����start_rtn��Ҫ������������Խ���������һ���ṹ�У�Ȼ�󽫽ṹ�ĵ�ַ��Ϊarg���롣
*/
