#ifndef Utils_H
#define Utils_H

#include <cstdio>
#include <cstdlib>

namespace Tan {

void my_print_info(const char *msg);

void my_error_exit(const char *msg);

void my_success_exit(const char *msg);

int makeNonblock(int fd);
}

#endif
