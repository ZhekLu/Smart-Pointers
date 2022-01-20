#pragma once

class TestType {
public:
    static int AliveCount() {
        return count_alive;
    }

    TestType() {
        this->value = 0;
        ++count_alive;
    }

    TestType(int value) {
        this->value = value;
        ++count_alive;
    };

    TestType(const TestType& other) : value(other.value) {
        ++count_alive;
    }

    TestType(TestType&&) = delete;

    TestType& operator=(const TestType& other) {
        this->value = other.value;
        ++count_alive;
        return *this;
    }

    TestType& operator=(TestType&& other) = delete;

    ~TestType() {
        --count_alive;
    }

    bool operator==(int other) const {
        return value == other;
    }

private:
    int value;
    static int count_alive;
};

int TestType::count_alive = 0;