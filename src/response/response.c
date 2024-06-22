#include "response.h"
#include <stdlib.h>

Response *new_response() {
  Response *res;

  res = (Response *)malloc(sizeof(Response));
  res->headers = (Headers *)malloc(sizeof(Headers));
  res->headers = new_headers(100, 20);
  res->status_code = 200;

  return res;
}
