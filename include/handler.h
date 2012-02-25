#ifndef HANDLER_H 
#define HANDLER_H

#include "env.h"

typedef struct {
  char method[10];
  char url[200];
  char * query_string;
} Request;

void handle_request(int fd);

#endif
