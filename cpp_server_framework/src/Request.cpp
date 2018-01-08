#include "Request.h"
#include "Connection.h"

namespace Tan {

void Request::setConnection(Connection *conn) { conn_ = conn; }
Connection *Request::getConnection() { return conn_; }
}
