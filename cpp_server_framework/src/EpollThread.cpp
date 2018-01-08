#include <iostream>
#include <cassert>

#include <pthread.h>

#include "EpollThread.h"
#include "AbstractServer.h"
#include "MutexLockGuard.h"

namespace Tan {

void *startEpollThread(void *args) {
    Thread *td = (Thread *)args;
    if (td->detach_) {
        pthread_detach(pthread_self());
    }
    td->runBusiness();
}

EpollThread::~EpollThread() {
    if (events_ != NULL) {
        free(events_);
    }
    // pthread_mutex_destroy(&connections_lock_);
    close(epollfd_);
}

void EpollThread::start() {
    int ret;
    ret = pthread_create(&pt_, NULL, startEpollThread, this);
    if (ret < 0) {
        my_error_exit("create epoll thread error");
    }
}

void EpollThread::setHandler(ConnectionEpollHandler *h) { handler = h; }

void EpollThread::init() {
    if (initialized_) return;
    epollfd_ = epoll_create(max_events_);
    if (epollfd_ < 0) {
        my_error_exit("epollthread error : create epollfd failed\n");
    }
    events_ =
        (struct epoll_event *)malloc(sizeof(struct epoll_event) * max_events_);
    if (!events_) {
        my_error_exit("epollthread error : malloc memory for events failed\n");
    }
    // pass to connection handle
    initialized_ = true;
}

// rewrite the business
void EpollThread::runBusiness() {
    if (running_) return;
    if (!initialized_) init();
    startLoop();
}

void EpollThread::startLoop() {
    running_ = true;
    int epoll_ret;
    while (running_) {
        epoll_ret = epoll_wait(epollfd_, events_, max_events_, epoll_timeout_);
        if (epoll_ret < 0) {
            my_error_exit("epollthread eventloop error : epoll_wait error\n");
        }
        // check the EPOLLIN and EPOLLOUT events
        for (int i = 0; i < epoll_ret; i++) {
            if (events_[i].events & EPOLLIN) {  // request data comming
                // assert(connections_[events_[i].data.fd] != nullptr);
                handleRead(events_[i].data.fd);
            }
            if (events_[i].events & EPOLLOUT) {  // data can be sent out
                handleWrite(events_[i].data.fd);
            }
        }
    }
}

void EpollThread::stopLoop() { running_ = false; }

void EpollThread::addEvent(int fd, unsigned int event) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) {
        my_error_exit("addEvent error\n");
    }
}

void EpollThread::updateEvent(int fd, unsigned int event) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = event;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev);
    if (ret < 0) {
        my_print_info("updateEvent error\n");
    }
}

void EpollThread::removeEvent(int fd) {
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, events_);
    if (ret < 0) {
        my_print_info("removeEvent error\n");
    }
    // to do
    // move the according Connection out of main thread map<int, Connection *>
}

void EpollThread::handleRead(int fd) {
    Connection *conn = getConnection(fd);
    assert(conn != nullptr);
    // if(conn == nullptr)
    //    fprintf(stdout, "----before handle read conn is nullptr : %d\n", fd);
    handler->handleRead(conn);
}

void EpollThread::handleWrite(int fd) {
    Connection *conn = getConnection(fd);
    assert(conn != nullptr);
    // if(conn == nullptr)
    //    fprintf(stdout, "----before handle write conn is nullptr : %d\n", fd);
    handler->handleWrite(conn);
}

void EpollThread::addConnection(Connection *conn) {
    // fprintf(stdout, "add connection : %d\n", conn->getFd());
    // assert(conn != nullptr);
    MutexLockGuard guard(mutex_);
    int fd = conn->getFd();
    if (getConnection(fd)) {
        fprintf(stdout,
                "the same connection  %d exists, this should never happen\n",
                fd);
        delete conn;
        close(fd);
        return;
    }
    // we must set the connection before set event state !!!
    // otherwise the race condition will happen between handlleRead
    conn->setState(CONNECTED);
    connections_[fd] = conn;
    addEvent(fd, EPOLLIN | EPOLLET);
    // pthread_mutex_unlock(&connections_lock_);
}

Connection *EpollThread::getConnection(int fd) { return connections_[fd]; }

void EpollThread::removeConnection(int fd) {
    // fprintf(stdout, "remove fd %d\n", fd);
    MutexLockGuard guard(mutex_);
    // pthread_mutex_lock(&connections_lock_);
    if (getConnection(fd) != nullptr) {
        // we must remove the event state
        // then delete the connection
        // and last close the fd
        removeEvent(fd);
        delete connections_[fd];
        connections_[fd] = nullptr;
        close(fd);
        // pthread_mutex_unlock(&connections_lock_);
        // fprintf(stdout, "end of remove connection : %d\n", fd);
        return;
    }
    // pthread_mutex_unlock(&connections_lock_);
}

void EpollThread::setEpollTimeout(int n) { epoll_timeout_ = n; }
}
