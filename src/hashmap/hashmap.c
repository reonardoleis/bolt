#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

HashMap *new_hashmap(int table_size, int list_size) {
  HashMap *hashmap;

  hashmap = (HashMap *)malloc(sizeof(HashMapItem));
  hashmap->items = (HashMapItem ***)malloc(sizeof(HashMapItem **) * table_size);
  hashmap->keys = (char **)malloc(sizeof(char *) * table_size * list_size);

  int i, j;
  for (i = 0; i < table_size; i++) {
    hashmap->items[i] =
        (HashMapItem **)malloc(sizeof(HashMapItem *) * list_size);

    for (j = 0; j < list_size; j++)
      hashmap->items[i][j] = NULL;
  }

  hashmap->table_size = table_size;
  hashmap->list_size = list_size;

  return hashmap;
}

static unsigned long hash(char *str) {
  unsigned long hash_value = 5381;
  int c;

  while ((c = *str++)) {
    hash_value = ((hash_value << 5) + hash_value) + c;
  }

  return hash_value;
}

int set_item(HashMap *self, char *key, void *value, size_t value_size) {
  int hashed_value = hash(key) % self->list_size;

  HashMapItem **item = self->items[hashed_value];
  int index = 0;

  while (index++ < self->list_size) {
    if (item[index] == NULL) {
      item[index] = (HashMapItem *)malloc(sizeof(HashMapItem));
      item[index]->key = (char *)malloc(sizeof(char) * strlen(key));
      item[index]->value = (void *)malloc(value_size);
      item[index]->value_size = value_size;

      strcpy(item[index]->key, key);
      memcpy(item[index]->value, value, value_size);

      index = 0;
      while (index < self->table_size * self->list_size &&
             self->keys[index] != NULL) {
        index++;
      }

      self->keys[index] = (char *)malloc(sizeof(char) * strlen(key));
      strcpy(self->keys[index], key);

      return 0;
    }
  }

  return -1;
}

HashMapItem *get_item(HashMap *self, char *key) {
  int hashed_value = hash(key) % self->list_size;
  int index = 0;

  HashMapItem **item = self->items[hashed_value];

  while (index++ < self->list_size) {
    if (item[index] != NULL && strcmp(item[index]->key, key) == 0) {
      return item[index];
    }
  }

  return NULL;
}

char **get_keys(HashMap *self) { return self->keys; }
