/*****************************
*    Iris数据库操作底层类(直接修改sqlite3数据库，增删查)
*    约定：函数成功返回0，失败要么退出程序(无法访问数据库)，要么返回错误代码(>0)
*    2017/12/24 by cjf [xmu]
*******************************/

#ifndef __IRIS_DB_LOW__
#define __IRIS_DB_LOW__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <sqlite3.h>
class IrisDBLow{       
private:
	char*    m_pDBPath;
  sqlite3* m_sqlite3;
  char     m_buf[4096];
  int      conn();                                //连接数据库m_pDBPath
  //sqlite3 callback function
  static int s_cbReadAll(void* p,int nColumnNum,char** pColumnText,char** pColumnName);
	static int s_cbReadId(void* p,int nColumnNum,char** pColumnText,char** pColumnName);
public:
	IrisDBLow(char* pDBPath);                       //sqlite3数据库文件地址
	~IrisDBLow();                                   //关闭数据库
	int ins(char* pIrisCode,int nLen);      //添加一个
	int readAll(std::map<int,std::string>* mIrisCode); //读取所有
	int del(int nId);                       //删除一个
  int readId(char* pIrisCode);		
};
int IrisDBLow::conn(){
  char* zErrMsg=0;
	int rc;
	if (rc=sqlite3_open(m_pDBPath,&m_sqlite3)){
	  printf("[!]无法打开数据库[%s],程序退出",m_pDBPath);
		exit(-1);
	}
	return 0;
}

int IrisDBLow::s_cbReadAll(void* p,int nColumn,char** pColumnText,char** pColumnName){
	std::map<int,std::string>* ns=(std::map<int,std::string>*)p;
  if (nColumn!=2){printf("[*]nColumn!=2，未知错误");exit(-1);}
  (*ns)[atoi(pColumnText[0])]=std::string(pColumnText[1]);
  return 0;
}

IrisDBLow::IrisDBLow(char *pDBPath){
  m_pDBPath=pDBPath;
  conn();
}

IrisDBLow::~IrisDBLow(){
  sqlite3_close(m_sqlite3);
}

int IrisDBLow::ins(char* pIrisCode,int nlen){
  pIrisCode[nlen]=0;
  char *zErrMsg=NULL;
  int rc;
	sprintf(m_buf,"insert into iriscode (iriscode)values(\"%s\");",pIrisCode);
	rc=sqlite3_exec(m_sqlite3,m_buf,s_cbReadAll,0,&zErrMsg);
  if (rc!=SQLITE_OK){
	   printf("[!]sql 执行失败:%s\n",zErrMsg);
		 exit(-1);
	}else{
	   return readId(pIrisCode);
	}
}

int IrisDBLow::readAll(std::map<int,std::string>* mIrisCode){
  char *zErrMsg=NULL;
  int rc;
  sprintf(m_buf,"select * from iriscode");
  rc=sqlite3_exec(m_sqlite3,m_buf,s_cbReadAll,mIrisCode,&zErrMsg);
  if (rc!=SQLITE_OK){
     printf("[!]sql 执行失败:%s\n",zErrMsg);
     exit(-1);
  }else{
     //printf("[*]Sql 执行成功 [%s]\n",m_buf);
     return 0;
  }
}

int IrisDBLow::del(int nId){
  char *zErrMsg=NULL;
  int rc;
  sprintf(m_buf,"delete from iriscode where id=%d",nId);
  rc=sqlite3_exec(m_sqlite3,m_buf,0,0,&zErrMsg);
  if (rc!=SQLITE_OK){
     printf("[!]sql 执行失败:%s\n",zErrMsg);
     return -1;
  }else{
     return 0;
  }
}

int IrisDBLow::s_cbReadId(void* p,int nCNum,char** pText,char** pName){
  int *id=(int*)p;
	if (nCNum!=1) return 0;
	*id=atoi(pText[0]);
	return 0;
}
int IrisDBLow::readId(char* pIrisCode){
	int id=-1;
	char* zErrMsg=NULL;
  sprintf(m_buf,"select id from iriscode where iriscode=\"%s\"",pIrisCode);
  int rc=sqlite3_exec(m_sqlite3,m_buf,s_cbReadId,(void*)&id,&zErrMsg);
  if (rc!=SQLITE_OK){
	  printf("[!]sql 执行失败:%s\n",m_buf);
		exit(-1);
	}else{
	  return id;
	}
}
#endif
