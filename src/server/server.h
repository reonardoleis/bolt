#pragma once
#include "../hashmap/hashmap.h"
#include "../router/router.h"
#include <netinet/in.h>
#define MAX_HANDLERS 1000

typedef struct {
  int port;
  int sockfd;
  struct sockaddr_in address;
  Router *router;
} Server;

typedef struct {
  int sockfd;
  Server *self;
  char *buffer;
} DispatchedRequest;

Server *new_server(int port);
int run_server(Server *server);
