#pragma once

template<typename T>
class UniquePointer {
public:
	// ctors 
	UniquePointer();

	explicit UniquePointer(T* );

	// for std::move
	UniquePointer(UniquePointer<T>&& );

	// operators

	T& operator*() const noexcept;

	UniquePointer<T>& operator=(UniquePointer<T>&& ) noexcept;

	explicit operator bool() const;

	// functions

	T* get() const noexcept;

	void swap(UniquePointer<T>& other) noexcept;

	void reset(T* newptr = nullptr) noexcept;

	T* release() noexcept;

private:
	T* ptr;
	// copy and assignment not allowed
	UniquePointer(const T&) = delete;
	void operator=(UniquePointer<T>&) = delete;
};