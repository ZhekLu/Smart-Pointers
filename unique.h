#pragma once

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
		delete ptr;
	}

	// operators

	T& operator*() const noexcept {
		return *ptr;
	}

	UniquePointer<T>& operator=(UniquePointer<T>&& x) noexcept {
		if (&x == this)
			return *this;

		delete ptr;

		this->ptr = x.ptr;
		x.ptr = nullptr;

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