#include <thread>
#include <atomic>
#include <iostream>

struct PetersonMutex {
    std::atomic<bool> want[2] {};
    std::atomic<int> victim {-1};
    int owner {-1};

    void lock(int i) {
       want[i].store(true);
       victim.store(i);
       while (want[1 - i].load() && i == victim.load());
       owner = i;
    }

    void unlock() {
       want[owner].store(false);
    }
};

PetersonMutex m;
int glob = 0;

void foo(int i) {
    m.lock(i);
    ++glob;
    m.unlock();
}

int main() {
    std::thread t1([](){
        for (int i=0; i<1'000'000; ++i) {
            foo(0);
       }
    });

    std::thread t2([](){
       for (int i=0; i<1'000'000; ++i) {
            foo(1);
       }
    });

    t1.join();
    t2.join();
    std::cout << glob << std::endl;
}