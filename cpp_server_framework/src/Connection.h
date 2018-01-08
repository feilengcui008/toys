#ifndef Connection_H
#define Connection_H

#include "Noncopyable.h"
#include "Request.h"
#include "Response.h"

/*
 * class Connection
 * stands for a client connection(socket)
 * store the read and write buffer
 * store the state transfer of client socket etc
 *
 * */

namespace Tan {

#define DEFAULT_BUFFER_SIZE 4 * 1024 * 1024
#define CHUNCK_SIZE 4096

enum STATE {
  CONNECTED = 1,
  READING,
  WRITING,
  CLOSING,
  DEFAULT
};

class AbstractServer;

class Connection : Noncopyable {
 public:
  Connection() {
    clientfd_ = -1;
    state_ = DEFAULT;
    server_ = nullptr;
    read_size_ = write_size_ = 0;
    req_ = nullptr;
    res_ = nullptr;
  };
  ~Connection();

 public:
  void setState(STATE s);
  STATE getState() const;
  void setFd(int fd);
  int getFd() const;
  char *getReadBuffer();
  char *getWriteBuffer();
  void setServer(AbstractServer *server);
  AbstractServer *getServer();
  int getReadSize() const;
  void setReadSize(int len);
  int getWriteSize() const;
  void setWriteSize(int len);
  Request *getRequest();
  void setRequest(Request *req);
  Response *getResponse();
  void setResponse(Response *res);

 private:
  int clientfd_;
  char read_buffer_[DEFAULT_BUFFER_SIZE];
  int read_size_;
  int write_size_;
  char write_buffer_[DEFAULT_BUFFER_SIZE];
  STATE state_;
  // which server i com from, also we need to
  // use the server_->handleRequest
  AbstractServer *server_;
  Request *req_;
  Response *res_;
};
}

#endif
