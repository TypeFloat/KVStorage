#include "data_dump.hpp"

#include <gtest/gtest.h>

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
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}