
#include "master.h"
#include "node.h"
#include "worker.h"

class A {
  int a ;
};
NodeInfo FILE_SYSTEM;
int main(int argc, char** argv){
  FILE_SYSTEM= NodeInfo(argv[3],argv[2]);
  if(strcmp(argv[1],"1")==0){
    Master m(1,argv[5],argv[4]);
    m.run();
  }
  else{
    
    NodeInfo n(argv[7],argv[6]);
    CWorker w(argv[5],argv[4],n);
    
    w.run();
  }
  

}
