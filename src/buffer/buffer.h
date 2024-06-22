#pragma once
#include <stdlib.h>

char *extend_buffer(char *current, size_t curent_size, char *to_append,
                    size_t to_append_size);
