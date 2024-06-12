#pragma once
#include <memory>
#include <vector>

template <typename K, typename V>
class Node {
   public:
    Node() = default;
    Node(K, V, int);
    ~Node() = default;
    K get_key() const;
    V get_value() const;
    void set_value(V);
    std::vector<std::shared_ptr<Node<K, V>>> forward;

   private:
    K key;
    V value;
    int node_level;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward =
        std::vector<std::shared_ptr<Node<K, V>>>(level + 1, nullptr);
};

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
};