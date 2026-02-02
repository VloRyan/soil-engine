#ifndef SOIL_EXAMPLES_COMMON_COMPONENT_PREFAB_HPP
#define SOIL_EXAMPLES_COMMON_COMPONENT_PREFAB_HPP
#include "string"
#include "unordered_map"
template<class T>
class Prefab {
 public:
  Prefab() = default;
  ~Prefab() = default;

  void Insert(const std::string& name, const T& data) {
    data_.insert({name, data});
  }

  const T& Get(const std::string& name) {
    auto itr = data_.find(name);
    if (itr == data_.end()) {
      throw std::runtime_error("no prefab with name '" + name + "' found");
    }
    return itr->second;
  }

  bool Has(const std::string& name) {
    return data_.contains(name);
  }

  void Remove(const std::string& name) {
    data_.erase(name);
  }

 private:
  std::unordered_map<std::string, T> data_;
};
#endif
