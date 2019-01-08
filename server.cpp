#include <unistd.h>
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#define SERVER_PORT 8000
using namespace std;

int init(struct sockaddr_in addr);
int Link(int sockfd);

typedef struct Group{
	int groupID;
	int member[10];
}group;

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serveraddr.sin_port=htons(SERVER_PORT);
	sockfd=init(serveraddr);
	if(sockfd==-1)
	{
		printf("init error\n");
		return -1;
	}
	printf("init success!\n");
	Link(sockfd);
	close(sockfd);
	return 0;
}


int init(struct sockaddr_in addr)
{
	int sockfd;
	int ret;//返回值，记录错误码
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
		return -1;
	ret=bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret==-1)
		return -1;
	listen(sockfd,10);
	return sockfd;
}

int Link(int sockfd)
{
	int ret,T=-1;
	int Maxfd;
	int acceptRet,id;
	struct sockaddr_in acceptaddr;
	socklen_t len;//地址大小
	char *p=NULL,*pt=NULL,*pr=NULL;
	char temp[500],array[500],tmp[500];
	char buf[500]="";//接受到的消息内容
	int recvsock;//recv中用到的文件描述符号
	int client[10];//存储连接的文件描述符
	int i,j=0,k=0,m=0,flag=0,mark=-1,n=0,INDX;
	int initGroup[100]={1,2,3,4,5,6,7,8,9,10};//初始设置了几个组  （除了这些组其他组都需要自己创建）
	int recvGroupID,recvPrivateID;
	group allGroup[10];
	for(int index=0;index<10;index++)
	{
		allGroup[index].groupID=index;
		for(int t=0;t<10;t++)
			allGroup[index].member[t]=-1;
	}
	fd_set refd,allfd;
	for(i=0;i<10;i++)
		client[i]=-1;
	FD_ZERO(&refd);
	FD_ZERO(&allfd);
	Maxfd=sockfd;
	FD_SET(sockfd,&allfd);//
	while(1)
	{
	   refd=allfd;
	   ret=select(Maxfd+1,&refd,0,0,0);
	   if(ret==-1)return -1;
	   if(FD_ISSET(sockfd,&refd))
	   {
	      cout<<endl;
	      cout<<"someone connect  ";
	      len=sizeof(acceptaddr);
	      acceptRet=accept(sockfd,(struct sockaddr*) &acceptaddr,&len);
	      cout<<"sockfd:"<<acceptRet;
	      j=0;
	      while(j<10 && client[j]!=-1)++j;
	      client[j]=acceptRet;
	    //cout<<"client["<<j<<"]: "<<client[j]<<endl;
	      Maxfd=Maxfd>acceptRet?Maxfd:acceptRet;
	      FD_SET(acceptRet,&allfd);//
	   }

	   for(i=0;i<10;i++)
	   {
	      if((recvsock=client[i])<0)continue;
	      
	      if(FD_ISSET(recvsock,&ruefd))
              {
		      if((j=recv(recvsock,buf,20,0))==0)
		      {
			     printf("someone exit\n");
			     close(recvsock);
			     FD_CLR(recvsock,&allfd);
			     client[i]=-1;
		      }
		      else
		      {
			     p=strtok(buf,":");
			     m=0;
			     while(p!=NULL)
			     {
				m++;
				if(m%2!=0)
					recvGroupID=atoi(p);
				else
				{
					strcpy(temp,p);
				}
				p=strtok(NULL,":");
			     }   
			     //cout<<"groupID:"<<recvGroupID<<endl;
			    // cout<<"temp:"<<temp<<endl; 
			     
			     pr=strtok(temp," ");
			     n=0;
			     while(pr!=NULL)
			     {
				n++;
				if(n%2!=0)
					strcpy(array,pr);
				else
					strcpy(buf,pr);
				pr=strtok(NULL," ");	
			     }
			    // cout<<"array:"<<array<<endl;
			     //cout<<"buf:"<<buf<<endl;
			     for(int t=0;t<10;t++)
			     {
					if(initGroup[t]!=recvGroupID)
					{	
						if(t==9)
						{
							cout<<endl;
							mark=1;
							//cout<<"mark:"<<mark<<endl;
							cout<<"the group "<<recvGroupID<<" you want to join doesn't exist"<<endl;
						}
						continue;
					}
					else
					{
						cout<<endl;
						cout<<"the group "<<recvGroupID<<" you want to join  exists"<<endl;
						mark=0;
						for(int h=0;h<10;h++)
						{
							if(allGroup[recvGroupID].member[h]==recvsock)
							{
								INDX=1;
								cout<<"you have joined this group"<<endl;	
								break;
							}
						}
						if(INDX!=1)//这个用户不在这个群组
						{
							cout<<"You were not in this group before, but you just joined the group."<<endl;
							for(int h=0;h<10;h++)
							{
								if(allGroup[recvGroupID].member[h]==-1)
								{
									allGroup[recvGroupID].member[h]=recvsock;
									break;
								}
							}
						}
						//cout<<"mark:"<<mark<<endl;
						break;
					}
			    }
			    if(mark==0)
			    {
				cout<<"The  groupID"<<recvGroupID<<" has member:";
				 
				for(int t=0;t<10;t++)
				{
				    if(allGroup[recvGroupID].member[t]!=-1)
					cout<<allGroup[recvGroupID].member[t]<<"  ";
				}
				cout<<endl;
				char *s=NULL;
				s=array;
				//cout<<"s:"<<s<<endl;
				int strcmpRet=strcmp(s,"groupChat");				
				//cout<<"strcmpRet:"<<strcmpRet<<endl;
				if((strcmp(s,"groupChat"))==0)//群聊
				{
					cout<<"this is a group-chat message\n";
					printf("%s",buf);
				      // cout<<"send group message"<<endl;
				        for(int t=0;t<10;t++)
				        {
					   if(allGroup[recvGroupID].member[t]!=recvsock)
				       	   {

					   //printf("%d ,",client[i]);
					  // cout<<"recvGroupID: "<<recvGroupID<<endl;
					//   cout<<"recvsock:"<<recvsock<<endl;
					   send(allGroup[recvGroupID].member[t],buf,100,0);
				           //cout<<endl;
					   }
				       }	
				}
				else
				{
					cout<<"this is a private-chat message\n";
					/*pt=strtok(temp," ");
					
					while(pt!=NULL)
					{
						k++;
						if(k%2!=0)
							recvPrivateID=atoi(pt);
						else
							strcpy(tmp,pt);
						pt=strtok(NULL," ");
					}*/	
					recvPrivateID=atoi(array);
					cout<<"the private-Chat-object ID:"<<recvPrivateID<<endl;
					send(recvPrivateID,buf,500,0);
				}
					
			   }
			   mark=-1;
		         }
	             }
	         }
        }
	return 0;	 
}



