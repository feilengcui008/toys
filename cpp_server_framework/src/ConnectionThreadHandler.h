#ifndef ConnectionThreadHandler_H
#define ConnectionThreadHandler_H

#include <map>
#include <memory>

#include "ConnectionHandler.h"
#include "Thread.h"

namespace Tan {

class AbstractServer;
void *startHandlerThread(void *args);
class ConnectionThreadHandler : public ConnectionHandler, public Thread {
 public:
  ConnectionThreadHandler() : ConnectionHandler() {};
  ~ConnectionThreadHandler() = default;
  void handleConnection(Connection *conn) override;
  void handleRead(Connection *conn) override;
  void handleWrite(Connection *conn) override;
  void runBusiness() override;  // call handle connection in a new thread
  void handleInThread(Connection *conn);
  // private:
  //    std::map<int, std::shared_ptr<Connection>> conns_;
};

// used for pass data when create thread
struct Data {
  ConnectionThreadHandler *handler;
  Connection *conn;
  Data(ConnectionThreadHandler *h, Connection *c) : handler(h), conn(c) {};
};
}

#endif
