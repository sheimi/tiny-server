#ifndef CHILD_POOL_H
#define CHILD_POOL_H

#include <unistd.h>

#include "env.h"

void create_childpool(int argc, char * argv[]);

void free_childpool();


#endif
