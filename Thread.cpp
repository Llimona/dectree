#include "Thread.h"

static void startThread(Thread* t) {
    t->run();
}

Thread::Thread() {}

Thread::~Thread() {}

void Thread::start() {
    t = std::thread(startThread, this);
}

void Thread::join() {
    t.join();
}