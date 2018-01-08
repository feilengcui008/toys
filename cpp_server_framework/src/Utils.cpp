#include "Utils.h"
#include <fcntl.h>

namespace Tan {

void my_print_info(const char *msg) { fprintf(stdout, "%s", msg); }

void my_error_exit(const char *msg) {
    fprintf(stdout, "%s", msg);
    exit(EXIT_FAILURE);
}

void my_success_exit(const char *msg) {
    fprintf(stdout, "%s", msg);
    exit(EXIT_SUCCESS);
}

int makeNonblock(int fd) {
    int ret = fcntl(fd, F_GETFL);
    if (ret < 0) {
        my_error_exit("get fd flag error\n");
    }
    ret = fcntl(fd, F_SETFL, ret | O_NONBLOCK);
    if (ret < 0) {
        my_error_exit("set fd nonblocking error\n");
    }
    return ret;
}
}
