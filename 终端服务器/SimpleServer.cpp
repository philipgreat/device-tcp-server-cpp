// SimpleServer.cpp: implementation of the SimpleServer class.
//
//////////////////////////////////////////////////////////////////////

#include "SimpleServer.h"
#ifdef  _WIN32
#pragma comment(lib,"ws2_32")
#include <winsock2.h>
#include <windows.h>
#include <winsock.h>
#else
/*berkerly and unix/unix-like systems*/
#include<errno.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h> 
#include<sys/wait.h> 
#include<sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "BlackListDao.h"
#include "WhiteListDao.h"
#include "CommonDao.h"
#include "TerminalParameterDao.h"
#include "AdStatsBody.h"
#include "AdStatsDao.h"
#include "wtdp.h"
SimpleServer::SimpleServer()
{
	
}

SimpleServer::~SimpleServer()
{
	
}

#ifdef _WIN32
#include <windows.h>
static DWORD __stdcall ThreadWork(void *pTask)
{	
	SimpleServer server;
	server.run(pTask);
	return 0;
}

int SimpleServer::start(void *pTask)
{
	unsigned long threadId;
	::CreateThread(NULL,40*1024,ThreadWork,pTask,0,&threadId);
	return 0;
	
}
#else
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
void* ThreadWork(void *pTask)
{	
	SimpleServer server;
	server.run(pTask);
	return NULL;
}

int SimpleServer::start(void *pTask)
{
	pthread_t threadId;
	pthread_create(&threadId,NULL,ThreadWork,pTask);
	return 0;	
}


#endif
#include "ORACLE_OPER.H"
#include "WtdpPacket.h"
#include "TerminalService.h"

static int hireTerminal(OLFO *oracle, WtdpPacket *packet, int clientsocket)
{
	ServiceStartRequest* req=(ServiceStartRequest*)packet->getHead();
	printf("%s\t",req->strTerminalId);		
	printf("%ld\n",req->tTermDate);
	int errorCode=TerminalService::hire(oracle,req->strTerminalId);
				
	if(errorCode!=0){
		packet->packServiceStartResp(req->strTerminalId,errorCode);
		send(clientsocket,packet->getHead(),packet->getLength(),0);
		return errorCode;
	}
	//ServiceStartRequest* req=(ServiceStartRequest*)packet.getHead();
	packet->packServiceStartResp(req->strTerminalId,0);
	send(clientsocket,packet->getHead(),packet->getLength(),0);
	TerminalService::commit(oracle);
	return 0;

}

static int fireTerminal(OLFO *oracle, WtdpPacket *packet, int clientsocket)
{
	ServiceEndRequest* req=(ServiceEndRequest*)packet->getHead();
	int errorCode=TerminalService::fire(oracle,req->strTerminalId);
	printf("%s\t",req->strTerminalId);		
	printf("\n");
	if(errorCode!=0){
		packet->packServiceEndResp(req->strTerminalId,errorCode,0,3);
		send(clientsocket,packet->getHead(),packet->getLength(),0);
		return errorCode;
	}
	packet->packServiceEndResp(req->strTerminalId,0,0,3);
	send(clientsocket,packet->getHead(),packet->getLength(),0);
	TerminalService::commit(oracle);
	return 0;
}
static int sentServerCommandsToTerminal(OLFO *oracle, WtdpPacket *packet, int clientsocket)
{
	//下发系统参数
	DisconnectingRequest* req=(DisconnectingRequest*)packet->getHead();
	char params[1024];
	int errorCode=TerminalService::getTerminalParameter(oracle,req->strTerminalId,params,1024);
	if(errorCode<0){
		//发送断开确认包就可以了
		packet->packDisconnectingResp(req->strTerminalId);
		send(clientsocket,packet->getHead(),packet->getLength(),0);
		return -1;
		//结束会话

	}
	
	if(strlen(params)==0){
		packet->packDisconnectingResp(req->strTerminalId);
		send(clientsocket,packet->getHead(),packet->getLength(),0);
		return -1;
		//发送断开确认包就可以了
	}	
	//发送系统参数	
	packet->packParameterModificationReq(req->strTerminalId,params,strlen(params));
	send(clientsocket,packet->getHead(),packet->getLength(),0);
	printf("=== %s\n",params);
	//接收回复
	recv(clientsocket,packet->getHead(),sizeof(ParameterModificationResponse),0);
	//暂时不处理
	
	packet->packDisconnectingResp(req->strTerminalId);
	send(clientsocket,packet->getHead(),packet->getLength(),0);
	//结束会话
	return -1;
}
static int processRequests(OLFO *oracle,int clientsocket)
{
	WtdpPacket packet;
			
	if(recv(clientsocket,packet.getHead(),12,0)!=12){
		//closesocket(clientsocket);			
		return -1;
		//终止
	}
	//2.读取命令，确定由哪个来取
	PackageHead *p=(PackageHead*)packet.getHead();
	if(p->nLengh<0){	
		//closesocket(clientsocket);
		return -2;
	}
	printf("%d\t0x%04x\t%d\t",p->nLengh,p->nType,p->nSeqId);
	
	int len=packet.getBodyLength();
	if(recv(clientsocket,(char*)packet.getBody(),len,0)!=len){			
		//closesocket(clientsocket);
		printf("error");
		return 1;
		//终止
	}
	//查找命令代码,寻找处理的函数
	if(packet.getCommand()==0x002){	
		//租用包
		return hireTerminal(oracle,&packet,clientsocket);;
	}
	
	if(packet.getCommand()==0x003){	
		//退租包
		return fireTerminal(oracle,&packet,clientsocket);;			
	}
	if(packet.getCommand()==0x017){
		//请求断开包
		return sentServerCommandsToTerminal(oracle,&packet,clientsocket);;			
	}
	return 0;

}
int SimpleServer::run(void *pTask)
{
	OLFO* oracle=(OLFO*)pTask;
	
	int 	serversocket;
	struct sockaddr_in address;
	int 	clientsocket;
	
	if ((serversocket=socket(AF_INET,SOCK_STREAM,0))==0)
	{
		perror("create socket");
		exit(EXIT_FAILURE);
	}	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(7000);
	if (bind(serversocket,(struct sockaddr *)&address,sizeof(address))<0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if (listen(serversocket,3)<0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
#ifdef _WIN32	
	int addrlen=sizeof(address);
#else
	socklen_t addrlen=sizeof(address);
#endif
	printf("length\ttype\tseq\ttermid\t\t\t\ttime\n");//printf("new socket is %d\n",clientsocket);
	while ((clientsocket=accept(serversocket,(struct sockaddr *)&address,&addrlen))>0)
	{		
		while(true){
			int ret=processRequests(oracle,clientsocket);
			if(ret>=0){
				continue;
			}
			break;
			//请求断开包处理			
		}
#ifdef _WIN32	
	closesocket(clientsocket);
#else
	close(clientsocket);
#endif		
	
	}
#ifdef _WIN32	
	closesocket(serversocket);
#else
	close(serversocket);
#endif	

	return 0;
}


void SimpleServer::sleep(int miliseconds)
{
#ifdef _WIN32
	Sleep(miliseconds);
#else
	sleep(miliseconds/1000);	
#endif
}


