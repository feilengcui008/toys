#ifndef ConnectionEpollHandler_H
#define ConnectionEpollHandler_H

#include <memory>
#include <unistd.h>

#include "Connection.h"
#include "ConnectionHandler.h"

namespace Tan {

class EpollThread;
class AbstractServer;

class ConnectionEpollHandler : public ConnectionHandler {

 public:
  ConnectionEpollHandler();
  ~ConnectionEpollHandler();

  void handleConnection(Connection *conn) override;
  void handleRead(Connection *conn) override;
  void handleWrite(Connection *conn) override;
  void init() override;
  void setServer(AbstractServer *serv) override;
  // std::shared_ptr<EpollThread> &getEpoller();
  EpollThread *getEpoller();

 private:
  EpollThread *epoller_;
  // std::shared_ptr<EpollThread> epoller_;
};
}

#endif
