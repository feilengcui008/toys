#ifndef LockGuard_H

#define LockGuard_H

#include "MutexLock.h"

namespace Tan {

class MutexLockGuard {
 public:
  explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex_.lock(); }
  ~MutexLockGuard() { mutex_.unlock(); }

 private:
  MutexLock &mutex_;
};
}

#endif
