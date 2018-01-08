#include "Connection.h"

namespace Tan {

class AbstractServer;

Connection::~Connection() {
    if (req_ != nullptr) delete req_;
    if (res_ != nullptr) delete res_;
}

void Connection::setState(STATE s) { state_ = s; }

STATE Connection::getState() const { return state_; }

void Connection::setFd(int fd) { clientfd_ = fd; }

int Connection::getFd() const { return clientfd_; }

char *Connection::getReadBuffer() { return read_buffer_; }

char *Connection::getWriteBuffer() { return write_buffer_; }

void Connection::setServer(AbstractServer *server) { server_ = server; }

AbstractServer *Connection::getServer() { return server_; }

int Connection::getReadSize() const { return read_size_; }

void Connection::setReadSize(int len) { read_size_ = len; }

int Connection::getWriteSize() const { return write_size_; }

void Connection::setWriteSize(int len) { write_size_ = len; }

Request *Connection::getRequest() { return req_; }

Response *Connection::getResponse() { return res_; }

void Connection::setRequest(Request *req) { req_ = req; }

void Connection::setResponse(Response *res) { res_ = res; }
}
