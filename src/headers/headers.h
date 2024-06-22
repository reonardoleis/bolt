#pragma once
#include "../hashmap/hashmap.h"

typedef struct {
  HashMap *hashmap;
} Headers;

Headers *new_headers(int table_size, int list_size);
int set_header(Headers *self, char *key, char *value);
char *get_header(Headers *self, char *key);
