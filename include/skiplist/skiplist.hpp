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
#include "node.hpp"

#define DUMP_FILE "dump_file.txt"

template <typename K, typename V>
class SkipList {
   private:
    int max_level{};
    int skip_list_level{};
    int element_count{};
    std::shared_ptr<LRUCache<K, V>> lru_cache;
    std::shared_ptr<Node<K, V>> header;
    std::shared_mutex mutex;
    std::shared_ptr<Node<K, V>> create_node(K, V, int);

   public:
    SkipList(int, int);
    ~SkipList() = default;
    int get_random_level();
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level, int capacity) {
    this->max_level = max_level;
    this->skip_list_level = 0;
    this->element_count = 0;
    K k;
    V v;
    this->header = std::make_shared<Node<K, V>>(k, v, max_level);
    this->lru_cache = std::make_shared<LRUCache<K, V>>(capacity);
}

template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int level = 0;
    while (rand() % 2) {
        level++;
    }
    return std::min(level, this->max_level - 1);
}

template <typename K, typename V>
std::shared_ptr<Node<K, V>> SkipList<K, V>::create_node(K key, V value,
                                                        int level) {
    return std::make_shared<Node<K, V>>(key, value, level);
}

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    this->mutex.lock_shared();
    auto value = this->lru_cache->get(key);
    if (value.has_value()) {
        this->mutex.unlock_shared();
        return true;
    }
    std::shared_ptr<Node<K, V>> current = this->header;
    for (int i = this->skip_list_level; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    bool status = current != nullptr && current->get_key() == key;
    this->mutex.unlock_shared();
    return status;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value) {
    this->mutex.lock();
    // 缓存中存在该键，返回1
    if (this->lru_cache->get(key).has_value()) {
        this->mutex.unlock();
        return 1;
    }
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->max_level + 1,
                                                    nullptr);
    for (int i = this->skip_list_level; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    // 如果已经有要插入的键值对，返回1
    if (current != nullptr && current->get_key() == key) {
        this->mutex.unlock();
        return 1;
    }

    // 否则，创建一个新节点
    int random_level = this->get_random_level();
    // 如果随机层数大于当前跳表的层数，要建立高层索引
    if (random_level > this->skip_list_level) {
        for (int i = this->skip_list_level + 1; i < random_level + 1; ++i) {
            update[i] = this->header;
        }
        this->skip_list_level = random_level;
    }

    std::shared_ptr<Node<K, V>> new_node =
        this->create_node(key, value, random_level);
    for (int i = 0; i <= random_level; ++i) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
    ++(this->element_count);
    this->lru_cache->set(key, value);
    this->mutex.unlock();
    return 0;
}

template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    this->mutex.lock();
    this->lru_cache->delete_element(key);
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->max_level + 1,
                                                    nullptr);
    for (int i = this->skip_list_level; i >= 0; --i) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current != nullptr && current->get_key() == key) {
        for (int i = 0; i <= this->skip_list_level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        while (this->skip_list_level > 0 &&
               this->header->forward[this->skip_list_level] == nullptr) {
            --(this->skip_list_level);
        }
        --(this->element_count);
    }
    this->mutex.unlock();
}

template <typename K, typename V>
void SkipList<K, V>::display_list() {
    this->mutex.lock_shared();
    std::shared_ptr<Node<K, V>> current;
    for (int i = this->skip_list_level; i >= 0; --i) {
        current = this->header->forward[i];
        while (current != nullptr) {
            std::cout << current->get_key() << ":" << current->get_value()
                      << " ";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }
    this->mutex.unlock_shared();
}

template <typename K, typename V>
void SkipList<K, V>::dump_file() {
    this->mutex.lock_shared();
    std::shared_ptr<Node<K, V>> current;
    DataDumper<K, V> data_dumper;
    for (int i = this->skip_list_level; i >= 0; --i) {
        current = this->header->forward[i];
        while (current != nullptr) {
            data_dumper.keys.emplace_back(current->get_key());
            data_dumper.values.emplace_back(current->get_value());
            current = current->forward[i];
        }
    }
    data_dumper.dump(DUMP_FILE);
    this->mutex.unlock_shared();
}

template <typename K, typename V>
void SkipList<K, V>::load_file() {
    DataDumper<K, V> data_dumper;
    std::vector<std::pair<K, V>> data = data_dumper.load(DUMP_FILE);
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        this->insert_element(iter->first, iter->second);
    }
}