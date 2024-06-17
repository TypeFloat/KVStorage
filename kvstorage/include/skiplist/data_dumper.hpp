#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <utility>
#include <vector>

/**
 * @brief
 * 数据持久化保存和加载，利用Boost的序列化功能，使用模版类实现，支持多种键值数据类型
 * 需要将要保存的数据的键和值分别添加到vector中进行保存，加载同理
 *
 * @tparam K, key
 * @tparam V, value
 */
template <typename K, typename V>
class DataDumper {
   public:
    // TODO 使用 std::vector<std::pair<K, V>> 进行存储
    std::vector<K> keys;
    std::vector<V> values;
    DataDumper<K, V>() = default;

    /**
     * @brief 数据的持久化保存
     *
     * @param filename 文件路径
     */
    void dump(const std::string& filename) {
        std::ofstream ofs(filename);
        boost::archive::text_oarchive oa(ofs);
        oa << *this;
    }

    /**
     * @brief 数据的持久化加载
     *
     * @param filename 文件路径
     * @return std::vector<std::pair<K, V>>
     */
    std::vector<std::pair<K, V>> load(const std::string& filename) {
        std::vector<std::pair<K, V>> result;
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> *this;
        for (size_t i = 0; i < this->keys.size(); i++) {
            result.emplace_back(this->keys[i], this->values[i]);
        }
        return result;
    }

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& this->keys;
        ar& this->values;
    }
};