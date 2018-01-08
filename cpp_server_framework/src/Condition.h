#ifndef Condition_H
#define Condition_H

#include <pthread.h>
#include "MutexLock.h"

namespace Tan {

class Condition {
 public:
  explicit Condition(MutexLock &mutex) : mutex_(mutex) {
    pthread_cond_init(&cond_, NULL);
  }

  ~Condition() { pthread_cond_destroy(&cond_); }

  void wait();
  void notify();
  void notifyAll();

 private:
  MutexLock mutex_;
  pthread_cond_t cond_;
};
}

#endif
