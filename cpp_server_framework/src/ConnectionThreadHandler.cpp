#include <memory>
#include <cassert>
#include "ConnectionThreadHandler.h"
#include "AbstractServer.h"

namespace Tan {

void *startHandlerThread(void *args) {
    Data *data = (Data *)args;
    ConnectionThreadHandler *td = data->handler;
    Connection *conn = data->conn;
    // fprintf(stdout, "in startHandlerThread %d\n", conn->getFd());
    if (td->detach_) {
        pthread_detach(pthread_self());
    }
    td->handleInThread(conn);
}

void ConnectionThreadHandler::handleConnection(Connection *conn) {
    // start running business in a new thread
    assert(conn != nullptr);
    // int fd = conn->getFd();
    // fprintf(stdout, "handleConnection : %d\n", fd);
    conn->setState(READING);
    // conns_[fd] = std::shared_ptr<Connection>(conn);
    Data *data = new Data(this, conn);
    int ret;
    ret = pthread_create(&pt_, NULL, startHandlerThread, (void *)data);
    if (ret < 0) {
        my_error_exit("create epoll thread error");
    }
}

void ConnectionThreadHandler::handleInThread(Connection *conn) {
    // read the data
    // fprintf(stdout, "%s %d\n", "in handleInThread", conn->getFd());
    handleRead(conn);
    Request *req = new Request();
    Response *res = new Response();
    req->setConnection(conn);
    res->setConnection(conn);
    conn->setRequest(req);
    conn->setResponse(res);
    server->parseRequest(req);
    server->transformResponse(res);
    handleWrite(conn);
    // fprintf(stdout, "%s\n", "====== in ConnectionThreadHandler::runBusiness
    // end
    // =======");
}

// spawn the thread and handle the connection in a new thread
void ConnectionThreadHandler::runBusiness() {}

void ConnectionThreadHandler::handleRead(Connection *conn) {
    int fd = conn->getFd();
    // fprintf(stdout, "in handleRead client fd : %d\n", fd);
    int len;
    int current_len = 0;
    char *readBuf = conn->getReadBuffer();
    char chunck[CHUNCK_SIZE];
    for (;;) {
        len = read(fd, chunck, sizeof(chunck));
        if (len < CHUNCK_SIZE) {
            break;
        } else {
            for (int i = 0; i < len; ++i) {
                *readBuf = chunck[i];
                readBuf++;
                current_len++;
            }
        }
    }
    if (len >= 0) {
        for (int i = 0; i < len; ++i) {
            *readBuf = chunck[i];
            readBuf++;
            current_len++;
        }
    }
    if (len < 0) {
        fprintf(stdout, "%s\n", "delete conn in handle read");
        assert(conn != nullptr);
        delete conn;
        conn = nullptr;
        close(fd);
        fprintf(stdout, "%s\n", "after close fd in handle read");
        my_error_exit("read from client socket error");
    }
    conn->setReadSize(current_len);
    conn->setState(WRITING);
    // fprintf(stdout, "finish in handleRead %d\n", fd);
}

void ConnectionThreadHandler::handleWrite(Connection *conn) {
    int fd = conn->getFd();
    // fprintf(stdout, "in handleWrite %d\n", fd);
    char *writeBuf = conn->getWriteBuffer();
    int len = -1;
    int written = 0;
    int to_written = conn->getWriteSize();
    while (written < to_written) {
        len = write(fd, writeBuf + written, CHUNCK_SIZE);
        if (len < CHUNCK_SIZE) break;
        written += len;
    }
    if (len < 0) {
        delete conn;
        conn = nullptr;
        close(fd);
        my_error_exit("error write buffer to client socket");
    }
    // if(server->isKeepalived()) {
    // todo
    //} else {
    delete conn;
    conn = nullptr;
    close(fd);
    //}
    // fprintf(stdout, "finish in handle write %d\n", fd);
}
}
