#include <gtest/gtest.h>

#include <string>

#include "skiplist.hpp"

#define TEST_COUNT 10000

TEST(Stress, testcase) {
    SkipList<int, std::string> skipList(18);  // 创建一个最大层级为18的跳表实例
    for (int count = 0; count < TEST_COUNT; count++) {
        skipList.insert_element(rand() % TEST_COUNT,
                                "a");  // 随机生成一个键，并插入带有"a"的元素
    }
    for (int count = 0; count < TEST_COUNT; count++) {
        skipList.search_element(rand() % TEST_COUNT);  // 随机查找一个键
    }
}