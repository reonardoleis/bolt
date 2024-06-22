#include "../src/server/server.h"
#include "routes/routes.h"

int main() {
  Server *s = new_server(8192);
  setup_routes(s->router);
  run_server(s);
  return 0;
}
