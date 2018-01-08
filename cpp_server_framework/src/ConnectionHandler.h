#ifndef ConnectionHandler_H
#define ConnectionHandler_H

#include "Connection.h"

/*
 *
 * base class ConnectionHandler to be extends by
 * the process and thread mode
 *
 * */

namespace Tan {

class Connection;
class ConnectionHandler {
 public:
  ConnectionHandler() = default;
  virtual ~ConnectionHandler() = default;
  virtual void handleConnection(Connection *conn) {}
  virtual void handleRead(Connection *conn) {}
  virtual void handleWrite(Connection *conn) {}
  virtual void setServer(AbstractServer *serv) { server = serv; }
  virtual void init() {}

 public:
  AbstractServer *server;
};
}

#endif
