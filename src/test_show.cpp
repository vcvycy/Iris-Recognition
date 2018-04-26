#include "IrisDBLow.h"
char iriscode[2050];
char dbPath[50]="../IrisDB/iris.db";
int main(){
	IrisDBLow a(dbPath);
	std::map<int,std::string> ns;
  a.readAll(&ns);
	printf("sz=%d\n",ns.size());
  for (std::map<int,std::string>::iterator it=ns.begin();it!=ns.end();it++){
		std::cout<<"  "<<(*it).first;
  }
  return 0;
}
