#pragma once
#include <iostream>

template<typename T>
class SharedPointer {
public:
	// ctors

	explicit SharedPointer(T* ptr = nullptr) {
		this->ptr = ptr;
        if  (ptr)
            count = new size_t(1);
        else 
            count = nullptr;
	}

	SharedPointer(SharedPointer<T>&& x) noexcept : ptr(x.ptr), count(x.count) {
		x.ptr = nullptr;
        x.count = nullptr;
	}

    SharedPointer(const SharedPointer& other) noexcept {
        this->ptr = other.ptr; 
        this->count = other.count;
        if (count)
            *count++;
    }

	~SharedPointer() {
		if (count) {
            *count--;
            if (!*count) {
                delete count;
                delete ptr;
            }
        }
	}

	// operators

	T& operator*() const {
		return *ptr;
	}

	T* operator->() const {
		return ptr;
	}

	SharedPointer& operator=(SharedPointer&& other) noexcept {
		if (&other == this)
			return *this;

		if (count && !--(*count) && ptr) {
			delete ptr;
            delete count;
        }

		this->ptr = other.ptr;
        this->count = other.count;
        other.ptr = nullptr;
        other.count = nullptr;

		return *this;
	}

	SharedPointer& operator=(SharedPointer& other) noexcept {
        if (&other == this)
			return *this;

		if (count && !--(*count) && ptr) {
			delete ptr;
            delete count;
        }

		this->ptr = other.ptr;
        this->count = other.count;
        *count++;

		return *this;
    } 

	explicit operator bool() const {
		return ptr;
	}

	// methods

	T* get() const noexcept {
		return ptr;
	}

	void swap(SharedPointer<T>& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(count, other.count);
	}

	void reset(T* newptr = nullptr) noexcept {
		if (newptr == ptr)
			return;
		this->~SharedPointer();
		ptr = newptr;
        if (ptr)
            count = new size_t(1);
        else
            count = nullptr;
	}

    size_t use_count() const noexcept {
        if (count)
            return *count;
        return 0;
    }

private:
	T* ptr;
    size_t* count;
};


// // Special for arrays
// template <typename T> 
// class SharedPointer<T[]> {
// public:
// 	// ctors

// 	explicit UniquePointer(T* ptr = nullptr) {
// 		this->ptr = ptr;
// 	}

// 	// for std::move
// 	UniquePointer(UniquePointer<T>&& x) noexcept : ptr(x.ptr) {
// 		x.ptr = nullptr;
// 	}

// 	~UniquePointer() {
// 		if (ptr)
// 			delete[] ptr;
// 	}

// 	// operators

// 	T& operator*() const noexcept {
// 		return *ptr;
// 	}

// 	T* operator->() const noexcept {
// 		return ptr;
// 	}

// 	UniquePointer<T>& operator=(UniquePointer<T>&& other) noexcept {
// 		if (&other == this)
// 			return *this;
		
// 		if (ptr)
// 			delete[] ptr;

// 		this->ptr = other.ptr;
// 		other.ptr = nullptr;

// 		return *this;
// 	}

// 	explicit operator bool() const {
// 		return ptr;
// 	}

// 	T& operator[](size_t index) {
//         return ptr[index];
//     }

//     const T& operator[](size_t index) const {
//         return ptr[index];
//     }

// 	// functions

// 	T* get() const noexcept {
// 		return ptr;
// 	}

// 	void swap(UniquePointer<T>& other) noexcept {
// 		std::swap(this->ptr, other.ptr);
// 	}

// 	void reset(T* newptr = nullptr) noexcept {
// 		if (newptr == ptr)
// 			return;
// 		if (ptr)
// 			delete[] ptr;
// 		ptr = newptr;
// 	}

// 	T* release() noexcept {
// 		auto mem = ptr;
// 		this->ptr = nullptr;
// 		return mem;
// 	}

// private:
// 	T* ptr;

// 	// copy and assignment not allowed
// 	UniquePointer(const T&) = delete;
// 	void operator=(UniquePointer<T>&) = delete;
// };