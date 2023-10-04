#include <bits\stdc++.h>
#include "Multi_Threading.h"
#include "Counter.h"

using namespace std;

//int main() {
//
//    Producer prod;
//    Consumer cons;
//
//    cons.start(); //�������ȿ�ʼ��buffer��û����
//    prod.start(); //�������ٿ�ʼ����buffer�ڼӶ���
//
//    //�����join��ʲô�����ᷢ������Ϊ�����ߺ���������main�߳�һ�����
//    prod.join();
//    cons.join(); //join�������ȴ����������ٷ��أ���ʱbuffer�����ж������������
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