#include <thread>
#include <vector>

#include "skiplist.hpp"

#define NUM_THREADS 100
#define TEST_COUNT 10000
SkipList<int, std::string> skipList(18, 1000);

void insertElement(const int tid) {
    std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        skipList.insert_element(rand() % TEST_COUNT, "a");
    }
}

void getElement(int tid) {
    std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        skipList.search_element(rand() % TEST_COUNT);
    }
}

int main() {
    srand(time(nullptr));

    {
        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            threads.emplace_back(insertElement, i);
        }

        for (auto &th : threads) {
            th.join();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "insert elapsed:" << elapsed.count() << std::endl;
    }

    {
        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            threads.emplace_back(getElement, i);
        }

        for (auto &th : threads) {
            th.join();
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "get elapsed:" << elapsed.count() << std::endl;
    }

    return 0;
}
