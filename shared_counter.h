#pragma once
#include <iostream>

class SharedCounter {
public:
    explicit SharedCounter(size_t* ptr = nullptr) {
        this-> ptr = ptr;
    }

    void inc() noexcept {
        if (ptr)
            *ptr++;
        else
            ptr = new size_t(1);
    }

    // return true if object must be removed
    bool dec() noexcept {
        *ptr--;
        if (!*ptr) {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }

    void reset(size_t* newptr = nullptr) noexcept {
        this->dec();
        ptr = newptr;
    }

    void drop() noexcept {
        ptr = nullptr;
    }

    size_t get_value() const noexcept {
        if (ptr)
            return *ptr;
        return 0;
    }

    ~SharedCounter() {
        if (ptr)
            delete ptr;
    }
private:
    size_t* ptr;
};