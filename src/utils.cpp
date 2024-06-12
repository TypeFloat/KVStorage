#include "utils.hpp"

template <>
int stoT<int>(const std::string &s) {
    return std::stoi(s);
}

template <>
std::string stoT<std::string>(const std::string &s) {
    return s;
}