#include "unique.h"
#include "test_type.h"
#include <assert.h>
#include <algorithm>
#include <iostream>

void Test_init() {
    TestType* p1 = new TestType(13);

    UniquePointer<TestType> s1(p1);
    UniquePointer<TestType> s2;

    assert(s1.get() == p1);
    assert(s2.get() == nullptr);
    assert(*s1 == 13);
    std::cout << "Test init : OK" << std::endl;
}

void Test_get() {
    {
        int* p = new int(1);

        UniquePointer<int> s(p);
        UniquePointer<int> const& sc = s;

        assert(s.get() == p);
        assert(sc.get() == s.get());
    }
    {
        const int* p = new int(1);

        UniquePointer<const int> s(p);
        UniquePointer<const int> const& sc = s;

        assert(s.get() == p);
        assert(sc.get() == s.get());
    }
    std::cout << "Test get : OK" << std::endl;
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

void _simple_move() {
    UniquePointer<TestType> s1(new TestType(42));
    TestType* p1 = s1.get();
    UniquePointer<TestType> s2(new TestType(1));

    assert(TestType::AliveCount() == 2);

    s2 = std::move(s1);

    assert(s2.get() == p1);
    assert(s1.get() == nullptr);
    assert(TestType::AliveCount() == 1);
    std::cout << "\tsimple std::move : OK" << std::endl;
}

void _move_inctor() {
    UniquePointer<TestType> s1(new TestType(42));
    TestType* p1 = s1.get();
    UniquePointer<TestType> s2(std::move(s1));

    assert(s2.get() == p1);
    assert(s1.get() == nullptr);
    assert(TestType::AliveCount() == 1);
    std::cout << "\tmove in consructor : OK" << std::endl;
}

void _self_move() {
    UniquePointer<TestType> s(new TestType(666));
    TestType* p = s.get();
    s = std::move(s);

    assert(TestType::AliveCount() == 1);
    assert(s.get() == p);
    assert(*s == 666);
    std::cout << "\tself move : OK" << std::endl;
}

void Test_move() {
    std::cout << "Test move :" << std::endl;
    _simple_move();
    _move_inctor();
    _self_move();
}

void Test_release() {
    UniquePointer<TestType> s(new TestType(42));
    TestType* p = s.get();
    TestType* pr = s.release();

    assert(TestType::AliveCount() == 1);
    assert(s.get() == nullptr);
    assert(p == pr);
    assert(*pr == 42);

    delete p;
    assert(TestType::AliveCount() == 0);
    std::cout << "Test release : OK" << std::endl;
}

void _reset() {
    UniquePointer<TestType> s(new TestType);

    assert(TestType::AliveCount() == 1);
    
    // no arg = null
    s.reset();

    assert(s.get() == nullptr);
    assert(TestType::AliveCount() == 0);

    TestType* p = new TestType;
    // test reset after null reset
    s.reset(p);

    assert(s.get() == p);
    assert(TestType::AliveCount() == 1);
}

void _reset_const() {
    
    UniquePointer<const TestType> s(new TestType);
    assert(TestType::AliveCount() == 1);

    const TestType* p = s.get();
    assert(p);

    TestType* new_p = new TestType;
    assert(TestType::AliveCount() == 2);

    s.reset(new_p);

    assert(s.get() == new_p);
    assert(TestType::AliveCount() == 1);
}

void _self_reset() {
    
    UniquePointer<TestType> s(new TestType);
    TestType* p = s.get();

    assert(TestType::AliveCount() == 1);

    s.reset(p);

    assert(s.get() == p);
    assert(TestType::AliveCount() == 1);
}

void Test_reset() {
    _reset();
    _reset_const();
    _self_reset();
    std::cout << "Test reset : OK" << std::endl;
}

void Test_operator_bool() {
    UniquePointer<int> p(new int(1));
    UniquePointer<int> const& cp = p;

    assert(p);
    assert(cp);

    UniquePointer<int> p0;
    UniquePointer<int> const& cp0 = p0;

    assert(!p0);
    assert(!cp0);

    std::cout << "Test bool operator : OK" << std::endl;
}

void Test_operator_arrow() {
    struct A {
        int data{42};
    };

    UniquePointer<A> p(new A);
    assert(p->data == 42);
    
    std::cout << "Test -> operator : OK" << std::endl;
}

// for arrays
void Test_delete() {
    UniquePointer<TestType[]> s(new TestType[300]);
    assert(TestType::AliveCount() == 300);
    
    s.reset();
    assert(TestType::AliveCount() == 0);

    std::cout << "Test reset & delete : OK" << std::endl;
}

void Test_branch_operator() {
    int size = 7;
    int* array = new int[size]{1, 2, 3, 5, 8, 13, 21};
    UniquePointer<int[]> s(array);
    for (int i = 0; i < size; i++) {
        assert(s[i] == array[i]);
        s[i] = -42 + i;
        assert(s[i] == -42 + i);
    }
    
    std::cout << "Test [] operator : OK" << std::endl;
}

int main() {
    std::cout << "Test basic realization." << std::endl;
    Test_init();
    Test_get();
    Test_swap();
    Test_move();
    Test_release();
    Test_reset();
    Test_operator_bool();
    Test_operator_arrow();

    std::cout << "Test array specialization." << std::endl;
    Test_delete();
    Test_branch_operator();
}