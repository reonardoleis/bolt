#pragma once
#include "../headers/headers.h"

typedef struct {
  char *body;
  int status_code;
  Headers *headers;
} Response;

Response *new_response();
