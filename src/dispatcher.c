#include "env.h"
#include "dispatcher.h"
#include "stdlib.h"

void init_dispatcher() {

}

void dispatch(request_info * dif) {

}

void free_dispatcher() {

}

request_info * init_request_info() {
  request_info * ri = (request_info *) (malloc(sizeof(request_info *)));
  ri->url = 0;
  ri->method = 0;
  return ri;
}

void free_request_info(request_info * ri) {
  free(ri); 
}
