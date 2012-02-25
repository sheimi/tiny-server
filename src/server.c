#include "env.h"
#include "dispatcher.h"

#include "wrappers.h"
#include <sys/socket.h>

int main(int argc, char * argv[]) {

  socklen_t addrlen;
  int listenfd;

  init_dispatcher();

  listenfd = tcp_listen("localhost", "4000", &addrlen);

  create_childpool(5, listenfd, addrlen);
  free_dispatcher();
  return 0;
}
