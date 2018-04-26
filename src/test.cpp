#include "IrisDB.h"
char dbpath[20]="../IrisDB/iris.db";
int main(){
  printf("操作: del 10\n,ins 10100000\n,query 10100000\n q ");
	char op[10];
	int id;
	int rst;
	char code[10];
	std::string tmp;
	IrisDB* irisDB=new IrisDB(dbpath,8);
	while (1){
	  scanf("%s",op);
		switch (op[0]){
			case 'i':scanf("%s",code);
			    tmp=std::string(code);
			    id=irisDB->ins(tmp);
					printf("Insert Id=%d\n",id);
					break;
			case 'd':scanf("%d\n",&id);
			    rst=irisDB->del(id);
					if (rst==0)printf("delete sucess\n");else printf("delete failed!\n");
					break;
			case 'q':scanf("%s",code);
			    tmp=std::string(code);
					std::pair<int,int> xx=irisDB->query(tmp);
					printf("Id=%d Dist=%d\n",xx.first,xx.second);
		      break;
		}
	}
	return 0;
}
