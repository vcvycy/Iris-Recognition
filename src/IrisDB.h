/************************************
*    高层数据库访问
*    (1)通过IrisDBLow.h访问数据库
*    (2)通过KDTREE01.h对查询进行加速
*   提供的接口:
*   (1)构造函数: 初始化(读取sqlite3数据库并建立kd树)
*   (2)ins(string,int):插入一个IrisCode，同时在数据库中和KD树中添加。返回其ID
*   (3)query(string):  查询最近Hamming编码，返回(id,dist),数据库为空，返回id=-1
*   (4)del(id)        :删除
*          2017/12/  by cjf[xmu]
*************************************/
#ifndef __IRIS_DB__
#define __IRIS_DB__

#include<stdio.h>
#include<string.h>
#include "KDTree01.h"
#include "IrisDBLow.h"
#define enumMap(mp,it) for (std::map<int,std::string>::iterator it=mp.begin();it!=mp.end();it++)
class IrisDB{
private:
  int                        m_Dims;    //维度个数
  std::map<int,std::string>  m_Data;    //当前类中保留一份副本
  IrisDBLow*                 m_DBLow;
  KDTREE01*                  m_KD01;
  void                       checkStrValid(std::string& sIrisCode); //检查字符串是否符合要求(长度+0/1)
public:
  IrisDB(char* pDBPath,int nDims);                       //参数sqlite3数据库文件地址
  ~IrisDB();
  int ins(std::string& sIrisCode);
  std::pair<int,int> query(std::string& sIrisCode);
  int del(int nId);
  int size(){return (int)m_Data.size();}
};

IrisDB::IrisDB(char* pDBPath,int nDims){  //维度个数
  m_Dims=nDims;
  m_DBLow=new IrisDBLow(pDBPath);              //连接失败会此函数退出程序
  //读取数据库
  m_DBLow->readAll(&m_Data);
  //判断数据库存储的
  enumMap(m_Data,it) checkStrValid((*it).second);
  m_KD01 =new KDTREE01(m_Data,nDims);
}
IrisDB::~IrisDB(){
  delete m_KD01;
  delete m_DBLow;
}

void  IrisDB::checkStrValid(std::string& s){
  char* p=(char*)s.c_str();
  if (s.length()!=m_Dims){
    printf("[!]字符串%s长度有误\n",p);
    exit(-1);
  }
  for (int i=0;i<m_Dims;i++,p++){
    if (*p!='0'&&*p!='1'){
      printf("[!]字符串存在非0/1字符[%s]\n",p);
      exit(-1);
    }
  }
}
int IrisDB::ins(std::string& sIrisCode){
  checkStrValid(sIrisCode);
  //插入数据库
  char *pIrisCode=(char*)sIrisCode.c_str();
  int id=m_DBLow->ins(pIrisCode,m_Dims);
  //插入KD树
  int rst=m_KD01->ins(sIrisCode,id);
	if (rst!=0){       //插入重复图片
		m_DBLow->del(id);
		return rst;
	}else{
   //维护本地副本
   m_Data[id]=sIrisCode;
   return id;
	}
}
std::pair<int,int> IrisDB::query(std::string& sIrisCode){
  std::pair<int,int> ret;
  if (m_Data.size()==0){  //数据库为空
    ret.first=-1;
    return ret;
  }
	checkStrValid(sIrisCode);
  //在KD树中查询
  return m_KD01->query(sIrisCode);
}
int IrisDB::del(int nId){
  if (m_Data.count(nId)==0){
    return 1;
  }
	//从数据库中删除
	m_DBLow->del(nId);
  //从KD树中删除
  return m_KD01->del(m_Data[nId]);
}
#endif
