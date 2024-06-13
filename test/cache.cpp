#include "lru_cache.hpp"
#include <gtest/gtest.h>

TEST(Cache, Test1) {
    LRUCache<int, int> cache = LRUCache<int, int>(3);
    cache.set(1, 2);
    cache.set(2, 3);
    cache.set(3, 4);
    ASSERT_EQ(cache.get(1), 2);
    cache.set(4, 5);
    ASSERT_EQ(cache.contains(2), false);
}