// Copyright [2016] <feilengcui008@gmail.com>
#ifndef SRC_ABSTRACTSERVER_H_
#define SRC_ABSTRACTSERVER_H_

#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h>
#include <netdb.h>

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cstring>
#include <cstdio>

#include "Noncopyable.h"
#include "Utils.h"
#include "Thread.h"
#include "EpollThread.h"
#include "Connection.h"
#include "ConnectionThreadHandler.h"
#include "ConnectionProcessHandler.h"
#include "ConnectionEpollHandler.h"
#include "MutexLock.h"

/*
 * an AbstractServer
 *
 * this class includes the basic stuff of a server
 *
 * you can extends this erver and implement the virtual methods to realize
 * your specific application server, mainly for different read and write logic
 * as well as the business logics
 *
 * */

namespace Tan {

#define SERVER_POLL_NUMBER 1
enum MODE {
  PROCESS = 1,
  THREAD,
  EPOLL,
  DIRECT
};

class Request;
class Response;

class AbstractServer : Noncopyable {
 public:
  AbstractServer(std::string host, unsigned short port,
                 unsigned int backlog = 10);
  virtual ~AbstractServer();
  /*
   * AbstractServer(AbstractServer &&server) = delete;
   * AbstractServer(const AbstractServer &server) = delete;
   * AbstractServer &operator=(const AbstractServer &server) = delete;
   */

 public:
  void init();   // create the needed sockets
  void start();  // start event loop
  void stop();   // stop the event loop

 public:
  // for different mode, we overwrite this method in child class
  // to handle client connections, use fork or pthread creat or just
  // add the client socket into the server epoll
  virtual void handleConnection(Connection *conn);

  // for testing
  virtual void handleConnectionDirect(Connection *conn);

  // we define our read write logic in child server
  // these will be passed to EpollThread to read or
  // write client connection if in EPOLL mode
  // virtual void handleRead(Connection *conn);
  // virtual void handleWrite(Connection *conn);
  virtual void parseRequest(Request *req);

  virtual void transformResponse(Response *res);

  bool isKeepalived() const;
  void setKeepAlived(bool flag);

  void setMode(MODE m);
  MODE getMode() const;

  // std::shared_ptr<ConnectionEpollHandler> &getEpollHandler();
  // std::shared_ptr<ConnectionThreadHandler> &getThreadHandler();
  // std::shared_ptr<ConnectionProcessHandler> &getProcessHandler();
  ConnectionEpollHandler *getEpollHandler();
  ConnectionThreadHandler *getThreadHandler();
  ConnectionProcessHandler *getProcessHandler();

  void setPollTimeout(int n);

  void parseConf(std::string path);
  void printConf() const;

 protected:
  std::string host_;     // listen host or ip
  unsigned short port_;  // listen port
  unsigned int backlog_;
  MODE mode_;            // mode for handle client sockets
  std::string rootdir_;  // working dir
  std::string logdir_;   // log dir
  bool keepalived_;      // is keepalive
  unsigned int timeout_;
  unsigned int max_connection_;
  bool reuseaddr_;
  int serverfd_;
  bool nonblocking_;
  // the sockets needed by server very few,
  // so we just use poll other than epoll
  struct pollfd pollfds_[SERVER_POLL_NUMBER];
  int poll_timeout_;
  bool initialized_;
  bool running_;
  unsigned int current_connection_num_;
  unsigned int history_max_connection_;

 protected:
  // for multi-threaded mode
  // std::shared_ptr<ConnectionThreadHandler> threadHandler_;
  ConnectionThreadHandler *threadHandler_;
  // std::map<int, Thread *> pts_;
  MutexLock mutex_;
  // pthread_mutex_t pts_lock_;

 protected:
  // std::shared_ptr<ConnectionProcessHandler> processHandler_;
  // for multi-process mode
  ConnectionProcessHandler *processHandler_;

 protected:
  // std::shared_ptr<ConnectionEpollHandler> epollHandler_;
  ConnectionEpollHandler *epollHandler_;
  // EpollThread *epoller_;
};

}  // namespace Tan

#endif  // SRC_ABSTRACTSERVER_H_
