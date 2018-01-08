#ifndef MutexLock_H
#define MutexLock_H

#include <pthread.h>
#include "Noncopyable.h"

namespace Tan {

class MutexLock {
 public:
  MutexLock();
  ~MutexLock();
  void lock();
  void unlock();
  pthread_mutex_t *getMutex();

 private:
  pthread_mutex_t mutex_;
};
}

#endif
