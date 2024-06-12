#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>

#include "node.hpp"
#include "utils.hpp"

#define DELIMITER ":"
#define DUMP_FILE "dump_file.txt"

template <typename K, typename V>
class SkipList {
   private:
    int max_level;
    int skip_list_level;
    std::shared_ptr<Node<K, V>> header;
    int element_count;
    std::ofstream file_writer;
    std::ifstream file_reader;
    bool is_valid_string(const std::string &);
    std::pair<std::string, std::string> get_key_value_from_string(
        const std::string &);
    std::shared_mutex mutex;

   public:
    SkipList(int);
    ~SkipList() = default;
    int get_random_level();
    std::shared_ptr<Node<K, V>> create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    void clear(std::shared_ptr<Node<K, V>>);
    int size();
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
    this->max_level = max_level;
    this->skip_list_level = 0;
    this->element_count = 0;
    K k;
    V v;
    this->header = std::make_shared<Node<K, V>>(k, v, max_level);
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
    std::shared_ptr<Node<K, V>> current = this->header;
    for (int i = this->skip_list_level; i >= 0; i--) {
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
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->max_level + 1,
                                                    nullptr);
    for (int i = this->skip_list_level; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    bool status = current != nullptr && current->get_key() == key;
    // 如果已经有要插入的键值对，返回1
    if (status) {
        this->mutex.unlock();
        return 1;
    }

    // 否则，创建一个新节点
    int random_level = this->get_random_level();
    // 如果随机层数大于当前跳表的层数，要建立高层索引
    if (random_level > this->skip_list_level) {
        for (int i = this->skip_list_level + 1; i < random_level + 1; i++) {
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
    this->element_count++;
    this->mutex.unlock();
    return 0;
}

template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    this->mutex.lock();
    std::shared_ptr<Node<K, V>> current = this->header;
    std::vector<std::shared_ptr<Node<K, V>>> update(this->max_level + 1,
                                                    nullptr);
    for (int i = this->skip_list_level; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    bool status = current != nullptr && current->get_key() == key;
    if (status) {
        for (int i = 0; i <= this->skip_list_level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        while (this->skip_list_level > 0 &&
               this->header->forward[this->skip_list_level] == nullptr) {
            this->skip_list_level--;
        }
        this->element_count--;
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
            std::cout << current->get_key() << DELIMITER << current->get_value()
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
    this->file_writer.open(DUMP_FILE);
    std::shared_ptr<Node<K, V>> current;
    for (int i = this->skip_list_level; i >= 0; --i) {
        current = this->header->forward[i];
        while (current != nullptr) {
            this->file_writer << current->get_key() << DELIMITER
                              << current->get_value() << "\n";
            current = current->forward[i];
        }
    }
    this->file_writer.close();
    this->mutex.unlock_shared();
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string &s) {
    return !s.empty() && s.find(DELIMITER) != std::string::npos;
}

template <typename K, typename V>
std::pair<std::string, std::string> SkipList<K, V>::get_key_value_from_string(
    const std::string &s) {
    std::string key = s.substr(0, s.find(DELIMITER));
    std::string value = s.substr(s.find(DELIMITER) + 1);
    return std::make_pair(key, value);
}

template <typename K, typename V>
void SkipList<K, V>::load_file() {
    this->mutex.lock();
    this->file_reader.open(DUMP_FILE);
    std::string line;
    K k;
    V v;
    while (std::getline(this->file_reader, line)) {
        if (this->is_valid_string(line)) {
            std::pair<std::string, std::string> kv =
                this->get_key_value_from_string(line);
            k = stoT<K>(kv.first);
            v = stoT<V>(kv.second);
            this->insert_element(k, v);
        }
    }
    this->file_reader.close();
    this->mutex.unlock();
}