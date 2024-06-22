#pragma once
#include <stdlib.h>

typedef struct {
  char *key;
  void *value;
  size_t value_size;
} HashMapItem;

typedef struct {
  int table_size;
  int list_size;
  char **keys;
  HashMapItem ***items;
} HashMap;

HashMap *new_hashmap(int table_size, int list_size);
int set_item(HashMap *self, char *key, void *value, size_t value_size);
HashMapItem *get_item(HashMap *self, char *key);
char **get_keys(HashMap *self);
