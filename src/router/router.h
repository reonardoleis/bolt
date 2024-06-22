#pragma once
#include "../enums/http/http.h"
#include "../hashmap/hashmap.h"
#define MAX_ROUTES 1000
typedef void (*HandlerFunc)(void *);

typedef struct {
  enum HttpMethod method;
  char *path;
  char **path_parts;
  int count;
  int *is_param;
  HandlerFunc handler;
} Route;

typedef struct {
  Route **routes;
} Router;

Router *new_router();
int set_route(Router *router, enum HttpMethod method, char *path,
              HandlerFunc handler);
HandlerFunc get_route(Router *router, enum HttpMethod method, char *path);
HashMap *get_path_params(Router *self, enum HttpMethod method, char *path);
