#pragma once

#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <utility>
#include <vector>

#include "data_dumper.hpp"
#include "lru_cache.hpp"

#define DUMP_FILE "dump_file.txt"

/**
 * @brief 跳表中的数据节点
 *
 * @tparam K
 * @tparam V
 */
template <typename K, typename V>
class Node {
   public:
    Node() = default;
    Node(K, V, int);
    ~Node() = default;
    K getKey() const;
    V getValue() const;
    void setValue(V);
    // NOTE：forward数组存储了每一层的下一个节点
    std::vector<std::shared_ptr<Node<K, V>>> forward;

   private:
    K key;
    V value;
    int nodeLevel{};
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->nodeLevel = level;
    this->forward =
        std::vector<std::shared_ptr<Node<K, V>>>(level + 1, nullptr);
};

template <typename K, typename V>
K Node<K, V>::getKey() const {
    return key;
};

template <typename K, typename V>
V Node<K, V>::getValue() const {
    return value;
};

template <typename K, typename V>
void Node<K, V>::setValue(V value) {
    this->value = value;
};

// TODO 添加备注
template <typename K, typename V>
class SkipList {
   private:
    int maxLevel{};
    int skipListLevel{};
    int elementCount{};
    std::shared_ptr<LRUCache<K, V>> lruCache;
    std::shared_ptr<Node<K, V>> header;
    std::shared_mutex mutex;
    std::shared_ptr<Node<K, V>> createNode(K, V, int);

   public:
    SkipList(int, int);
    ~SkipList() = default;
    int getRandomLevel();
    int insertElement(K, V);
    void displayList();
    bool searchElement(K);
    void deleteElement(K);
    void dumpFile();
    void loadFile();
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int maxLevel, int capacity) {
    this->maxLevel = maxLevel;
    this->skipListLevel = 0;
    this->elementCount = 0;
    K k;
    V v;
    this->header = std::make_shared<Node<K, V>>(k, v, maxLevel);
    this->lruCache = std::make_shared<LRUCache<K, V>>(capacity);
}

template <typename K, typename V>
int SkipList<K, V>::getRandomLevel() {
    int level = 0;
    while (rand() % 2) {
        level++;
    }
    return std::min(level, this->maxLevel - 1);
}

template <typename K, typename V>
std::shared_ptr<Node<K, V>> SkipList<K, V>::createNode(K key, V value,
                                                       int level) {
    return std::make_shared<Node<K, V>>(key, value, level);
}

template <typename K, typename V>
bool SkipList<K, V>::searchElement(K key) {
    this->mutex.lock_shared();
    auto value = this->lruCache->get(key);
    if (value.has_value()) {
        this->mutex.unlock_shared();
        return true;
    }
    std::shared_ptr<Node<K, V>> current = this->header;
    for (int i = this->skipListLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    bool status = current != nullptr && current->getKey() == key;
    this->mutex.unlock_shared();
    return status;
}

template <typename K, typename V>
int SkipList<K, V>::insertElement(K key, V value) {
    this->mutex.lock();
    // 缓存中存在该键，返回1
    if (this->lruCache->get(key).has_value()) {
        this->mutex.unlock();
        return 1;
    }
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->maxLevel + 1,
                                                    nullptr);
    for (int i = this->skipListLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    // 如果已经有要插入的键值对，返回1
    if (current != nullptr && current->getKey() == key) {
        this->mutex.unlock();
        return 1;
    }

    // 否则，创建一个新节点
    int randomLevel = this->getRandomLevel();
    // 如果随机层数大于当前跳表的层数，要建立高层索引
    if (randomLevel > this->skipListLevel) {
        for (int i = this->skipListLevel + 1; i < randomLevel + 1; ++i) {
            update[i] = this->header;
        }
        this->skipListLevel = randomLevel;
    }

    std::shared_ptr<Node<K, V>> newNode =
        this->createNode(key, value, randomLevel);
    for (int i = 0; i <= randomLevel; ++i) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    ++(this->elementCount);
    this->lruCache->set(key, value);
    this->mutex.unlock();
    return 0;
}

template <typename K, typename V>
void SkipList<K, V>::deleteElement(K key) {
    this->mutex.lock();
    this->lruCache->deleteElement(key);
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->maxLevel + 1,
                                                    nullptr);
    for (int i = this->skipListLevel; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->getKey() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current != nullptr && current->getKey() == key) {
        for (int i = 0; i <= this->skipListLevel; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        while (this->skipListLevel > 0 &&
               this->header->forward[this->skipListLevel] == nullptr) {
            --(this->skipListLevel);
        }
        --(this->elementCount);
    }
    this->mutex.unlock();
}

template <typename K, typename V>
void SkipList<K, V>::displayList() {
    this->mutex.lock_shared();
    std::shared_ptr<Node<K, V>> current;
    for (int i = this->skipListLevel; i >= 0; --i) {
        current = this->header->forward[i];
        while (current != nullptr) {
            std::cout << current->getKey() << ":" << current->getValue() << " ";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }
    this->mutex.unlock_shared();
}

template <typename K, typename V>
void SkipList<K, V>::dumpFile() {
    this->mutex.lock_shared();
    std::shared_ptr<Node<K, V>> current;
    DataDumper<K, V> data_dumper;
    for (int i = this->skipListLevel; i >= 0; --i) {
        current = this->header->forward[i];
        while (current != nullptr) {
            data_dumper.keys.emplace_back(current->getKey());
            data_dumper.values.emplace_back(current->getValue());
            current = current->forward[i];
        }
    }
    data_dumper.dump(DUMP_FILE);
    this->mutex.unlock_shared();
}

template <typename K, typename V>
void SkipList<K, V>::loadFile() {
    DataDumper<K, V> data_dumper;
    std::vector<std::pair<K, V>> data = data_dumper.load(DUMP_FILE);
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        this->insertElement(iter->first, iter->second);
    }
}