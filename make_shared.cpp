#include <iostream>
#include "shared.h"
#include "detail.h"

template<typename T, typename... Args>
std::enable_if_t<!std::is_array<T>::value, SharedPointer<T>>
MakeShared(Args&&... args) {
    return SharedPointer<T>(new T(std::forward<Args>(args)...));
}

template<class T>
std::enable_if_t<detail::is_unbounded_array_v<T>, SharedPointer<T>>
MakeShared(size_t n)
{
    return SharedPointer<T>(new std::remove_extent_t<T>[n]());
}

template<class T, class... Args>
std::enable_if_t<detail::is_bounded_array_v<T>> MakeShared(Args&&...) = delete;

struct A {
    A() : a(0), b(0) {}
    A(int a_, int b_) : a(a_), b(b_) {}
    int a, b;
};

int main() {
    auto a = MakeShared<A>(6, 1);
    std::cout << a->a << a->b;

    auto b = MakeShared<A>();
    std::cout <<b->a;

    auto c = MakeShared<A[]>(5);
    std::cout << c[1].a;
}