#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Headers *new_headers(int table_size, int list_size) {
  Headers *headers;

  headers = (Headers *)malloc(sizeof(Headers));
  headers->hashmap = new_hashmap(table_size, list_size);

  return headers;
}

int set_header(Headers *self, char *key, char *value) {
  int a = 1;

  return set_item(self->hashmap, key, value, strlen(value));
}

char *get_header(Headers *self, char *key) {
  HashMapItem *item;
  char *value;

  item = get_item(self->hashmap, key);

  if (item != NULL) {
    value = (char *)malloc(sizeof(char) * strlen(item->value));
    strcpy(value, item->value);
    return value;
  }

  return NULL;
}
