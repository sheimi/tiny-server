#ifndef CHILD_POOL_H
#define CHILD_POOL_H

#include <unistd.h>

#include "env.h"

void create_childpool(int num, int listenfd, int addrlen);

void free_childpool();


#endif
