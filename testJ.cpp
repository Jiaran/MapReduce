
#include "master.h"
#include "node.h"
#include "worker.h"

int main(int argc, char** argv){
  if(strcmp(argv[1],"1")==0){
    Master m(2,argv[3],argv[2]);
    m.run();
  }
  else{
    CWorker w(argv[3],argv[2]);
    NodeInfo n(argv[5],argv[4]);
    printf("::::%s,%s",argv[5],argv[4]);
    w.join(n);
    w.run();
  }
  

}
