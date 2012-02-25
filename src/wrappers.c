#include "env.h"
#include "wrappers.h"

#include <unistd.h>

void socketpair_wrapper(int family, int type, int protocol, int * fd) {
  int n;
  if ( (n = socketpair(family, type, protocol, fd)) < 0)
    perror("socketpair error");
}

pid_t fork_wrapper() {
  pid_t pid;
  if ((pid = fork()) < 0)
    perror("fork error");
  return pid;
}

void dup2_wrapper(int fd1, int fd2) {
  if (dup2(fd1, fd2) == -1)
    perror("dup error");
}
