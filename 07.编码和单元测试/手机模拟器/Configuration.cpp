// onfiguration.cpp: implementation of the Configuration class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Configuration.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
Configuration::Configuration()
{
	memset((this->values),0,sizeof(void*)*128);
}

Configuration::~Configuration()
{

}

int Configuration::load(const char *fileName)
{

	FILE *f=fopen(fileName,"r+b");
	if(f==NULL){
		return 1;
	}

	size_t  len;
	int readlen=1024*10;
	//memset(buffer,0,len);
	len = fread(this->buffer,1,readlen,f);
	//printf("len=%d\n",len);
	if(len<=0)
	{
		fclose(f);
		return 2;
	}
	//::ReadFile(saveFile,buffer,len-1,&readlen,NULL);
	*(buffer+readlen)='\0';
	fclose(f);
	return 0;

}

int Configuration::save(const char *fileName)
{
	FILE *f=fopen(fileName,"w+b");
	//write the parameters one by one
	char line[1024];
	sprintf(line,"%s = %s\r\n");	
	fwrite(line,strlen(line),1,f);
	fclose(f);
	return 0;
}
static bool isNameValueToken(char ch)
{
	return ch=='=';
}
static bool isLineToken(char ch)
{
	return ch=='\r'||ch=='\n';

}
enum PARSE_STATE{INIT=0,IN_NAME,BEFORE_VALUE,IN_VALUE,END_VALUE,END};
int Configuration::parse()
{
	int state=INIT;
	char *header=NULL;
	int  parseStrLen=strlen(this->buffer)+1;
	char *p=header=this->buffer;
	char *name=NULL;
	char *value=NULL;
	int currrentIndex=0;
	while(p-header<parseStrLen){	
		switch(state){		
		case INIT:
			if(isalnum(*p)){
				name=p;
				state=IN_NAME;
			}
			break;
		case IN_NAME:
			//do nothing
			if(isNameValueToken(*p)){
				state=BEFORE_VALUE;
				*p='\0';
			}else if (!(isalnum(*p)||*p=='_'||*p=='.'||*p=='-')){
				*p='\0';
			}
			break;
		case BEFORE_VALUE:
			if(isLineToken(*p)){
				value="";
				state=END_VALUE;
				p--;
			}else if(*p==' '||*p=='\t' ){
				//忽略前面的空格等无效字符
			}else {
				value=p;
				state=IN_VALUE;
			}	
			break;
		case IN_VALUE:
			//seperator
			//if(*p=='\r'||*p=='\n'||*p==';'){
			if(isLineToken(*p)){
				*p='\0';
				p--;
				state=END_VALUE;
			}
			break;
		case END_VALUE:
			values[currrentIndex++]=name;
			values[currrentIndex++]=value;
			state=INIT;	
			break;
		default:
			break;
		}
		p++;
	
	}
	return 0;
}

void Configuration::report()
{
	printf("\n%30s\t%20s","Name","Value");
	printf("\n----------------------------------------------------------------");
	int i=0;
	while(values[i*2]!=NULL){
		printf("\n%30s\t%s",values[i*2],values[i*2+1]);
		i++;
	}

	printf("\n");
}

char * Configuration::getValue(const char *name)
{
	int i=0;
	while(values[i*2]!=NULL){
		if(strcmp(values[i*2],name)!=0){
			i++;
			continue;
		}
		return values[i*2+1];
	}
	return NULL;
}
