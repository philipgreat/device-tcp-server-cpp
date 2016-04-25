// AdStatsBody.cpp: implementation of the AdStatsBody class.
//
//////////////////////////////////////////////////////////////////////


#include "AdStatsBody.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
AdStatsBody::AdStatsBody()
{
	
}

AdStatsBody::~AdStatsBody()
{

}
/**********************************************************************
 ��������: parse
 ��������: �ѹ����������ݽ�������
 �������:
 �� �� ֵ: 0-����,-1-���������̫����!
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
int AdStatsBody::parse(const char *body)
{
	int length=this->length=strlen(body);
	if(length>1024*16){
		return -1;
	}
	memcpy(this->message,body,length);
	this->message[length]=0;

	char *p=this->message;

	while(*p>0){
		switch(*p){
		case ' ' : *p=0;break;
		case '\r': *p=0;break;
		case '\n': *p=0;break;
		default: break;
		}		
		p++;
	}
	
	this->pos=this->message;

	return 0;
	

}
/**********************************************************************
 ��������: next
 ��������: ȡ���¸�ֵ,����㷨��Ч�ʲ�����,������������Ҫ��
 �������:
 �� �� ֵ: NULL,û����,�ַ���,��ǰ��ֵ
 ȫ�ֱ���:
 ��д����: zhangxilai
 ��д����: 2009��01��23��
 �����־: 
 ��� SQL��
**********************************************************************/
char * AdStatsBody::next()
{
	if(this->pos-this->message>this->length){
		return NULL;
	}
	if(this->pos==this->message){
		//��һ��
		this->pos=this->message+strlen(this->message)+1;
		return this->message;
	}
	char *p=this->pos;
	while(*p==0){
		p++;
		if(p-this->message>this->length){
			return NULL;
		}
	}
	this->pos=p+strlen(p)+1;
	return p;
}

