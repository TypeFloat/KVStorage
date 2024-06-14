#include "skiplist.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

TEST(SkipListTest, test1) {
    SkipList<int, int> skip_list(3, 10);
    std::vector<std::vector<int>> kv = {{1, 3}, {2, 4}, {3, 5}, {3, 3}};
    std::vector<int> kv_status = {0, 0, 0, 1};
    int status;
    for (int i = 0; i < 4; ++i) {
        status = skip_list.insert_element(kv[i][0], kv[i][1]);
        ASSERT_EQ(status, kv_status[i]);
    }

    std::vector<int> k = {1, 2, 4, 3};
    std::vector<bool> k_ans = {true, true, false, true};
    for (int i = 0; i < 4; ++i) {
        status = skip_list.search_element(k[i]);
        ASSERT_EQ(status, k_ans[i]);
    }
}

TEST(SkipListTest, test2) {
    SkipList<int, int> skip_list(3, 10);
    std::vector<std::vector<int>> kv = {{1, 100}, {2, 200}, {3, 300}};
    for (int i = 0; i < 3; ++i) {
        skip_list.insert_element(kv[i][0], kv[i][1]);
    }
    std::vector<int> delete_key = {2, 3};
    for (int i = 0; i < 2; ++i) {
        skip_list.delete_element(delete_key[i]);
    }
    std::vector<int> search_key = {1, 3, 10};
    std::vector<bool> status = {true, false};
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(skip_list.search_element(search_key[i]), status[i]);
    }
}

TEST(SkipListTest, test3) {
    SkipList<int, int> skip_list(3, 10);
    for (int i = 0; i < 50; ++i) {
        skip_list.insert_element(i, i * 10);
    }
    skip_list.display_list();
    skip_list.dump_file();
}

TEST(SkipListTest, test4) {
    SkipList<int, int> skip_list(3, 10);
    skip_list.load_file();
    skip_list.display_list();
}