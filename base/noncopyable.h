#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

class noncopyable {
    public: 
        noncopyable(const noncopyable&) = delete;
        const noncopyable& operator=(const noncopyable&) = delete;
    protected:
        noncopyable() = default;
        ~noncopyable() = default; 
}; // class noncopyable
#endif // BASE_NONCOPYABLE_H