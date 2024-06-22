#include "request.h"
#include <stdlib.h>

Request *new_request(char *body, Headers *headers, char *remote_addr,
                     HashMap *params) {
  Request *req;

  req = (Request *)malloc(sizeof(Request));
  req->headers = headers;
  req->body = body;
  req->remote_addr = remote_addr;
  req->params = params;

  return req;
}

char *get_param(Request *self, char *key) {
  HashMapItem *item = get_item(self->params, key);
  return (char *)item->value;
}
