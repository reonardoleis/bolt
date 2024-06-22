#include "payload_builder.h"
#include "../buffer/buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *build_payload(int status_code, char *version, char *body,
                    Headers *headers) {

  char *status_code_str = (char *)malloc(sizeof(char) * 4);
  snprintf(status_code_str, sizeof(char) * 4, "%d", status_code);

  size_t payload_len = strlen(status_code_str) + 1; // whitespace;
  payload_len += strlen(version) + 2;               // \r\n
  payload_len += strlen(body) + 3; // extra \r\n before body + null terminator

  char **keys = get_keys(headers->hashmap);
  int i;

  char *headers_str = (char *)malloc(0);

  for (i = 0; keys[i] != NULL; i++) {
    char *key = keys[i];
    char *value = get_header(headers, key);
    char *terminator = "\r\n";

    headers_str =
        extend_buffer(headers_str, strlen(headers_str), key, strlen(key));
    headers_str =
        extend_buffer(headers_str, strlen(headers_str), ": ", strlen(": "));
    headers_str =
        extend_buffer(headers_str, strlen(headers_str), value, strlen(value));
    headers_str = extend_buffer(headers_str, strlen(headers_str), terminator,
                                strlen(terminator));
  }

  payload_len += strlen(headers_str);

  char *payload = (char *)malloc(sizeof(char) * payload_len);

  snprintf(payload, payload_len,
           "%s %s\r\n"
           "%s\r\n"
           "%s",
           version, status_code_str, headers_str, body);

  free(headers_str);
  free(status_code_str);

  return payload;
}
