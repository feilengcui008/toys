/* a little piece of code for the homework 
 * of distributed systems class in xjtu 2016
 * A very simple async and non-blocking 
 * file transfer server using ae eventloop
 * receive the data format size(4bytes)+data 
 * from client and write data to disk file 
 * by tanhongfei  
 * feilengcui008@gmail.com 
 *
 * this is the server part
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ae.h"
#include "anet.h"
#include "util.h"

#define SERVER_SEND_BUF 4096
#define SERVER_RECEIVE_BUF 4096
#define SET_SIZE 1024


// some forward declarations
void clientRead(struct aeEventLoop *ae, int fd);
void clientWrite(struct aeEventLoop *ae, int fd);
void clientHandler(struct aeEventLoop *ae, int fd, void *clientData, int mask);
void connHandler(struct aeEventLoop *ae, int fd, void *clientData, int mask);
void removeEventAndClose(struct aeEventLoop *ae, int fd);

// connection handler 
void connHandler(struct aeEventLoop *ae, int fd, void *clientData, int mask)
{
  fprintf(stdout, "handling client write...\n"); 
  fprintf(stdout, "conn coming...\n");
  // add to ae 
  char err[ERR_BUF_LEN];
  char cip[IP_MAX_LEN];
  int clientPort;
  int clientfd = anetTcpAccept(err, fd, cip, sizeof(cip), &clientPort);
  if (clientfd == -1) {
    if (errno != EWOULDBLOCK) {
      logInfo("accept client error");
    }
    return;
  } 
  if (aeCreateFileEvent(ae, clientfd, AE_READABLE, clientHandler, NULL) == -1) {
    logInfo("add client fd to ae error");
  }
}

// clientfd handler
void clientHandler(struct aeEventLoop *ae, int fd, void *clientData, int mask) 
{
  fprintf(stdout, "handling client...\n"); 
  if (mask == AE_READABLE) {
    clientRead(ae, fd);
  } 
  if (mask == AE_WRITABLE) {
    clientWrite(ae, fd);
  }
}

// read client logic 
void clientRead(struct aeEventLoop *ae, int fd)
{
  fprintf(stdout, "handling client read...\n"); 
  // first read 4 bytes 
  char temp[4];
  char buf[SERVER_RECEIVE_BUF];
  int inRead = 0;
  int totalRead = 0;
  int size = 0;
  if ((inRead = read(fd, temp, sizeof(temp))) < 4) {
    if (inRead == -1 && errno != EWOULDBLOCK && errno != EAGAIN) {
      logInfo("read client header error"); 
    } else 
      logInfo("client format error");
    removeEventAndClose(ae, fd);
    return;
  } else {
    const char *filename = "received.txt";
    FILE *fp = fopen(filename, "w+");
    if (!fp) {
      logInfo("open file error");
      removeEventAndClose(ae, fd);
      return;
    }    
    size = *((int *)&temp[0]);
    //fprintf(stdout, "first four bytes: %d\n", size);
    while (totalRead < size) {
      if ((inRead = read(fd, buf, sizeof(buf))) < (int)sizeof(buf)) {
        if (inRead == -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
          continue;
        }
        break;
      }
      totalRead += inRead;
      //fprintf(stdout, "%s\n", buf);
      fwrite(buf, 1, inRead, fp);
    }
    if (inRead == -1) {
      logInfo("read client data error");
      removeEventAndClose(ae, fd);
      return;
    } 
    if (inRead == 0) {
      logInfo("client closed");
      removeEventAndClose(ae, fd);
      return;
    }
    if (inRead > 0) {
      totalRead += inRead;
      //fprintf(stdout, "%s\n", buf);
      fwrite(buf, 1, inRead, fp);
      fprintf(stdout, "read file from client complete\n");
      fclose(fp);
    }
  }
  // modify event to writable so we can write the result msg
  int msk = aeGetFileEvents(ae, fd);
  aeCreateFileEvent(ae, fd, msk | AE_WRITABLE, clientHandler, NULL);
}

// write client logic
void clientWrite(struct aeEventLoop *ae, int fd)
{
  const char *msg = "successful uploaded file...";
  write(fd, msg, strlen(msg));
  int msk = aeGetFileEvents(ae, fd);
  aeDeleteFileEvent(ae, fd, msk);
  close(fd);
}

void removeEventAndClose(struct aeEventLoop *ae, int fd)
{
  int msk = aeGetFileEvents(ae, fd);
  aeDeleteFileEvent(ae, fd, msk);
  close(fd);
}


int main(int argc, char *argv[])
{
  char err[ERR_BUF_LEN];
  int port = 8080;
  int backlog = 100;
  aeEventLoop *ae;
  int serverfd;
  // create event loop
  ae = aeCreateEventLoop(SET_SIZE); 
  if (!ae) {
    errorExit(errno);
  }
  // create, bind, listen socket; make it nonblocking
  serverfd = anetTcpServer(err, port, NULL, backlog);
  if (serverfd == -1) {
    errorMsgExit(err);
  }
  if (anetNonBlock(err, serverfd) == -1) {
    errorMsgExit(err); 
  }
  // bind event 
  if (aeCreateFileEvent(ae, serverfd, AE_READABLE, connHandler, NULL) == -1) {
    errorExit(errno);
  }
  aeMain(ae);
  return 0;
}
