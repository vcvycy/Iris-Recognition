/***************************************************************
*    0/1二进制KD树
* (1)说明：每一维只有0/1两种取值的KD树。
*    主要用于寻找最近汉明距离。
*    通过某一维度0/1个数差来对维度进行排序(字符串都会进行重组再处理)
*    【发现此时KD树变成了贪心预处理+搜索+剪枝】
* (2) 提供的操作：初始化kd树、插入ins、询问query
*      2017/12/24 by cjf[xmu]
*
****************************************************************/
#ifndef __KD_TREE_01__
#define __KD_TREE_01__
#define __SORT_DIM__                     //是否对维度排序。查看排序与否的效果对比。
#define MAX_FEATURE_SIZE 1536            //特征点个数
#include<stdio.h>
#include<string>
#include<algorithm>
#include<iostream>
#include<stdlib.h>
#include<map>

struct KD_NODE{                           //这个结构体被KDTREE01 class调用
  KD_NODE* ch[2];                         //子节点
  int id;                                 //叶子节点时记录其id。非叶子节点id为-1。
  int cnt;                                //cnt表示当前节点以下共有多少个编码
  KD_NODE(){
    ch[0]=ch[1]=NULL;
    id=-1;
    cnt=0;
  }
  int ins(char* p,int val);                   //插入一个二进制串及其ID
  //查找最近hamming 距离:KD树为空的情况minId返回-1
  //(1)dist表示根节点到当前节点距离，
  //(2)minDist表示当前找到的最近距离，minId表示最近时的ID
  int query(char* p,int& minDist,int& minId,int dist);    //返回当前节点下，搜索的步数
  int release(); //内存释放
  int del(char* p);                           //编码删除，成功返回0，失败返回1
};
int KD_NODE::release(){
  if (ch[0]) {ch[0]->release();delete ch[0];}
  if (ch[1]) {ch[1]->release();delete ch[1];}
}
int KD_NODE::del(char* p){//
    //("NODE DEL %c ch[0]%d ch[1]%d\n",*p,ch[0],ch[1]);
    if (*p==0){  //到达根节点
      cnt--;
      //printf("[*]删除成功Id[%d]\n",id);
      return 0;
    }
    int rst=1;
    if (*p=='0' && ch[0]){
      rst=ch[0]->del(p+1);
      cnt-=(rst==0);      //删除成功减去一个计数
      if (ch[0]->cnt==0){
        delete ch[0];
        ch[0]=NULL;
      }
      return rst;
    }
    else
    if (*p=='1' && ch[1]){
      rst=ch[1]->del(p+1);
      cnt-=(rst==0);      //删除成功减去一个计数
      if (ch[1]->cnt==0){
        delete ch[1];
        ch[1]=NULL;
      }
      return rst;
    }
    //printf("No Way!\n");
    return 1;//无路可走
}
int KD_NODE::ins(char* p,int val){               //插入一个二进制串及其ID,若已经存在，则返回旧的ID。
    if (*p==0) {
      if (id!=-1){
          printf("[*]已存在的编码ID[%d]与当前插入的编码ID[%d]重复，不做任何操作\n",id,val);
          return id;
      }
      id=val;
      return 0;
    }
    int rst;
    if (*p=='0'){
      if (ch[0]==NULL)ch[0]=new KD_NODE();
      rst=ch[0]->ins(p+1,val);
    }else{
      if (ch[1]==NULL)ch[1]=new KD_NODE();
      rst=ch[1]->ins(p+1,val);
    }
    if (rst==0) cnt++;  //0代表成功插入
    return rst;
  }
