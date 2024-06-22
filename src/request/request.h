#pragma once
#include "../headers/headers.h"

typedef struct {
  char *body;
  Headers *headers;
  HashMap *params;
  char *remote_addr;
} Request;

Request *new_request(char *body, Headers *headers, char *remote_addr,
                     HashMap *params);
char *get_param(Request *self, char *key);
