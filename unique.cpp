#include "unique.h"

// ctors 

template<typename T>
inline UniquePointer<T>::UniquePointer() : ptr(nullptr) {}

template<typename T>
UniquePointer<T>::UniquePointer(T* ptr) {
	this->ptr = ptr;
}

template<typename T>
UniquePointer<T>::UniquePointer(UniquePointer<T>&& x) : ptr(x.ptr) {
	x.ptr = nullptr;
}


// operators

template<typename T>
T& UniquePointer<T>::operator*() const noexcept {
	return *ptr;
}

template<typename T>
UniquePointer<T>& UniquePointer<T>::operator=(UniquePointer<T>&& x) noexcept {
	if (&x == this)
		return *this;

	delete ptr;

	this->ptr = x.ptr;
	x.ptr = nullptr;

	return *this;
}

template<typename T>
UniquePointer<T>::operator bool() const {
	return ptr;
}


// functions

template<typename T>
T* UniquePointer<T>::get() const noexcept {
	return ptr;
}

template<typename T>
void UniquePointer<T>::swap(UniquePointer<T>& other) noexcept {
	T* temp = this->ptr;
	this->ptr = other.ptr;
	other.ptr = temp;
}

template<typename T>
void UniquePointer<T>::reset(T* newptr) noexcept {
	if (ptr)
		delete ptr;
	ptr = newptr;
}

template<typename T>
T* UniquePointer<T>::release() noexcept {
	auto mem = ptr;
	this->ptr = nullptr;
	return mem;
}