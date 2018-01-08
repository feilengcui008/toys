#ifndef Request_H
#define Request_H

#include "Noncopyable.h"

/*
 *
 * class Request
 *
 * stands for the semantics of a io request of a client connection
 * we read the request io stream from the client connection with
 *handleConnection
 * of AbstractServer, and then read the stream with handleRead and pass the
 *stream
 * content into the Request class
 *
 * this class needs to be extended for a specific application server since
 * the logics of different request io streams are different
 *
 * */

namespace Tan {

class Connection;

class Request : Noncopyable {
 public:
  Request() : conn_(nullptr) {};
  ~Request() {}
  void setConnection(Connection *conn);
  Connection *getConnection();

 protected:
  Connection *conn_;  // which connection I am in
};
}

#endif
