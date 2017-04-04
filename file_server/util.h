#ifndef _UTIL_H
#define _UTIL_H

#define ERR_BUF_LEN 256
#define IP_MAX_LEN 46

void errorExit(int err);
void errorMsgExit(const char *msg);

void logInfo(const char *msg);

int getFileSize(const char *filename);

int getFileSizeByFd(int fd);

#endif
