#include "skiplist/skiplist.hpp"

#include <chrono>
#include <thread>
#include <vector>

#define NUM_THREADS 100
#define TEST_COUNT 100000
SkipList<int, std::string> skipList(18, 1000);

void insertElement(const int tid) {
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        skipList.insertElement(rand() % TEST_COUNT, "a");
    }
}

void getElement(int tid) {
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        skipList.searchElement(rand() % TEST_COUNT);
    }
}

int main() {
    srand(time(nullptr));
    std::vector<std::thread> threads;

    // 随机读写性能
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(insertElement, i);
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(getElement, i);
    }
    for (auto &th : threads) {
        th.join();
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "get elapsed:" << elapsed.count() << std::endl;

    return 0;
}
