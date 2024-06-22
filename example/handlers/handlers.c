#include "handlers.h"
#include "../../src/buffer/buffer.h"
#include "../../src/context/context.h"
#include <stdio.h>
#include <string.h>

int counter = 0;

void ping(void *c) {
  Context *ctx = get_context(c);

  char *firstname = get_param(ctx->req, "firstname");
  char *lastname = get_param(ctx->req, "lastname");

  char *response = "Hello, ";

  response =
      extend_buffer(response, strlen(response), firstname, strlen(firstname));

  response = extend_buffer(response, strlen(response), " ", strlen(" "));

  response =
      extend_buffer(response, strlen(response), lastname, strlen(lastname));

  ctx->res->status_code = 200;
  ctx->res->body = response;
}

void count(void *c) {
  Context *ctx = get_context(c);

  char *response = (char *)malloc(sizeof(char) * 50);
  sprintf(response, "{\"count\":%d}", counter++);

  set_header(ctx->res->headers, "Content-Type", "application/json");

  ctx->res->status_code = 200;
  ctx->res->body = response;
}
