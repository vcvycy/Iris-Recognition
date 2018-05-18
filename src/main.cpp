#include "IrisManager.h"

#include<sys/time.h>
#include "GloFun.h"
//test函数：测试效果
// (1)从../img/first/  读取108张图片插入数据库
// (2)从../img/second/ 读取108张图片在数据库中查询
#define TEST_MODE
int test(IrisManager &mgr){
  printf("=============准确率测试==============\n");
  //插入
#define IMGSZ 108
  std::string file;
  char filename[50];
  int id[110];
  
  for (int i=1;i<=IMGSZ;i++){
    sprintf(filename,"../img/first/%03d_1_1.bmp",i);
    file=std::string(filename);
    id[i]=i+173;
    if (id[i]<0) printf("[!]失败%s\n",filename);else printf("[*]图片%s插入成功,id %d\n",filename,id[i]);
  }
  //查询
  int cnt=0;
  for (int i=1;i<=IMGSZ;i++){
    float sco;
    sprintf(filename,"../img/second/%03d_1_2.bmp",i);
    printf("query %s\n",filename);
    file=std::string(filename);
    //TIME_START
    int match_id=mgr.query(file,&sco);
    //TIME_SHOW
    if (match_id==id[i]){
      printf("[*]匹配成功%d -> %d sco=%f\n",i,match_id,sco);
      cnt++;
    }else printf("[*]faile%d -> %d sco=%f\n",i,match_id,sco);
  }
  printf("===成功率%d/%d===\n",cnt,IMGSZ);
}
int main (int argc, char * argv[]){
    //配置文件路径，只需要传入文件夹，会自动读取文件夹中的conf.ini文件。
    char* confPath=(char*)"../conf/";
    if (argc>2) confPath=argv[1];
    
    //
    IrisManager mgr(confPath);//参数为配置文件的路径
    mgr.showConf();
#ifdef TEST_MODE
    test(mgr);return 0;
#endif
    //
    int  id;
    char op[10];
    char filename[250];
    std::string file;
    bool quit=false;
    while (!quit){
      scanf("%s",op);
      switch(op[0]){
        case 'i': //TIME_START
                  scanf("%s",filename);
                  file=std::string(filename);
                  id=mgr.ins(file);
                  if (id>=0)
                    printf("[*]虹膜图片已编码并存入数据库,id :%d\n",id);
                  else
                    printf("[!]图片%s处理失败\n",file.c_str());
                  //TIME_SHOW
                  break;
        case 'q': scanf("%s",filename);
                  file=std::string(filename);
                  float sco;
                  id=mgr.query(file,&sco);
                  if (id>=0)
                    printf("[*]找到匹配虹膜,id为%d,score=%f\n",id,sco);
                  else
                    printf("[!]数据库中找不到与%s匹配的虹膜或者路径无效!\n",filename);
                  break;
       case 'd':  scanf("%d",&id);
                  if (mgr.del(id)==0)printf("[*]删除成功ID[%d]\n",id);else printf("[!]删除失败ID[%d]\n",id);
                  break;
       case 'z':  quit=true;
                  break;
      }
    }
    /*
    int id=4;
    if (mgr.del(4)==0)printf("[*]删除成功ID[%d]\n",id);else printf("[!]删除失败ID[%d]\n",id);
    std::string file="../img/040_2_1.bmp";
    id=mgr.ins(file);
    if (id>=0)
      printf("[*]虹膜图片已编码并存入数据库,id :%d\n",id);
    else
      printf("[!]图片%s处理失败\n",file.c_str());

    std::string file2="../img/040_2_2.bmp";
    id=mgr.query(file2);
    if (id!=-1)
      printf("[*]找到匹配虹膜,id为%d\n",id);
    else
      printf("[!]数据库中找不到匹配的虹膜!");
    */
    return 0;
}
