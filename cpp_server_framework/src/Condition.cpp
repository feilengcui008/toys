#include "Condition.h"

namespace Tan {

void Condition::wait() { pthread_cond_wait(&cond_, mutex_.getMutex()); }

void Condition::notify() { pthread_cond_signal(&cond_); }

void Condition::notifyAll() { pthread_cond_broadcast(&cond_); }
}
