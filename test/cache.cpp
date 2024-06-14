#include <gtest/gtest.h>

#include "lru_cache.hpp"

TEST(Cache, Test1) {
    LRUCache<int, int> cache = LRUCache<int, int>(3);
    cache.set(1, 2);
    cache.set(2, 3);
    cache.set(3, 4);
    ASSERT_EQ(cache.get(1), 2);
    cache.set(4, 5);
    ASSERT_EQ(cache.get(2).has_value(), false);
}