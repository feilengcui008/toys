/* a little piece of code for the homework
 * of distributed systems class in xjtu 2016
 * A very simple async and non-blocking
 * file transfer server using ae eventloop
 * receive the data format size(4bytes)+data
 * from client and write data to disk file
 * by tanhongfei
 * feilengcui008@gmail.com
 *
 * this is the client part
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "anet.h"
#include "util.h"

#define CLIENT_SEND_BUF 4096
#define CLIENT_RECEIVE_BUF 4096

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stdout, "usage: client port filepath\n");
        return 0;
    }
    int port = atoi(argv[1]);
    char *pathname = argv[2];
    char err[ERR_BUF_LEN];

    // connect
    int clientfd = anetTcpConnect(err, NULL, port);
    if (clientfd == -1) {
        errorMsgExit(err);
    }
    // open file and get file size
    FILE *fp = fopen(pathname, "r");
    if (!fp) {
        errorExit(errno);
    }
    int size = getFileSizeByFd(fileno(fp));
    // fprintf(stdout, "file size is : %d\n", size);
    char buf[CLIENT_SEND_BUF];
    // first 4 bytes to identify the len,
    // then followed by data, so max file len 4G
    // send size first
    if (write(clientfd, &size, sizeof(int)) == -1) {
        close(clientfd);
        fclose(fp);
        errorMsgExit("write size info error");
    }
    int written = 0;
    int read = 0;
    while (written < size) {
        // finish sending or error
        if ((read = fread(buf, 1, sizeof(buf), fp)) < (int)sizeof(buf)) {
            break;
        }
        if (anetWrite(clientfd, buf, read) == -1) {
            close(clientfd);
            fclose(fp);
            errorMsgExit("send buf to server error");
        }
        written += read;
    }
    if (read == -1) {
        close(clientfd);
        fclose(fp);
        errorMsgExit("write size info error");
    }
    if (read != 0) {
        if (anetWrite(clientfd, buf, read) == -1) {
            close(clientfd);
            fclose(fp);
            errorMsgExit("send buf to server error");
        }
        written += read;
    }

    // receive ack from server
    char msg[CLIENT_RECEIVE_BUF];
    while (1) {
        if ((read = anetRead(clientfd, msg, sizeof(msg))) < (int)sizeof(msg)) {
            fprintf(stdout, "%s\n", msg);
            break;
        }
        fprintf(stdout, "%s\n", msg);
    }

    // some clean work
    close(clientfd);
    fclose(fp);
    return 0;
}
