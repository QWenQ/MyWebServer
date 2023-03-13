#include <iostream>
#include <poll.h>
#include "EventLoop.h"

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
:   looping_(false),
    threadId_(CurrentThread::tid())
    {
        if (t_loopInThisThread) {
            // LOG_FATAL << "Another EventLoop " << t_loopInThisThread
            //  << " exists in this thread " << threadId_;
        }
        else {
            t_loopInThisThread = this;
        }
    }

EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;

    // do nothing and quit after 5 seconds
    ::poll(NULL, 0, 5 * 1000);
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    /*
      LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
    */
   std::cerr << "abort not int loop thread." << std::endl;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}