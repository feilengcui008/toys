#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "util.h"

void errorExit(int err) {
    char lineinfo[ERR_BUF_LEN];
    sprintf(lineinfo, "in file %s line %d", __FILE__, __LINE__);
    char *temp = strerror(err);
    fprintf(stderr, "ERROR: %s %s\n", temp, lineinfo);
    exit(EXIT_FAILURE);
}

void errorMsgExit(const char *msg) {
    char lineinfo[ERR_BUF_LEN];
    sprintf(lineinfo, "in file %s line %d", __FILE__, __LINE__);
    fprintf(stderr, "ERROR: %s %s\n", msg, lineinfo);
    exit(EXIT_FAILURE);
}

void logInfo(const char *msg) { fprintf(stdout, "Info: %s\n", msg); }

int getFileSize(const char *filename) {
    struct stat buf;
    if (stat(filename, &buf) == -1) {
        errorMsgExit("get file size error");
    }
    return buf.st_size;
}

int getFileSizeByFd(int fd) {
    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        errorMsgExit("get file size error");
    }
    return buf.st_size;
}
