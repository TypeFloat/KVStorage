#include "node.hpp"

#include <gtest/gtest.h>

TEST(NodeTest, test1) {
    Node<int, int> node(1, 2, 3);
    ASSERT_EQ(node.get_key(), 1);
    ASSERT_EQ(node.get_value(), 2);
}