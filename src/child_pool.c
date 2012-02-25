#include "child_pool.h"
#include "wrappers.h"

#include <stdlib.h>
#include <math.h>

ChildProcess * array;
int array_len;
ChildProcess * idle_head;
ChildProcess * idle_tail;

fd_set * rset, masterset;
int maxfd, navail, nsel, connfd, rc;

static pid_t child_make(int i, int listenfd, int addrlen);
static void child_main(int i, int listenfd, int addrlen);

void create_childpool(int num, int listenfd, int addrlen) {
  int i;
  array_len= num;
  array = (ChildProcess *)(malloc(sizeof(ChildProcess) * num));
  idle_head = 0;
  idle_tail = 0;

  FD_ZERO(&masterset);
  FD_SET(listenfd, &masterset);
  maxfd = listenfd;

  for (i = 0; i < num; i++) {
    child_make(i, listenfd, addrlen);
    FD_SET(array[i].child_pipefd, &masterset);
    maxfd = max(maxfd, array[i].child_pipefd);
  } 

}

void free_childpool() {
  free(array);
}



/* 
 * Static methods
 * */
static pid_t child_make(int i, int listenfd, int addrlen) {
  int sockfd[2];
  pid_t pid;

  socketpair_wrapper(AF_LOCAL, SOCK_STREAM, 0, sockfd);

  if ((pid = fork_wrapper()) > 0) {
    close(sockfd[1]);
    array[i].child_pid = pid;
    array[i].child_pipefd = sockfd[0];
    array[i].child_status = 0;
    array[i].next = 0;
    if (idle_head == 0) {
      idle_head = &array[i];
      idle_tail = &array[i];
    } else {
      idle_tail->next = &array[i];
      idle_tail = &array[i];
    } 
    return pid;
  }
  dup2_wrapper(sockfd[1], STDERR_FILENO);
  close(sockfd[0]);
  close(sockfd[1]);
  close(listenfd);
  child_main(i, listenfd, addrlen); 
  return 0;
}

static void child_main(int i, int listenfd, int addrlen) {
  char c;
  int connfd;
  ssize_t n;
  fprintf(stdout, "sub p %ld startling\n", (long)getpid());
  while(true) {
    if ( (n = Read_fd(STDERR_FILENO, &c, 1, &connfd)) == 0)
      perror("error");
    if (connfd < 0)
      perror("error");
    close(connfd);
  }
}
