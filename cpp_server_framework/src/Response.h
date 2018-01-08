#ifndef Response_H
#define Response_H

#include "Noncopyable.h"

/*
 * class Response
 *
 * stands for the response contens for a specific client request,
 * after parse the io stream, we get the Request, we generate the
 * response according to the request attributes
 *
 * */

namespace Tan {

class Connection;

class Response : Noncopyable {
 public:
  Response() : conn_(nullptr) {}
  ~Response() {}
  void setConnection(Connection *conn);
  Connection *getConnection();

 private:
  Connection *conn_;
};
}

#endif
