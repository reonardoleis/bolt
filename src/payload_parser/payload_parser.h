#pragma once
#include "../enums/http/http.h"
#include "../headers/headers.h"

enum PayloadParsingError {
  INVALID_METHOD,
  INVALID_PATH,
  INVALID_VERSION,
  INVALID_HEADERS,
  INVALID_BODY,
};

typedef struct {
  enum HttpMethod method;
  char *path;
  char *version;
  Headers *headers;
  char *body;
  int content_length;
  enum PayloadParsingError error;
} ParsedPayload;

ParsedPayload *parse_payload(char *data);
