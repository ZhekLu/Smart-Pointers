#include "shared.h"
#include "test_type.h"
#include <assert.h>
#include <algorithm>
#include <iostream>

void _empty_init() {
    SharedPointer<TestType> a, b; 
    b = a;

    SharedPointer<TestType> c(a);

    assert(!a.get());
    assert(!b.get());
    assert(!c.get());
}

void Test_init() {
    TestType* p1 = new TestType(13);

    SharedPointer<TestType> s1(p1);
    SharedPointer<TestType> s2;

    assert(s1.get() == p1);
    assert(s2.get() == nullptr);
    assert(*s1 == 13);

    _empty_init();

    std::cout << "Test init : OK" << std::endl;
}

void Test_get() {
    {
        int* p = new int(1);

        SharedPointer<int> s(p);
        SharedPointer<int> const& sc = s;

        assert(s.get() == p);
        assert(sc.get() == s.get());
    }
    {
        const int* p = new int(1);

        SharedPointer<const int> s(p);
        SharedPointer<const int> const& sc = s;

        assert(s.get() == p);
        assert(sc.get() == s.get());
    }
    std::cout << "Test get : OK" << std::endl;
}

void Test_swap() {
    {
        TestType* ptr1 = new TestType(1);
        TestType* ptr2 = new TestType(2);
        SharedPointer<TestType> p1(ptr1);
        {
            SharedPointer<TestType> p2(ptr2);
            p1.swap(p2);
            assert(p1.use_count() == 1);
            assert(p1.get() == ptr2);
            assert(p2.use_count() == 1);
            assert(p2.get() == ptr1);
            assert(TestType::AliveCount() == 2);
        }
        assert(p1.use_count() == 1);
        assert(p1.get() == ptr2);
        assert(TestType::AliveCount() == 1);
    }
    assert(TestType::AliveCount() == 0);
    {
        TestType* ptr1 = new TestType;
        TestType* ptr2 = nullptr;
        SharedPointer<TestType> p1(ptr1);
        {
            SharedPointer<TestType> p2;
            p1.swap(p2); 
            assert(p1.use_count() == 0);
            assert(p1.get() == ptr2);
            assert(p2.use_count() == 1);
            assert(p2.get() == ptr1);
            assert(TestType::AliveCount() == 1);
        }
        assert(p1.use_count() == 0);
        assert(p1.get() == ptr2);
        assert(TestType::AliveCount() == 0);
    }
    assert(TestType::AliveCount() == 0);
    {
        TestType* ptr1 = nullptr;
        TestType* ptr2 = new TestType;
        SharedPointer<TestType> p1;
        {
            SharedPointer<TestType> p2(ptr2);
            p1.swap(p2); 
            assert(p1.use_count() == 1);
            assert(p1.get() == ptr2);
            assert(p2.use_count() == 0);
            assert(p2.get() == ptr1);
            assert(TestType::AliveCount() == 1);
        }
        assert(p1.use_count() == 1);
        assert(p1.get() == ptr2);
        assert(TestType::AliveCount() == 1);
    }
    assert(TestType::AliveCount() == 0);
    {
        TestType* ptr1 = nullptr;
        TestType* ptr2 = nullptr;
        SharedPointer<TestType> p1;
        {
            SharedPointer<TestType> p2;
            p1.swap(p2); 
            assert(p1.use_count() == 0);
            assert(p1.get() == ptr2);
            assert(p2.use_count() == 0);
            assert(p2.get() == ptr1);
            assert(TestType::AliveCount() == 0);
        }
        assert(p1.use_count() == 0);
        assert(p1.get() == ptr2);
        assert(TestType::AliveCount() == 0);
    }
    assert(TestType::AliveCount() == 0);
    std::cout << "Test swap : OK" << std::endl;
}

void _move_inctor() {
    SharedPointer<TestType> s1(new TestType(42));
    TestType* p1;
    {
        SharedPointer<TestType> s2(s1);
        SharedPointer<TestType> s3(s1);
        p1 = s3.get();

        assert(TestType::AliveCount() == 1);
        assert(s3.use_count() == 3);
    }

    assert(s1.get() == p1);
    assert(*p1 == 42);
    assert(TestType::AliveCount() == 1);
    std::cout << "\tmove in consructor : OK" << std::endl;
}

