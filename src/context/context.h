#pragma once
#include "../request/request.h"
#include "../response/response.h"

typedef struct {
  Request *req;
  Response *res;
} Context;

Context *new_context(Request *req, Response *res);
void *voidify_context(Context *context);
Context *get_context(void *context);
