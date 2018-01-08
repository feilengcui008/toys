#include <cassert>
#include "Thread.h"

namespace Tan {

Thread::Thread(Routine routine, const std::string &name, bool detach) {
    routine_ = routine;
    name_ = name;
    detach_ = detach;
}

Thread::~Thread() {}

void Thread::start() {}

// leave for child
void Thread::runBusiness() {}
}
