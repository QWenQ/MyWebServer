#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <assert.h>
#include "base/CurrentThread.h"
#include "base/noncopyable.h"

class EventLoop : public noncopyable {
    public: 
        EventLoop();
        ~EventLoop();
        
        void loop();

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
        
        static EventLoop* getEventLoopOfCurrentThread();
    private:
        void abortNotInLoopThread();
        
        bool looping_;
        const pid_t threadId_;
}; // class EventLoop
#endif // EVENTLOOP_H