#pragma once

#include <optional>
#include <unordered_map>

#include "lru_cache.hpp"
#include "node.hpp"

template <typename K, typename V>
struct LinkNode {
    std::shared_ptr<LinkNode> prev;
    std::shared_ptr<LinkNode> next;
    K key;
    V value;
    LinkNode() = default;
    LinkNode(K key, V value) : key(key), value(value) {
        this->prev = nullptr;
        this->next = nullptr;
    }
};

template <typename K, typename V>
class LRUCache {
   private:
    int capacity{};
    int size{};
    std::shared_ptr<LinkNode<K, V>> head;
    std::shared_ptr<LinkNode<K, V>> tail;
    std::unordered_map<K, std::shared_ptr<LinkNode<K, V>>> cache;
    void move_to_head(std::shared_ptr<LinkNode<K, V>>);

   public:
    LRUCache() = delete;
    explicit LRUCache(int);
    void set(K, V);
    std::optional<V> get(K);
    void delete_element(K);
};

template <typename K, typename V>
LRUCache<K, V>::LRUCache(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->head = std::make_shared<LinkNode<K, V>>();
    this->tail = std::make_shared<LinkNode<K, V>>();
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <typename K, typename V>
void LRUCache<K, V>::set(K key, V value) {
    if (this->cache.find(key) != this->cache.end()) return;
    std::shared_ptr<LinkNode<K, V>> link_node =
        std::make_shared<LinkNode<K, V>>(LinkNode<K, V>(key, value));
    this->move_to_head(link_node);
    this->cache[key] = link_node;
    ++(this->size);
    if (this->size > this->capacity) delete_element(this->tail->prev->key);
}

template <typename K, typename V>
std::optional<V> LRUCache<K, V>::get(K key) {
    if (this->cache.find(key) == this->cache.end()) return std::nullopt;
    std::shared_ptr<LinkNode<K, V>> link_node = this->cache[key];
    link_node->next->prev = link_node->prev;
    link_node->prev->next = link_node->next;
    move_to_head(link_node);
    return link_node->value;
}

template <typename K, typename V>
void LRUCache<K, V>::move_to_head(std::shared_ptr<LinkNode<K, V>> link_node) {
    link_node->prev = this->head;
    link_node->next = this->head->next;
    link_node->next->prev = link_node;
    this->head->next = link_node;
}

template <typename K, typename V>
void LRUCache<K, V>::delete_element(K key) {
    if (this->cache.find(key) == this->cache.end()) return;
    std::shared_ptr<LinkNode<K, V>> link_node = this->cache[key];
    link_node->prev->next = link_node->next;
    link_node->next->prev = link_node->prev;
    this->cache.erase(key);
    --(this->size);
}