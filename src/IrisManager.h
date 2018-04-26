/******************************
*    用于插入/查询/删除虹膜。
*  2017/12/28  by cjf[xmu]
*****************************/
#include<stdio.h>
#include "OsiManager.h"
#include "IrisDB.h"
#include "GloFun.h"
#define IRIS_LEN 1536
#define DEBUG
using namespace osiris ;
class IrisManager{
private:
  IrisDB*                            mIrisDB;
	OsiManager                         mMgr;
  std::map<std::string,std::string>* mConf;
	float                              mMatchThreshold;
  //
  std::vector<int>                   mShift;    //要偏移的像素点。在query中用到
  std::vector<std::string>           mFeatures; //得到的所有多组shift后的特征
public:
	IrisManager(char* confPath);
	void showConf();                            //显示配置信息
	int  getDBSize();                           //读取数据库有多少项

	int ins(std::string& irisPath);             //插入一张图片
	int query(std::string& irisPath,float* psco);  //查找最佳匹配,找到返回其ID，找不到返回-1
  int del(int id);                            //删除一张图片，成功返回0，失败返回其他值。
};
int IrisManager::getDBSize(){
  return mIrisDB->size();
}
int IrisManager::ins(std::string& irisPath){
  //获取特征码
	std::vector<int> shift;shift.push_back(0);  //只需计算原本的特征
	mFeatures.clear();
	if (mMgr.extractFeature(irisPath,shift,mFeatures)!=0)  return -1;
	//保存到IrisDB
  TIME_START
	int id=mIrisDB->ins(mFeatures[0]);
	SHOW_TIME("保存到数据库");
	return id;
}
int IrisManager::del(int id){
  return mIrisDB->del(id);
}
int IrisManager::query(std::string& irisPath,float* pScore=NULL){
	mFeatures.clear();
	if (mMgr.extractFeature(irisPath,mShift,mFeatures)!=0) return -1;
	//查找最佳匹配
	TIME_START
  int ret=-1;
	float min_score=1.0;
	for (int i=0;i<mShift.size();i++){
	  std::pair<int,int> rst=mIrisDB->query(mFeatures[i]);
		int   id=rst.first;
		if (id==-1) return -1; //出错(比如数据库为空)
		float score=1.0*rst.second/mFeatures[i].length();

		if (min_score > score){
		  min_score=score;
			ret=id;
#ifndef DEBUG 
		  if (min_score<=mMatchThreshold)break;
#endif
		}
	}
	SHOW_TIME("查询数据库");
	if (pScore) *pScore=min_score;
	return ret;
}

IrisManager::IrisManager(char* confPath){
  mMgr.loadConfiguration(confPath);
	mConf= &mMgr.mOtherConf;
	//mShift
	mShift.push_back(0);
  for (int i=1;i<=10;i++){
	  mShift.push_back(i);
		mShift.push_back(-i);
	}
	//数据库路径
	char* dbPath;
	if (mConf-> find(std::string("sqlite3DB"))==mConf->end()){
		std::cout<<"[!]未配置数据库路径sqlite3DB"<<std::endl;
		exit(-1);
	}else
		dbPath=(char*)((*mConf)["sqlite3DB"].c_str());
  //Matching 阙值
	if (mConf->find(std::string("matchThreshold")) != mConf->end()){
		char* tmp=(char*)((*mConf)["matchThreshold"].c_str());
	  mMatchThreshold=atof(tmp);
	}else
	  mMatchThreshold=0.32;
	mIrisDB = new IrisDB(dbPath,IRIS_LEN);
}
void IrisManager::showConf(){
  printf("====配置项=====\n"); 
	printf("  [*]数据库中保存 %d 条信息\n",mIrisDB->size());
	printf("  [*]匹配阙值为 %f\n",mMatchThreshold);
	printf("===============\n\n");
}
