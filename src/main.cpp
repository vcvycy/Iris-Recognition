#include "IrisManager.h"

#include<sys/time.h>
#include "GloFun.h"
int main (int argc, char * argv[]){ 
    //(1) load config file[conf
    char* configPath=(char*)"../conf/"; 
    IrisManager mgr(configPath); //参数为配置文件的路径
    mgr.showConf(); 
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
    return 0;
}
