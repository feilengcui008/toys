#include "AbstractServer.h"

using namespace Tan;

class ExampleServer : public AbstractServer {

 public:
  ExampleServer(std::string host, unsigned short port, unsigned int backlog)
      : AbstractServer(host, port, backlog) {}
  void parseRequest(Request *req) override;
  void transformResponse(Response *res) override;
};

void ExampleServer::parseRequest(Request *req) {
  Connection *conn = req->getConnection();
  char *buf = conn->getWriteBuffer();
  // FILE *fp = fopen("index.html", "r");
  sprintf(buf,
          "HTTP/1.1 200 OK\r\nServer: ExampleServer\r\nDate:"
          "Wed, 02 Dec 2015 17:11:47 GMT\r\nContent-Length: %d\r\nContent-Type:"
          "text/html\r\nConnection: Close\r\n\r\n",
          conn->getReadSize());
  int written = strlen(buf);
  int to_write = conn->getReadSize();
  strncpy(buf + written, conn->getReadBuffer(), to_write);
  conn->setWriteSize(written + to_write);
}

void ExampleServer::transformResponse(Response *res) {}
