#include "child_pool.h"
#include "wrappers.h"
#include "handler.h"

#include <stdlib.h>
#include <math.h>
#include <signal.h>

pid_t * pids;
int pid_len;
int idle_num;

static pid_t child_make(int i, int listenfd, int addrlen);
static void child_main(int i, int listenfd, int addrlen);
static void sig_int(int signo);

void create_childpool(int argc, char * argv[]){
  int i;
  int num = atoi(argv[3]);
  pid_len = num;
  idle_num = num;

  socklen_t addrlen;
  int listenfd;

  listenfd = tcp_listen(argv[1], argv[2], &addrlen);

  pids = (pid_t *)(malloc(sizeof(pid_t) * num));

  for (i = 0; i < num; i++) {
    pids[i] = child_make(i, listenfd, addrlen);
  } 
  signal_wrapper(SIGINT, sig_int);
  while(true) {
    pause();
  }
}

void free_childpool() {
  free(pids);
}



/* 
 * Static methods
 * */
static pid_t child_make(int i, int listenfd, int addrlen) {
  pid_t pid;
  
  if ((pid = fork_wrapper()) > 0)
    return pid;
  child_main(i, listenfd, addrlen);
  return 0;
}

static void child_main(int i, int listenfd, int addrlen) {
  int connfd;
  socklen_t clilen;
  struct socketaddr * cliaddr;

  cliaddr = (struct socketaddr *)(malloc(addrlen)); 

  fprintf(stderr, "sub p %ld startling\n", (long)getpid());
  while(true) {
    clilen = addrlen;
    connfd = accept(listenfd, cliaddr, &clilen);
#ifdef DEBUG
    fprintf(stderr, "\n %ld is running \n", (long)getpid());
#endif
    handle_request(connfd);
#ifdef DEBUG
    fprintf(stderr, "\n %ld is finished\n", (long)getpid());
#endif
    close(connfd);
  }
}

static void sig_int(int signo) {
  int i;
  for (i = 0; i < idle_num; i++) {
    kill(pids[i], SIGTERM);
  }
  while (wait(NULL) > 0);
  exit(0); 
}
