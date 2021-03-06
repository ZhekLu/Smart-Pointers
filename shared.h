#pragma once
#include <iostream>
#include "shared_counter.h"

template<typename T>
class SharedPointer {
public:
	// ctors

	explicit SharedPointer(T* ptr = nullptr) {
		this->ptr = ptr;
		_inc();
	}

	SharedPointer(SharedPointer<T>&& x) noexcept : ptr(x.ptr), count(x.count) {
		x.ptr = nullptr;
        x.count.drop();
	}

    SharedPointer(const SharedPointer& other) noexcept {
        this->ptr = other.ptr; 
        this->count = other.count;
		_inc();
    }

	~SharedPointer() {
		_dec();
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

		_dec();

		this->ptr = other.ptr;
        this->count = other.count;
        other.ptr = nullptr;
        other.count.drop();

		return *this;
	}

	SharedPointer& operator=(SharedPointer& other) noexcept {
        if (&other == this)
			return *this;

		_dec();

		this->ptr = other.ptr;
        this->count = other.count;
		_inc();

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
		_dec();
		ptr = newptr;
        _inc();
	}

    size_t use_count() const noexcept {
        return count.get_value();
    }

private:
	T* ptr;
    SharedCounter count;

	void _inc() {
		if (ptr)
			count.inc();
	}

	void _dec() {
		if (!ptr)
			return;
		if (count.dec()) {
			delete ptr;
			ptr = nullptr;
		}
	}
};


// Special for arrays
template <typename T> 
class SharedPointer<T[]> {
public:
	// ctors

	explicit SharedPointer(T* ptr = nullptr) {
		this->ptr = ptr;
		_inc();
	}

	SharedPointer(SharedPointer<T>&& x) noexcept : ptr(x.ptr), count(x.count) {
		x.ptr = nullptr;
        x.count.drop();
	}

    SharedPointer(const SharedPointer& other) noexcept {
        this->ptr = other.ptr; 
        this->count = other.count;
		_inc();
    }

	~SharedPointer() {
		_dec();
	}

	// operators

	T& operator*() const {
		return *ptr;
	}

	T* operator->() const {
		return ptr;
	}

	T& operator[](size_t index) {
        return ptr[index];
    }

    const T& operator[](size_t index) const {
        return ptr[index];
    }

	SharedPointer& operator=(SharedPointer&& other) noexcept {
		if (&other == this)
			return *this;

		_dec();

		this->ptr = other.ptr;
        this->count = other.count;
        other.ptr = nullptr;
        other.count.drop();

		return *this;
	}

	SharedPointer& operator=(SharedPointer& other) noexcept {
        if (&other == this)
			return *this;

		_dec();

		this->ptr = other.ptr;
        this->count = other.count;
		_inc();

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
		_dec();
		ptr = newptr;
        _inc();
	}

    size_t use_count() const noexcept {
        return count.get_value();
    }

private:
	T* ptr;
    SharedCounter count;

	void _inc() {
		if (ptr)
			count.inc();
	}

	void _dec() {
		if (!ptr)
			return;
		if (count.dec()) {
			delete[] ptr;
			ptr = nullptr;
		}
	}
};