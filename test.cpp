#include "unique.h"
#include "test_type.h"
#include <assert.h>
#include <algorithm>
#include <iostream>

void Test_init() {
    int* p1 = new int(1);
    int* p2 = new int(2);

    UniquePointer<int> s1(p1);
    UniquePointer<int> s2(p2);

    assert(s1.get() == p1);
    assert(s2.get() == p2);
    assert(*s1 == 1);
    assert(*s2 == 2);
    std::cout << "Test init : OK" << std::endl;
}

void Test_swap() {
    std::cout << "Test swap : " << std::endl;
    TestType* p1 = new TestType(1);
    TestType* p2 = new TestType(2);
    UniquePointer<TestType> s1(p1);
    UniquePointer<TestType> s2(p2);

    s1.swap(s2);

    assert(s1.get() == p2);
    assert(*s1 == 2);
    assert(s2.get() == p1);
    assert(*s2 == 1);
    assert(TestType::AliveCount() == 2);
    std::cout << "\tCustom swap : OK" << std::endl;
    
    std::swap(s1, s2);

    assert(s1.get() == p1);
    assert(*s1 == 1);
    assert(s2.get() == p2);
    assert(*s2 == 2);
    std::cout << "\tstd::swap : OK" << std::endl;
}

void Test_move() {
    UniquePointer<TestType> s1(new TestType(42));
    TestType* p1 = s1.get();
    UniquePointer<TestType> s2(new TestType(1));

    assert(TestType::AliveCount() == 2);

    s2 = std::move(s1);

    assert(s2.get() == p1);
    assert(s1.get() == nullptr);
    assert(TestType::AliveCount() == 1);
    std::cout << "Test move : OK" << std::endl;
}

void Test_move_inctor() {
    UniquePointer<TestType> s1(new TestType(42));
    TestType* p1 = s1.get();
    UniquePointer<TestType> s2(std::move(s1));

    assert(s2.get() == p1);
    assert(s1.get() == nullptr);
    assert(TestType::AliveCount() == 1);
    std::cout << "Test move in consructor : OK" << std::endl;
}

int main() {
    Test_init();
    Test_swap();
    Test_move();
    Test_move_inctor();
}