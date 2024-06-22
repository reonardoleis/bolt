#include "context.h"

Context *new_context(Request *req, Response *res) {
  Context *ctx = (Context *)malloc(sizeof(Context));
  ctx->req = req;
  ctx->res = res;

  return ctx;
}

void *voidify_context(Context *context) {
  void *ptr = (void *)context;
  return ptr;
}

Context *get_context(void *context) { return (Context *)context; }
