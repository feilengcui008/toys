#ifndef EpollThread_H
#define EpollThread_H

#include <algorithm>
#include <unordered_map>
#include <memory>

#include <sys/epoll.h>

#include "Thread.h"
#include "ConnectionEpollHandler.h"
#include "Connection.h"
#include "MutexLock.h"

namespace Tan {

class AbstractServer;

void *startEpollThread(void *args);

class EpollThread : public Thread {
 public:
  EpollThread() : Thread() {
    epollfd_ = -1;
    epoll_timeout_ = -1;
    max_events_ = 1024;
    events_ = NULL;
    running_ = false;
    initialized_ = false;
    // pthread_mutex_init(&connections_lock_, NULL);
  }
  ~EpollThread();
  void init();
  void startLoop();  // called by runBusiness
  void start() override;
  void runBusiness() override;
  void stopLoop();
  void addEvent(int fd, unsigned int event);
  void updateEvent(int fd, unsigned int event);
  void removeEvent(int fd);
  void handleRead(int fd);
  void handleWrite(int fd);

  void addConnection(Connection *conn);
  Connection *getConnection(int fd);
  void removeConnection(int fd);
  void setEpollTimeout(int n);

  // void setHandler(std::shared_ptr<ConnectionEpollHandler> &handler);
  void setHandler(ConnectionEpollHandler *handler);

 public:
  // AbstractServer *server;
  ConnectionEpollHandler *handler;
  // std::weak_ptr<AbstractServer> server;
  // std::weak_ptr<ConnectionEpollHandler> handler; //break cycle reference

 private:
  int epollfd_;
  int epoll_timeout_;
  unsigned max_events_;
  struct epoll_event *events_;
  // struct epoll_event ev_;
  bool initialized_;
  bool running_;
  // pthread_mutex_t connections_lock_;
  MutexLock mutex_;
  std::unordered_map<int, Connection *> connections_;
  // std::unordered_map<int, std::shared_ptr<Connection>> connections_;
};
}

#endif
