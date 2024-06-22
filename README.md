### ⚡ Bolt

Bolt is a minimalist HTTP server/microframework written in pure C, developed as a toy project to explore server-side programming concepts. It was designed to be used by simply including the header files on your project and adding the .c files to your compilation script.

## Features

- **Minimalistic Design**: Designed to be lightweight and simple.
- **HTTP Server**: Handles incoming HTTP requests.
- **Microframework**: Provides basic routing and response handling functionalities.
- **Path Parameters**: Supports extracting path parameters from routes.
- **Multi-threaded Processing**: Uses pthreads for concurrent request handling.
- **Header Manipulation**: Capable of setting response headers and accessing request headers.
- **HTTP Status Codes**: Supports setting HTTP response codes.
- **Response Body**: Allows setting response body content.

## Getting Started

To get started with Bolt, follow these steps:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-username/bolt.git
   cd bolt
   ```
   
2. **Include the header files on your project and add bolt's .c files to your compilation script**
   More info about it can be found on the [example](https://github.com/reonardoleis/bolt/tree/main/example) service's Makefile.

## Usage

### Setting Routes

You can define routes using the `set_route` function, specifying methods, paths and handlers:

```c
set_route(router, POST, "/hello", hello_handler);
set_route(rouuter, GET, "/user/:name", user_handler);
```

### Handling Requests

Create handler functions that conform to the `HandlerFunc` function signature:

```c
void count(void *c) {
  Context *ctx = get_context(c);

  char *response = (char *)malloc(sizeof(char) * 50);
  sprintf(response, "{\"count\":%d}", counter++);

  set_header(ctx->res->headers, "Content-Type", "application/json");

  ctx->res->status_code = 200;
  ctx->res->body = response;
}
```
Register it as a handler setting the path and the method:

```c
void setup_routes(Router *router) {
  set_route(router, POST, "/count", count);
}
``` 

### Concurrent Requests

Bolt uses pthreads to handle multiple requests concurrently. Ensure your system supports pthreads for optimal performance.

## Contributing

Contributions are welcome! Fork the repository and submit a pull request with your changes.

## Acknowledgments

- Inspired by minimalistic HTTP servers and microframeworks.
- Built with love and curiosity.
