#include "skiplist/skiplist.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

TEST(Node, TestCase) {
    Node<int, int> node(1, 2, 3);
    ASSERT_EQ(node.getKey(), 1);
    ASSERT_EQ(node.getValue(), 2);
}

TEST(Cache, TestCase) {
    LRUCache<int, int> cache = LRUCache<int, int>(3);
    cache.set(1, 2);
    cache.set(2, 3);
    cache.set(3, 4);
    ASSERT_EQ(cache.get(1), 2);
    cache.set(4, 5);
    ASSERT_EQ(cache.get(2).has_value(), false);
}

TEST(DataDumper, Dump) {
    DataDumper<int, std::string> data;
    data.keys = {1, 2, 3};
    data.values = {"one", "two", "three"};

    // 保存到文件
    data.dump("data.txt");
}

TEST(DataDumper, Load) {
    DataDumper<int, std::string> loaded_data;
    std::vector<std::pair<int, std::string>> loaded_pairs =
        loaded_data.load("data.txt");

    // 打印加载的数据
    for (const auto& pair : loaded_pairs) {
        std::cout << pair.first << ":" << pair.second << std::endl;
    }
}

TEST(SkipList, Insert) {
    SkipList<int, int> skip_list(3, 10);
    std::vector<std::vector<int>> kv = {{1, 3}, {2, 4}, {3, 5}, {3, 3}};
    std::vector<int> kv_status = {0, 0, 0, 1};
    int status;
    for (int i = 0; i < 4; ++i) {
        status = skip_list.insertElement(kv[i][0], kv[i][1]);
        ASSERT_EQ(status, kv_status[i]);
    }

    std::vector<int> k = {1, 2, 4, 3};
    std::vector<bool> k_ans = {true, true, false, true};
    for (int i = 0; i < 4; ++i) {
        status = skip_list.searchElement(k[i]);
        ASSERT_EQ(status, k_ans[i]);
    }
}

TEST(SkipList, Delete) {
    SkipList<int, int> skip_list(3, 10);
    std::vector<std::vector<int>> kv = {{1, 100}, {2, 200}, {3, 300}};
    for (int i = 0; i < 3; ++i) {
        skip_list.insertElement(kv[i][0], kv[i][1]);
    }
    std::vector<int> delete_key = {2, 3};
    for (int i = 0; i < 2; ++i) {
        skip_list.deleteElement(delete_key[i]);
    }
    std::vector<int> search_key = {1, 3, 10};
    std::vector<bool> status = {true, false};
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(skip_list.searchElement(search_key[i]), status[i]);
    }
}

TEST(SkipList, Dump) {
    SkipList<int, int> skip_list(3, 10);
    for (int i = 0; i < 50; ++i) {
        skip_list.insertElement(i, i * 10);
    }
    skip_list.displayList();
    skip_list.dumpFile();
}

TEST(SkipList, Load) {
    SkipList<int, int> skip_list(3, 10);
    skip_list.loadFile();
    skip_list.displayList();
}

TEST(SkipList, Stress) {
    SkipList<int, std::string> skipList(18, 100);
    int test_count = 10000;
    for (int count = 0; count < test_count; count++) {
        skipList.insertElement(rand() % test_count, "a");
    }
    for (int count = 0; count < test_count; count++) {
        skipList.searchElement(rand() % test_count);
    }
}