void _simple_move() {
    SharedPointer<TestType> s1(new TestType(42));
    SharedPointer<TestType> s2(new TestType(13));
    {
        SharedPointer<TestType> s3(s2);
        SharedPointer<TestType> s4(s2);
        assert(s4.use_count() == 3);

        s4 = std::move(s1);
        assert(s1.use_count() == 0);
        assert(s4.use_count() == 1);
        assert(s2.use_count() == 2);
        assert(*s3 == 13);
        assert(*s4 == 42);
    }
    assert(s2.use_count() == 1);
    assert(*s2 == 13);
    assert(s1.use_count() == 0);
    assert(TestType::AliveCount() == 1);
    std::cout << "\tstd::move : OK" << std::endl;
}

void _self_move() {
    SharedPointer<TestType> s2(new TestType(42));
    SharedPointer<TestType> s;
    {
        SharedPointer<TestType> s4(new TestType(4));
        assert(TestType::AliveCount() == 2);
        s4 = s2;
        SharedPointer<TestType> s3(std::move(s2));
        assert(*s4 == 42);
        assert(*s3 == 42);
        assert(TestType::AliveCount() == 1);

        assert(s4.use_count() == 2);
        s4 = s4;
        assert(s4.use_count() == 2);
        
        s3 = s;
        s4.reset(new TestType(8));
        s = s4;
    }
    assert(*s == 8);
    std::cout << "\tself move : OK" << std::endl;
}

void Test_move() {
    std::cout << "Test move :" << std::endl;
    _simple_move();
    _move_inctor();
    _self_move();
}

void _simple_reset() {
    SharedPointer<TestType> s(new TestType);

    assert(TestType::AliveCount() == 1);
    
    // no arg = null
    s.reset();

    assert(s.get() == nullptr);
    assert(TestType::AliveCount() == 0);
    assert(s.use_count() == 0);

    TestType* p = new TestType;
    s.reset(p);

    assert(s.get() == p);
    assert(TestType::AliveCount() == 1);
    assert(s.use_count() == 1);
}

void _reset() {
    
    SharedPointer<TestType> s(new TestType);

    assert(TestType::AliveCount() == 1);

    TestType* p = new TestType;
    assert(TestType::AliveCount() == 2);

    s.reset(p);

    assert(s.get() == p);
    assert(TestType::AliveCount() == 1);
    assert(s.use_count() == 1);
}

void _self_reset() {
    
    SharedPointer<TestType> s(new TestType);
    TestType* p = s.get();

    assert(TestType::AliveCount() == 1);

    s.reset(p);

    assert(s.get() == p);
    assert(TestType::AliveCount() == 1);
}

void Test_reset() {
    _reset();
    _simple_reset();
    _self_reset();
    std::cout << "Test reset : OK" << std::endl;
}

void Test_operator_bool() {
    SharedPointer<int> p(new int(1));
    SharedPointer<int> const& cp = p;

    assert(p);
    assert(cp);

    SharedPointer<int> p0;
    SharedPointer<int> const& cp0 = p0;

    assert(!p0);
    assert(!cp0);

    std::cout << "Test bool operator : OK" << std::endl;
}

void Test_operator_arrow() {
    struct A {
        int data{42};
    };

    SharedPointer<A> p(new A);
    assert(p->data == 42);
    
    p->data = 13;
    assert(p->data == 13);

    std::cout << "Test -> operator : OK" << std::endl;
}

void Test_dereference() {
    const SharedPointer<int> p(new int(42));
    assert(*p == 42);
    *p = 8;
    assert(*p == 8);
    std::cout << "Test * operator : OK" << std::endl;
}

// for arrays
void Test_delete() {
    SharedPointer<TestType[]> s(new TestType[300]);
    assert(TestType::AliveCount() == 300);
    
    s.reset();
    assert(TestType::AliveCount() == 0);

    std::cout << "Test reset & delete : OK" << std::endl;
}

void Test_branch_operator() {
    int size = 7;
    int* array = new int[size]{1, 2, 3, 5, 8, 13, 21};
    SharedPointer<int[]> s(array);
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
    Test_reset();
    Test_operator_bool();
    Test_operator_arrow();
    Test_dereference();

    std::cout << "Test array specialization." << std::endl;
    Test_delete();
    Test_branch_operator();

    std::cout << "PASSED" << std::endl;
}