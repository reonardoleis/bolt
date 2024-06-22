#include "payload_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_HEADERS 100

static int find_first(char *haystack, char needle);
static enum HttpMethod translate_method(char *method);

ParsedPayload *parse_payload(char *data) {
  ParsedPayload *parsed_payload;
  parsed_payload = (ParsedPayload *)malloc(sizeof(ParsedPayload));
  parsed_payload->headers = new_headers(MAX_HEADERS, 20);

  char *data_copy = (char *)malloc(sizeof(char) * (strlen(data) + 1));
  strcpy(data_copy, data);

  int metadata_size = sizeof(char) * (find_first(data, '\r') + 1);
  char *metadata = (char *)malloc(metadata_size);

  strncpy(metadata, data, metadata_size);
  metadata[metadata_size - 1] = '\0';

  char *method = strtok(metadata, " ");
  char *path = strtok(NULL, " ");
  char *version = strtok(NULL, " ");

  parsed_payload->method = translate_method(method);
  parsed_payload->path = (char *)malloc(sizeof(char) * strlen(path));
  strcpy(parsed_payload->path, path);

  parsed_payload->version = (char *)malloc(sizeof(char) * strlen(version));
  strcpy(parsed_payload->version, version);

  char *body_start = strstr(data_copy, "\r\n\r\n");
  body_start[0] = '\0';

  char *header_line = strtok(data_copy, "\r\n");
  while (header_line != NULL) {
    char *key, *value;

    int key_end = find_first(header_line, ':') - 1;
    size_t key_size = sizeof(char) * (key_end + 2);
    key = (char *)malloc(key_size);
    strncpy(key, header_line, key_size);
    key[strlen(key) - 1] = '\0';

    header_line = header_line + key_end + 3;

    size_t value_size = sizeof(char) * strlen(header_line);
    value = (char *)malloc(value_size);
    strcpy(value, header_line);

    set_header(parsed_payload->headers, key, value);

    free(key);
    free(value);

    header_line = strtok(NULL, "\r\n");
  }

  body_start += 4;

  parsed_payload->body = (char *)malloc(sizeof(char) * strlen(body_start));
  strcpy(parsed_payload->body, body_start);

  free(metadata);
  free(data_copy);
  return parsed_payload;
}

static int find_first(char *haystack, char needle) {
  int len = strlen(haystack);
  if (len == 0) {
    return -1;
  }

  int pos = 0;
  while (pos < len) {
    if (haystack[pos] == needle) {
      return pos;
    }

    pos++;
  }

  return -1;
}

static enum HttpMethod translate_method(char *method) {
  if (strcmp(method, "GET") == 0) {
    return GET;
  } else if (strcmp(method, "POST") == 0) {
    return POST;
  } else if (strcmp(method, "PUT") == 0) {
    return PUT;
  } else if (strcmp(method, "DELETE") == 0) {
    return DELETE;
  } else if (strcmp(method, "PATCH") == 0) {
    return PATCH;
  } else if (strcmp(method, "HEAD") == 0) {
    return HEAD;
  } else if (strcmp(method, "OPTIONS") == 0) {
    return OPTIONS;
  } else {
    return UNKNOWN; // Caso o método não seja reconhecido
  }
}
