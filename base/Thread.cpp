// @author qwq
// @Email 1284095130@qq.com
#include <iostream>
#include <utility>
#include <pthread.h>
#include <sys/prctl.h>
#include <assert.h>
#include "Thread.h"
#include "CurrentThread.h"

namespace CurrentThread {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "default";
}

pid_t gettid() { return static_assert<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
        t_tidStringLength =
            snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
    }

}

struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(ThreadData func, const std::string name, 
                pid_t* tid, CountDownLatch* latch) 
        : func_(func), name_(name), tid_(tid), latch_(latch) 
    { }

    void runInThread() {
        *tid_ = CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);

        // 线程真正的运行逻辑
        func_();
        CurrentThread::t_threadName = "finished";
    }

}; // struct ThreadData

void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}


Thread::Thread(ThreadFunc func, const std::string& name) 
    :   started_(false), 
        joined_(false), 
        pthreadId_(0),
        tid_(0), 
        func_(std::move(func)), 
        name_(name), 
        latch_(1)
{
    setDefaultName();

}

Thread::~Thread() {
    if (started_ && !joined_) pthread_detach(pthreadId_);
}

void Thread::start() {
    assert(!started_);
    started_ = true;

    ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
    if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
        started_ = false;
        delete data;
    }
    else {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

void Thread::setDefaultName() {
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Default Thread");
        name_ = buf;
    }
}