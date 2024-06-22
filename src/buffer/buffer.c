#include "buffer.h"
#include <string.h>

char *extend_buffer(char *current, size_t current_len, char *to_append,
                    size_t to_append_len) {
  size_t total_len = current_len + to_append_len + 1;

  char *extended = (char *)malloc(sizeof(char) * total_len);

  memcpy(extended, current, current_len * sizeof(char));
  memcpy(extended + current_len, to_append, to_append_len * sizeof(char));

  return extended;
}
