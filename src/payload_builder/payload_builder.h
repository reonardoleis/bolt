#pragma once
#include "../headers/headers.h"

char *build_payload(int status_code, char *version, char *body,
                    Headers *headers);
