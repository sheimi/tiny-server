#include "env.h"
#include "dispatcher.h"

int main(int argc, char * argv[]) {

  init_dispatcher();
  
  free_dispatcher();
  return 0;
}