int KD_NODE::query(char* p,int& minDist,int& minId,int dist=0){ //查找最近hamming 距离。
  if (*p!=0 && ch[0]==NULL&&ch[1]==NULL){       //KD树为空的情况
    minId=-1;
    return 0;
  }
  if (minDist<=dist){         //剪枝
    return 0;
  }
  int cnt=0;
  if (id!=-1){                //到达叶子节点
    minDist=dist;
    minId=id;
    //printf("node qry(%d,%d)\n",minDist,minId);
    return 1;
  }
  if (*p=='0'){              //当前编码为0,先左节点后右节点
     if (ch[0]) cnt+=ch[0]->query(p+1,minDist,minId,dist);
     if (ch[1]) cnt+=ch[1]->query(p+1,minDist,minId,dist+1);
  }else
  if (*p=='1'){              //当前编码为1,先右节点后左节点
     if (ch[1]) cnt+=ch[1]->query(p+1,minDist,minId,dist);
     if (ch[0]) cnt+=ch[0]->query(p+1,minDist,minId,dist+1);
  }
  return cnt+1;
}
class KDTREE01{
private:
  int      m_nDim;          //维数
  KD_NODE* m_pRoot;         //根节点
  int      m_dimMap[MAX_FEATURE_SIZE];  //通过方差来决定哪一维先分割
  char     m_Buf[MAX_FEATURE_SIZE];
  int      initDimMap(std::map<int,std::string>& mData);
  int      transformString(std::string& s);       //将s通过dimMap转化,保存到m_Buf
public:
  KDTREE01(std::map<int,std::string>& mData,int nDims);
  ~KDTREE01();
  int ins(std::string& sIrisCode,int nId);        //插入irisCode
  std::pair<int,int> query(std::string& s);       //找到距离s最近的,返回(id,最近距离)
  int del(std::string& sIrisCode);                //删除一个编码
};
KDTREE01::KDTREE01(std::map<int,std::string>& mData,int nDims){
  //m_nDim=(*mData.begin()).second.length();
  m_nDim=nDims;
  initDimMap(mData);

  m_pRoot=new KD_NODE();
  for(std::map<int,std::string>::iterator it=mData.begin();it!=mData.end();it++){
    //std::cout<<(*it).second<<std::endl;
    ins((*it).second,(*it).first);
  }
}
KDTREE01::~KDTREE01(){
  m_pRoot->release();
}
int KDTREE01::transformString(std::string& s){       //将s通过dimMap转化,保存到m_Buf
  char* p=(char*)s.c_str();
  for (int i=0;i<m_nDim;i++){
    m_Buf[i]=p[m_dimMap[i]];
  }
  m_Buf[m_nDim]=0;
  //printf("trans:%s\n",m_Buf);
}
int KDTREE01::initDimMap(std::map<int,std::string>& mData){
  std::pair<int,int> pii[MAX_FEATURE_SIZE];  //(当前维0-1个数差，维度)
  for (int i=0;i<m_nDim;i++){
    pii[i].first=0;
    pii[i].second=i;
  }
  //pii[i].first记录0的个数
  for (std::map<int,std::string>::iterator it=mData.begin();it!=mData.end();it++){
     char*p =(char*)((*it).second.c_str());
     for (int i=0;i<m_nDim;i++,p++){
        pii[i].first+=(*p=='0');
     }
  }
  //pii[i].first记录01个数差
  for (int i=0;i<m_nDim;i++){
    //printf("dim %d zero num%d\n",i,pii[i].first);
    pii[i].first=(mData.size()-2*pii[i].first);
    if (pii[i].first<0)pii[i].first=-pii[i].first;
  }
  //排序(从小到大排序)，即01个数差更小的，应该放在前面
  #ifdef __SORT_DIM__
  std::sort(pii,pii+m_nDim);
  #endif
  for (int i=0;i<m_nDim;i++){
    m_dimMap[i]=pii[i].second;
    //printf("Dim %d  stddev %d\n",pii[i].second,pii[i].first);
  }
}
int KDTREE01::ins(std::string& sIrisCode,int nId){
  if (sIrisCode.length()!=m_nDim){
    printf("[!]iris code [id%d] 长度不符合要求\n",nId);
    return -1;
  }
  //char *pIrisCode=(char*)sIrisCode.c_str();
  transformString(sIrisCode);
  //printf("ins %s\n",m_Buf);
  return m_pRoot->ins(m_Buf,nId);
}
std::pair<int,int> KDTREE01::query(std::string& sIrisCode){  //
  std::pair<int,int> ret;
  ret.second=0x7fffffff;
  transformString(sIrisCode);

  int step=m_pRoot->query(m_Buf,ret.second,ret.first);
  //printf("step:%d\n",step);
  return ret;
}
int KDTREE01::del(std::string& sIrisCode){
  transformString(sIrisCode);
  //printf("del %s\n",m_Buf);
  return m_pRoot->del(m_Buf);
}
#endif
