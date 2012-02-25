#ifndef CHILD_POOL_H
#define CHILD_POOL_H

#include <unistd.h>

#include "env.h"

typedef struct child_process {
  pid_t child_pid;    //process id
  int child_pipefd;   //parent's stream pipe
  int child_status;   //0 = read
  long child_count;   //connections handled
  struct child_process * next;
} ChildProcess;

void create_childpool(int num, int listenfd, int addrlen);

void free_childpool();


#endif
