#include "env.h"
#include "wrappers.h"

#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>

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

sigfunc signal_wrapper(int signo, sigfunc func) {
  struct sigaction act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (sigaction(signo, &act, &oact) < 0)
    perror("signal error");
  return oact.sa_handler;
}

int tcp_listen(const char * host, const char * serv, socklen_t * addrlenp) {
  int       listenfd, n;
  const int   on = 1;
  struct addrinfo hints, *res, *ressave;

  bzero(&hints, sizeof(struct addrinfo));
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
    perror("error");
  ressave = res;

  do {
    listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenfd < 0)
      continue;   /*  error, try next one */

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
      break;      /*  success */

    close(listenfd);  /*  bind error, close and try next one */
  } while ( (res = res->ai_next) != NULL);

  if (res == NULL)  /*  errno from final socket() or bind() */
    perror("error");

  listen(listenfd, LISTENQ);

  if (addrlenp)
    *addrlenp = res->ai_addrlen;  /*  return size of protocol address*/ 
  freeaddrinfo(ressave);
  return(listenfd);
}
