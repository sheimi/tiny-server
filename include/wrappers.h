#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <sys/socket.h>
#include <stdio.h>

#define LISTENQ 1024

void socketpair_wrapper(int family, int type, int protocol, int * fd);
pid_t fork_wrapper();
void dup2_wrapper(int fd1, int fd2);

typedef void (*sigfunc)(int signo);
sigfunc signal_wrapper(int signo, sigfunc func); 

int tcp_listen(const char * host, const char * serv, socklen_t * addrlenp);

#endif
