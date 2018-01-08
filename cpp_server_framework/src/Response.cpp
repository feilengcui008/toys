#include "Response.h"
#include "Connection.h"

namespace Tan {

void Response::setConnection(Connection *conn) { conn_ = conn; }
Connection *Response::getConnection() { return conn_; }
}
