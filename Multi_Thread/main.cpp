#include <bits\stdc++.h>
#include "Multi_Threading.h"
#include "Counter.h"

using namespace std;

//int main() {
//
//    Producer prod;
//    Consumer cons;
//
//    cons.start(); //消费者先开始，buffer内没东西
//    prod.start(); //生产者再开始，往buffer内加东西
//
//    //如果不join将什么都不会发生，因为消费者和生产者随main线程一起结束
//    prod.join();
//    cons.join(); //join函数将等待任务做完再返回，此时buffer内已有东西，将其输出
//
//    return 0;
//}

Counter counter;
void increment(Counter* counter);
void decrement(Counter* counter);

int main(int argc, char* argv[]) {

    std::thread producer(increment, &counter);
    std::thread consumer(decrement, &counter);

    std::cout << " Launch " << std::endl;
    std::chrono::milliseconds duration(1000);
    std::this_thread::sleep_for(duration);

    producer.join();
    consumer.join();

    if (counter.count() == 0) {
        std::cout << " race condition : " << counter.count() << std::endl;
    }

    return 0;
}