#pragma once
#include <bits/stdc++.h>

using namespace std;

class Counter {
    long mCount;
    std::mutex mMutex;
public:
    Counter() : mCount(0), mMutex() {}

    void C_increment() {
        std::lock_guard < std::mutex > guard(mMutex);
        mCount++;
    }

    void C_decrement() {
        std::lock_guard < std::mutex > guard(mMutex);
        mCount--;
    }

    friend void increment(Counter* counter);
    friend void decrement(Counter* counter);

    long count() { return mCount; }

};