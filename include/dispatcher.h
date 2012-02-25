#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "env.h"

typedef struct _request_info{
  char * url;
  char * method;
}  request_info;

/* *
 * To init and free the request info
 */
request_info * init_request_info(); 
void free_request_info(request_info * ri);

/* * 
 * To init a a dispathcer for this server
 */
void init_dispatcher();

/* *
 * To dispatch a request
 */
void dispatch(request_info * dif);


/* *
 * To free a dispatcher()
 */
void free_dispatcher();

#endif
