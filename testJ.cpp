
#include "master.h"
#include "node.h"
#include "worker.h"

class A {
  int a ;
};
int main(int argc, char** argv){
  
  if(strcmp(argv[1],"1")==0){
    Master m(1,argv[3],argv[2]);
    m.run();
  }
  else{
    
    NodeInfo n(argv[5],argv[4]);
    CWorker w(argv[3],argv[2],n);
    printf("::::%s,%s",argv[5],argv[4]);
    w.run();
  }
  

}
