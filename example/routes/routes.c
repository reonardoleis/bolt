#include "routes.h"
#include "../../src/enums/http/http.h"
#include "../handlers/handlers.h"

void setup_routes(Router *router) {
  set_route(router, GET, "/ping/:firstname/:lastname", ping);
  set_route(router, POST, "/count", count);
}
