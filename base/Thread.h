// @author qwq
// @Email 1284095130@qq.com

#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include <functional>
#include <string>
#include <sys/types.h>
#include "CountDownLatch.h"
#include "noncopyable.h"

class Thread : public noncopyable {
    public:
        typedef std::function<void()> ThreadFunc;
        explicit Thread(ThreadFunc, const std::string& name = std::string());
        ~Thread();

        void start();
        int join();

        bool started() const { return started_; }
        pid_t pid() const { return tid_; }
        const std::string name() const { return name_; }
        
    private:
        void setDefaultName();
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        pid_t tid_;
        /* the calling function of current thread */
        ThreadFunc func_; 
        std::string name_;
        CountDownLatch latch_;
}; // class Thread
#endif // BASE_THREAD_H