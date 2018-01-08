#include <iostream>

#include <cassert>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>

#include "ConnectionEpollHandler.h"
#include "EpollThread.h"
#include "AbstractServer.h"

namespace Tan {

ConnectionEpollHandler::ConnectionEpollHandler() {
    // epoller_ = std::make_shared<EpollThread>(EpollThread());
    epoller_ = new EpollThread();
}

ConnectionEpollHandler::~ConnectionEpollHandler() {
    assert(epoller_ != nullptr);
    delete epoller_;
}

void ConnectionEpollHandler::init() {
    epoller_->init();
    epoller_->setHandler(this);
}

void ConnectionEpollHandler::setServer(AbstractServer *serv) { server = serv; }

// std::shared_ptr<EpollThread> &ConnectionEpollHandler::getEpoller()
EpollThread *ConnectionEpollHandler::getEpoller() { return epoller_; }

void ConnectionEpollHandler::handleConnection(Connection *conn) {
    /*
       if(conn == nullptr) {
       fprintf(stdout, "in handleConnection conn is nullptr\n");
       return;
       }
       */
    epoller_->addConnection(conn);
}

void ConnectionEpollHandler::handleRead(Connection *conn) {
    // we read the data from client socket stream into conn->readBuf
    // and then we construct a request
    // pass the request to server->parseRequest to parse the data
    // and maybe gen a Response
    // last we call handle write to finish the response

    /*
       if(conn == nullptr) {
       fprintf(stdout, "in handleRead conn is nullptr\n");
       return;
       }
       */
    int fd = conn->getFd();
    // fprintf(stdout, "in handleRead conn %d \n", fd);
    int len;
    int current_len = 0;
    char *readBuf = conn->getReadBuffer();
    char chunck[CHUNCK_SIZE];
    for (;;) {
        // fprintf(stdout, "----in loop----\n");
        len = read(fd, chunck, sizeof(chunck));
        // fprintf(stdout, "read len :%d\n", len);
        if (len <= 0) {  // error or peer close or read complete
            break;
        } else {
            for (int i = 0; i < len; i++) {
                *readBuf = chunck[i];
                readBuf++;
                current_len++;
                // too much
                if (current_len >= DEFAULT_BUFFER_SIZE) {
                    break;
                }
            }
        }
    }
    // fprintf(stdout, "in handleRead conn %d finish read\n", fd);
    // fprintf(stdout, "current_size : %d\n", current_len);
    conn->setReadSize(current_len);
    // peer close
    if (len == 0) {
        // fprintf(stdout, "peer close the ret : %d\n", len);
        epoller_->removeConnection(conn->getFd());
        return;
    }
    if (len < 0) {
        // nonblocking read complete
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            // error close
            // fprintf(stdout, "---removeConnection : read erro:%d\n", len);
            epoller_->removeConnection(fd);
            return;
        }
    }

    // fprintf(stdout, "fd:%d read:%d\n", fd, conn->getReadSize());

    // read ok, register write
    // parse the request data then register the write event
    // fprintf(stdout, "finsh read connection :%d\n", fd);
    Request *req = new Request();
    Response *res = new Response();
    req->setConnection(conn);
    res->setConnection(conn);
    conn->setRequest(req);
    conn->setResponse(res);
    // fprintf(stdout, "before parseRequest\n");
    server->parseRequest(req);
    // fprintf(stdout, "before transform response\n");
    server->transformResponse(res);
    // fprintf(stdout, "before update event\n");
    epoller_->updateEvent(fd, EPOLLOUT | EPOLLET);
    conn->setState(WRITING);
    // fprintf(stdout, "after set write state\n");
    // fprintf(stdout, "finish handleRead conn %d\n", fd);
}

void ConnectionEpollHandler::handleWrite(Connection *conn) {
    // write to client socket stream and maybe close the socket if keepalived is
    // off
    // or just re-register the fd to poller waiting the next request

    /*
       if(conn == nullptr) { //why?
       fprintf(stdout, "in handleWrite conn is nullptr\n");
       return;
       }
       */
    int fd = conn->getFd();
    // fprintf(stdout, "in handleWrite conn %d\n", fd);
    char *writeBuf = conn->getWriteBuffer();
    int len = -1;
    int written = 0;
    int to_written = conn->getWriteSize();
    while (written < to_written) {
        len = write(fd, writeBuf + written, CHUNCK_SIZE);
        if (len < CHUNCK_SIZE) {
            break;
        }
        written += len;
    }
    // peer close
    if (len == 0 || (len < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))) {
        // fprintf(stdout, "---removeConnection : write erro:%d\n", len);
        epoller_->removeConnection(fd);
    } else {
        // ok, written finish, we check the keepalive flag
        // there is no timeout function for now, to be added
        if (server->isKeepalived()) {
            // fprintf(stdout, "update event after write : %d\n", fd);
            // readd to pollin state
            epoller_->updateEvent(fd, EPOLLIN | EPOLLET);
            conn->setState(CONNECTED);
        } else {
            // fprintf(stdout, "finsh write connection :%d\n", fd);
            epoller_->removeConnection(fd);
        }
    }
}
}
