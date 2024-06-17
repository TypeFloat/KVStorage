#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

// SkipList的前向声明，具体声明在skiplist.hpp中
template <typename K, typename V>
class SkipList;

/**
 * @brief LRU缓存算法的实现，通过模版类支持任意数据类型键值对的保存
 *
 * LRU缓存算法：使用双向链表和哈希表实现，双向链表用于增加和删除数据，哈希表用于快速查找数据，
 * 新的缓存或新查询的数据加到队头，当数据量超过容量后，删除队尾元素。
 *
 * @tparam K, key
 * @tparam V, value
 */
template <typename K, typename V>
struct LinkNode {
    // NOTE: 使用弱引用，防止循环引用导致内存泄漏
    std::weak_ptr<LinkNode> prev;
    std::weak_ptr<LinkNode> next;
    K key;
    V value;
    LinkNode() = default;
    LinkNode(K key, V value) : key(key), value(value) {}
};

template <typename K, typename V>
class LRUCache {
   private:
    int capacity{};
    int size{};
    std::shared_ptr<LinkNode<K, V>> head;
    std::shared_ptr<LinkNode<K, V>> tail;
    std::unordered_map<K, std::shared_ptr<LinkNode<K, V>>> cache;
    void moveToHead(std::shared_ptr<LinkNode<K, V>>);
    void deleteElement(K);

   public:
    LRUCache() = delete;
    explicit LRUCache(int);
    void set(K, V);
    std::optional<V> get(K);
    friend void SkipList<K, V>::deleteElement(K);
};

/**
 * @brief Construct a new LRUCache<K, V>::LRUCache object
 *
 * @tparam K
 * @tparam V
 * @param capacity 队列容量
 */
template <typename K, typename V>
LRUCache<K, V>::LRUCache(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->head = std::make_shared<LinkNode<K, V>>();
    this->tail = std::make_shared<LinkNode<K, V>>();
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

/**
 * @brief 添加新的数据到队列中，新数据会插入到队头
 *
 * @tparam K
 * @tparam V
 * @param key
 * @param value
 */
template <typename K, typename V>
void LRUCache<K, V>::set(K key, V value) {
    if (this->cache.find(key) != this->cache.end()) return;
    std::shared_ptr<LinkNode<K, V>> linkNode =
        std::make_shared<LinkNode<K, V>>(LinkNode<K, V>(key, value));
    this->moveToHead(linkNode);
    this->cache[key] = linkNode;
    ++(this->size);
    std::shared_ptr<LinkNode<K, V>> preNode = this->tail->prev.lock();
    if (this->size > this->capacity) deleteElement(preNode->key);
}

/**
 * @brief 查询数据，如果查询到数据，则会将查询的数据添加到队头，并返回查询结果；
 * 如果查询失败，则返回空值，通过optional实现
 *
 * @tparam K
 * @tparam V
 * @param key
 * @return std::optional<V>
 */
template <typename K, typename V>
std::optional<V> LRUCache<K, V>::get(K key) {
    if (this->cache.find(key) == this->cache.end()) return std::nullopt;
    std::shared_ptr<LinkNode<K, V>> linkNode = this->cache[key];
    linkNode->prev.lock()->next = linkNode->next;
    linkNode->next.lock()->prev = linkNode->prev;
    moveToHead(linkNode);
    return linkNode->value;
}

template <typename K, typename V>
void LRUCache<K, V>::moveToHead(std::shared_ptr<LinkNode<K, V>> linkNode) {
    linkNode->prev = this->head;
    linkNode->next = this->head->next;
    linkNode->next.lock()->prev = linkNode;
    this->head->next = linkNode;
}

/**
 * @brief 从队列中移除某个键值对，当队列元素超过队列容量后进行调用；
 * 另外，将SkipList的delete_element方法设为友元函数，在SkipList中删除元素的时候也进行调用
 * @tparam K
 * @tparam V
 * @param key 要删除的键
 */
template <typename K, typename V>
void LRUCache<K, V>::deleteElement(K key) {
    if (this->cache.find(key) == this->cache.end()) return;
    std::shared_ptr<LinkNode<K, V>> linkNode = this->cache[key];
    linkNode->prev.lock()->next = linkNode->next;
    linkNode->next.lock()->prev = linkNode->prev;
    this->cache.erase(key);
    --(this->size);
}