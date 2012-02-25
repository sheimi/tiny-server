#include "env.h"
#include "dispatcher.h"
#include "child_pool.h"

#include "wrappers.h"
#include <sys/socket.h>



int main(int argc, char * argv[]) {
  
  init_dispatcher();
  
  create_childpool(argc, argv);
  free_childpool();

  free_dispatcher();
  return 0;
}
