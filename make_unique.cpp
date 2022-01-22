#include <iostream>
#include "unique.h"

template<typename T, typename... Args>
std::enable_if_t<!std::is_array<T>::value, UniquePointer<T>>
MakeUnique(Args&&... args) {
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}


struct A {
    A() : a(0), b(0) {}
    A(int a_, int b_) : a(a_), b(b_) {}
    int a, b;
};

int main() {
    auto a = MakeUnique<A>(4, 2);
    std::cout << a->a << a->b;

    auto b = MakeUnique<A>();
    std::cout <<b->a;
}