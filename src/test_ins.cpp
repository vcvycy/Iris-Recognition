#include "IrisDBLow.h"
 #include <time.h>
   #include <stdlib.h>
   char iriscode[2050];
   char dbPath[50]="../IrisDB/iris.db";
   IrisDBLow a(dbPath);
int  insRnd(){
     int curid=0;
     srand(time(0));
     for (int i=0;i<128;i++){
      short tmp=rand();
      for (int j=0;j<16;j++){
         if ((1<<j)&tmp){
            iriscode[curid++]='1';
        }else iriscode[curid++]='0';
      }  
  }
	if (curid!=2048) exit(-1);
  iriscode[curid]=0;
  return a.ins(iriscode);
}
int main(int argc,char**argv){ 
  if (argc==2){
	  for (int i=0;i<atoi(argv[1]);i++){
		  printf("Id %d\n",insRnd());
		}
	}
  return 0;
} 
