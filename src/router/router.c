#include "router.h"
#include "../hashmap/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Router *new_router() {
  Router *router;
  router = (Router *)malloc(sizeof(Router));

  router->routes = (Route **)malloc(sizeof(Route *) * MAX_ROUTES);

  int i;
  for (i = 0; i < MAX_ROUTES; i++) {
    router->routes[i] = NULL;
  }

  return router;
}

int set_route(Router *self, enum HttpMethod method, char *path,
              HandlerFunc handler) {
  int slash_count = 0;

  int i = 0;
  while (path[i] != '\0') {
    if (path[i] == '/') {
      slash_count++;
    }

    i++;
  }

  i = 0;
  while (self->routes[i] != NULL) {
    i++;
  }

  self->routes[i] = (Route *)malloc(sizeof(Route));
  self->routes[i]->path = (char *)malloc(sizeof(char) * strlen(path));
  strcpy(self->routes[i]->path, path);

  char *path_copy = (char *)malloc(sizeof(char) * strlen(path));
  strcpy(path_copy, path);

  self->routes[i]->path_parts = (char **)malloc(sizeof(char *) * slash_count);
  self->routes[i]->is_param = (int *)malloc(sizeof(int) * slash_count);

  char *path_part = strtok(path_copy, "/");

  int j = 0;
  while (path_part != NULL) {
    self->routes[i]->path_parts[j] =
        (char *)malloc(sizeof(char) * strlen(path_part));
    strcpy(self->routes[i]->path_parts[j], path_part);

    self->routes[i]->is_param[j] = path_part[0] == ':';

    path_part = strtok(NULL, "/");
    j++;
  }

  self->routes[i]->count = slash_count;
  self->routes[i]->handler = handler;
  self->routes[i]->method = method;
  free(path_copy);
  return 0;
}

HandlerFunc get_route(Router *self, enum HttpMethod method, char *path) {
  int slash_count = 0;

  int i = 0;
  while (path[i] != '\0') {
    if (path[i] == '/') {
      slash_count++;
    }

    i++;
  }

  char **path_parts = (char **)malloc(sizeof(char *) * slash_count);

  char *path_copy = (char *)malloc(sizeof(char) * strlen(path));
  strcpy(path_copy, path);

  char *path_part = strtok(path_copy, "/");

  i = 0;
  while (path_part != NULL) {
    path_parts[i] = (char *)malloc(sizeof(char) * strlen(path_part));
    strcpy(path_parts[i], path_part);

    path_part = strtok(NULL, "/");
    i++;
  }

  i = 0;
  while (self->routes[i] != NULL) {
    Route *route = self->routes[i];

    if (route->count == slash_count && route->method == method) {
      int equal_count = 0;
      int j;
      for (j = 0; j < slash_count; j++) {

        if (strcmp(path_parts[j], route->path_parts[j]) == 0 ||
            route->is_param[j]) {
          equal_count++;
        } else {
          break;
        }
      }

      if (equal_count == slash_count) {
        return route->handler;
      }
    }

    i++;
  }

  return NULL;
}

HashMap *get_path_params(Router *self, enum HttpMethod method, char *path) {
  int slash_count = 0;

  int i = 0;
  while (path[i] != '\0') {
    if (path[i] == '/') {
      slash_count++;
    }

    i++;
  }

  char **path_parts = (char **)malloc(sizeof(char *) * slash_count);

  char *path_copy = (char *)malloc(sizeof(char) * strlen(path));
  strcpy(path_copy, path);

  char *path_part = strtok(path_copy, "/");

  i = 0;
  while (path_part != NULL) {
    path_parts[i] = (char *)malloc(sizeof(char) * strlen(path_part));
    strcpy(path_parts[i], path_part);

    path_part = strtok(NULL, "/");
    i++;
  }

  i = 0;
  while (self->routes[i] != NULL) {
    Route *route = self->routes[i];

    if (route->count == slash_count && route->method == method) {
      int equal_count = 0;
      int j;
      for (j = 0; j < slash_count; j++) {

        if (strcmp(path_parts[j], route->path_parts[j]) == 0 ||
            route->is_param[j]) {
          equal_count++;
        } else {
          break;
        }
      }

      if (equal_count == slash_count) {
        HashMap *hashmap = new_hashmap(200, 10);

        i = 0;
        for (; i < slash_count; i++) {
          if (route->is_param[i]) {
            char *param_name = route->path_parts[i] + 1;
            set_item(hashmap, param_name, path_parts[i],
                     sizeof(char) * strlen(path_parts[i]));
          }
        }

        return hashmap;
      }
    }

    i++;
  }

  return NULL;
}
