#include "Counter.h"

using namespace std;

void increment(Counter* counter) {
    for (int i = 0; i < 10; ++i) {
        cout << i << ". increment, mCount: " << counter->mCount << endl;
        Sleep(1000);
        counter->C_increment();
    }
    cout << "increment done" << endl;
}

void decrement(Counter* counter) {
    for (int i = 0; i < 10; ++i) {
        cout << i << ". decrement, mCount: " << counter->mCount << endl;
        Sleep(2000);
        counter->C_decrement();
    }
    cout << "decrement done" << endl;
}
