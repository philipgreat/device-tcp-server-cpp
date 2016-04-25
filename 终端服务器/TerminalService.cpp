// TerminalService.cpp: implementation of the TerminalService class.
//
//////////////////////////////////////////////////////////////////////

#include "TerminalService.h"
#include "TerminalParameterDao.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "HiringDao.h"
TerminalService::TerminalService()
{

}

TerminalService::~TerminalService()
{

}
/*
0x1017	Ψһ��ʶ��ϵͳ�в�����	�����ڿ�ͨ�ն�
0x1018	ϵͳ�����ܾ����ն˵ĵ�¼	�����ڿ�ͨ�ն�
0x1019	ϵͳ������������ܾ����ն˵�¼	�����ڿ�ͨ�ն�
0x1003	�ն�״̬�����Ѿ��ǿ�ͨ״̬	�����ڿ�ͨ�ն�

*/
int TerminalService::hire(OLFO *oracle, const char *terminalId)
{
	int ret=HiringDao::checkIfCanBeHired(oracle,terminalId);
	if(ret==-1){		
		return 0x1019;	
	}
	if(ret==-2){
		return 0x1017;
	}
	if(ret==-3){
		return 0x1003;
	}
	if(ret==0){
		return 0x1018;
	}
	ret=HiringDao::hire(oracle,terminalId);
	if(ret<0){
		return 0x1019;
	}
	return 0;
}

int TerminalService::commit(OLFO *oracle)
{
	//
	oracle->Commit();
	return 0;
}
/*
0x1020	Ψһ��ʶ��ϵͳ�в�����	�����������ն�
0x1021	ϵͳ��û�յ����ն˵Ŀ�ͨ����	�����������ն�
0x1022	ϵͳ������������ܾ����ն�����	�����������ն�
*/

int TerminalService::fire(OLFO *oracle, const char *terminalId)
{

	int ret=HiringDao::checkIfCanBeFired(oracle,terminalId);
	if(ret==-1){		
		return 0x1022;	
	}
	if(ret==-2){		
		return 0x1019;	
	}
	if(ret==-3){		
		return 0x1021;	
	}
	if(ret==0){
		return 0x1021;
	}
	ret=HiringDao::fire(oracle,terminalId);
	if(ret<0){
		return 0x1019;
	}
	return 0;
}

int TerminalService::rollback(OLFO *oracle)
{
	oracle->RollBack();
	return 0;
}
#include "TerminalParameterDao.h"
int TerminalService::getTerminalParameter(OLFO *oracle, const char *terminalId,char*params,int length)
{
	return TerminalParameterDao::getParameterExpr(oracle,terminalId,params,length);

}
