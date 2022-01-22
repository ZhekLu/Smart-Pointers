#pragma once
#include <iostream>

template<typename T>
class UniquePointer {
public:
	// ctors

	explicit UniquePointer(T* ptr = nullptr) {
		this->ptr = ptr;
	}

	// for std::move
	UniquePointer(UniquePointer<T>&& x) noexcept : ptr(x.ptr) {
		x.ptr = nullptr;
	}

	~UniquePointer() {
		if (ptr)
			delete ptr;
	}

	// operators

	T& operator*() const noexcept {
		return *ptr;
	}

	T* operator->() const noexcept {
		return ptr;
	}

	UniquePointer<T>& operator=(UniquePointer<T>&& other) noexcept {
		if (&other == this)
			return *this;

		if (ptr)
			delete ptr;

		this->ptr = other.ptr;
		other.ptr = nullptr;

		return *this;
	}

	explicit operator bool() const {
		return ptr;
	}

	// functions

	T* get() const noexcept {
		return ptr;
	}

	void swap(UniquePointer<T>& other) noexcept {
		T* temp = this->ptr;
		this->ptr = other.ptr;
		other.ptr = temp;
	}

	void reset(T* newptr = nullptr) noexcept {
		if (newptr == ptr)
			return;
		if (ptr)
			delete ptr;
		ptr = newptr;
	}

	T* release() noexcept {
		auto mem = ptr;
		this->ptr = nullptr;
		return mem;
	}

private:
	T* ptr;

	// copy and assignment not allowed
	UniquePointer(const T&) = delete;
	void operator=(UniquePointer<T>&) = delete;
};


// Special for arrays
template <typename T> 
class UniquePointer<T[]> {
public:
	// ctors

	explicit UniquePointer(T* ptr = nullptr) {
		this->ptr = ptr;
	}

	// for std::move
	UniquePointer(UniquePointer<T>&& x) noexcept : ptr(x.ptr) {
		x.ptr = nullptr;
	}

	~UniquePointer() {
		if (ptr)
			delete[] ptr;
	}

	// operators

	T& operator*() const noexcept {
		return *ptr;
	}

	T* operator->() const noexcept {
		return ptr;
	}

	UniquePointer<T>& operator=(UniquePointer<T>&& other) noexcept {
		if (&other == this)
			return *this;
		
		if (ptr)
			delete[] ptr;

		this->ptr = other.ptr;
		other.ptr = nullptr;

		return *this;
	}

	explicit operator bool() const {
		return ptr;
	}

	T& operator[](size_t index) {
        return ptr[index];
    }

    const T& operator[](size_t index) const {
        return ptr[index];
    }

	// functions

	T* get() const noexcept {
		return ptr;
	}

	void swap(UniquePointer<T>& other) noexcept {
		std::swap(this->ptr, other.ptr);
	}

	void reset(T* newptr = nullptr) noexcept {
		if (newptr == ptr)
			return;
		if (ptr)
			delete[] ptr;
		ptr = newptr;
	}

	T* release() noexcept {
		auto mem = ptr;
		this->ptr = nullptr;
		return mem;
	}

private:
	T* ptr;

	// copy and assignment not allowed
	UniquePointer(const T&) = delete;
	void operator=(UniquePointer<T>&) = delete;
};