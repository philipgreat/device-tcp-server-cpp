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
0x1017	唯一标识在系统中不存在	适用于开通终端
0x1018	系统主动拒绝了终端的登录	适用于开通终端
0x1019	系统有致命错误而拒绝了终端登录	适用于开通终端
0x1003	终端状态错误，已经是开通状态	适用于开通终端

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
0x1020	唯一标识在系统中不存在	适用于退租终端
0x1021	系统从没收到过终端的开通请求	适用于退租终端
0x1022	系统有致命错误而拒绝了终端退租	适用于退租终端
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
