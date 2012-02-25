#ifndef HANDLER_H 
#define HANDLER_H

#include "env.h"

typedef struct {
  char method[16];
  char url[128];
  char * query_string;
  char protocol[8];
  char host[32];
  char connection[16];
  char accept[16];
  char agent[128];
  char encoding[32];
  char lang[32];
} Request;

void handle_request(int fd);

#endif
