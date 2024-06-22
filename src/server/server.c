#include "server.h"
#include "../context/context.h"
#include "../payload_builder/payload_builder.h"
#include "../payload_parser/payload_parser.h"
#include "../request/request.h"
#include "../response/response.h"
#include "../router/router.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void *dispatch_request(void *dispatched_request);

Server *new_server(int port) {
  Server *server;
  server = (Server *)malloc(sizeof(Server));

  server->router = new_router();

  server->port = port;
  if ((server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("server: failed to create socket");
    exit(EXIT_FAILURE);
  }

  int opt = 1;

  if (setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) < 0) {
    perror("server: failed to set socket opt");
    exit(EXIT_FAILURE);
  }

  socklen_t addrlen = sizeof(server->address);

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(port);

  return server;
}

int run_server(Server *server) {
  if (bind(server->sockfd, (struct sockaddr *)&server->address,
           sizeof(server->address)) < 0) {
    perror("server: failed to bind socket");
    exit(EXIT_FAILURE);
  }

  if (listen(server->sockfd, 100) < 0) {
    perror("server: failed to listen");
    exit(EXIT_FAILURE);
  }

  int incoming_socket;
  socklen_t addrlen = sizeof(server->address);
  ssize_t valread;
  char buffer[1024] = {0};

  while (1) {
    incoming_socket =
        accept(server->sockfd, (struct sockaddr *)&server->address, &addrlen);
    if (incoming_socket < 0) {
      perror("server: failed accepting connection, continuing...");
      continue;
    }

    valread = read(incoming_socket, buffer, 1024 - 1);

    char *buffer_copy = (char *)malloc(sizeof(char) * strlen(buffer));
    strcpy(buffer_copy, buffer);

    DispatchedRequest *dr =
        (DispatchedRequest *)malloc(sizeof(DispatchedRequest));
    dr->buffer = buffer_copy;
    dr->sockfd = incoming_socket;
    dr->self = server;

    pthread_t tid;
    pthread_create(&tid, NULL, dispatch_request, (void *)dr);

    memset(buffer, 0, sizeof(buffer));
  }

  return 0;
}

static void *dispatch_request(void *dispatched_request) {
  DispatchedRequest *dr = (DispatchedRequest *)dispatched_request;
  char *buffer = dr->buffer;
  int sockfd = dr->sockfd;
  Server *self = dr->self;

  ParsedPayload *pp = parse_payload(buffer);
  HashMap *params = get_path_params(self->router, pp->method, pp->path);

  Request *req = new_request(pp->body, pp->headers, "localhost", params);
  Response *res = new_response();
  Context *ctx = new_context(req, res);

  HandlerFunc handler = get_route(self->router, pp->method, pp->path);
  if (handler == NULL) {
    res->status_code = 404;
    res->body = "not found";
    goto finish;
    return NULL;
  }

  handler(voidify_context(ctx));

finish:
  if (res->status_code == 0) {
    res->status_code = 200;
  }
  char *response_payload =
      build_payload(res->status_code, pp->version, res->body, res->headers);

  send(sockfd, response_payload, strlen(response_payload), 0);

  free(req->headers);
  free(req->body);
  free(req);
  free(res->headers);
  free(res);
  free(ctx);
  free(pp);
  free(buffer);
  free(dispatched_request);

  close(sockfd);

  return NULL;
}
