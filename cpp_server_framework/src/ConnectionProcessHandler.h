#ifndef ConnectionProcessHandler_H
#define ConnectionProcessHandler_H

#include "ConnectionHandler.h"

namespace Tan {

class ConnectionProcessHandler : public ConnectionHandler {
 public:
  ConnectionProcessHandler() : ConnectionHandler() {};
  ~ConnectionProcessHandler() = default;
  void handleConnection(Connection *conn);
};
}

#endif
