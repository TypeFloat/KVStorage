#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <utility>
#include <vector>

template <typename K, typename V>
class DataDumper {
   public:
    std::vector<K> keys;
    std::vector<V> values;
    DataDumper<K, V>() = default;

    // FIXME
    void dump(const std::string& filename) {
        std::ofstream ofs(filename);
        boost::archive::text_oarchive oa(ofs);
        oa << *this;
    }

    // FIXME
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

template class DataDumper<int, int>;