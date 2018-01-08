#ifndef Thread_H
#define Thread_H

#include <string>
#include <pthread.h>

#include "Utils.h"
#include "Noncopyable.h"

namespace Tan {
typedef void *(*Routine)(void *);
class Thread : Noncopyable {
 public:
  // need because we may do not use the routine
  // we dirrectly define the business logic in out child
  // class's member methods
  Thread() : detach_(true), name_(std::string("")) {};
  Thread(Routine routine, const std::string &name, bool detach = true);
  virtual ~Thread();
  // call pthread_create to create startThread routine, and pass
  // the thread pointer to it
  virtual void start();
  virtual void runBusiness();

  bool detach_;
  pthread_t pt_;
  Routine routine_;
  std::string name_;
};

// business logic of thread finished, we need to delete the Thread * if
// necessary
// because now we do not have thread cache
// in epoll mode, it will never get here until the event loop breaks
// very ugly...
void *startThread(void *args);
}

#endif
