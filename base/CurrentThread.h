// @author qwq
// @Email 1284095130@qq.com
#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H

#include <stdint.h>
namespace CurrentThread {
    // internal
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    void cacheTid();

    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString() {
        return t_tidString;
    }

    inline int tidStringLength() {
        return t_tidStringLength;
    }

    inline const char* name() {
        return t_threadName;
    }
} // CurrentThread
#endif // BASE_CURRENTTHREAD_H