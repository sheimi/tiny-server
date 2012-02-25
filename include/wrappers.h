#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <sys/socket.h>
#include <stdio.h>

void socketpair_wrapper(int family, int type, int protocol, int * fd);
pid_t fork_wrapper();
void dup2_wrapper(int fd1, int fd2);

#endif
