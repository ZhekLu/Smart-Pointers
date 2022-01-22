#include <iostream>
#include "unique.h"
#include "detail.h"

template<typename T, typename... Args>
std::enable_if_t<!std::is_array<T>::value, UniquePointer<T>>
MakeUnique(Args&&... args) {
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}

template<class T>
std::enable_if_t<detail::is_unbounded_array_v<T>, UniquePointer<T>>
MakeUnique(size_t n)
{
    return UniquePointer<T>(new std::remove_extent_t<T>[n]());
}

template<class T, class... Args>
std::enable_if_t<detail::is_bounded_array_v<T>> MakeUnique(Args&&...) = delete;

struct A {
    A() : a(0), b(0) {}
    A(int a_, int b_) : a(a_), b(b_) {}
    int a, b;
};

int main() {
    auto a = MakeUnique<A>(6, 1);
    std::cout << a->a << a->b;

    auto b = MakeUnique<A>();
    std::cout <<b->a;

    auto c = MakeUnique<A[]>(5);
    std::cout << c[1].a;
}