#ifndef SOIL_UTIL_CACHE_HPP
#define SOIL_UTIL_CACHE_HPP
#include <string>
#include <unordered_map>
namespace soil::util {
template <class T>
class Cache {
 public:
  Cache() = default;
  Cache(Cache const&) = delete;
  void operator=(Cache const&) = delete;
  virtual ~Cache() = default;

  T* Get(std::string name) const {
    auto itr = content_.find(name);
    if (itr == content_.end()) {
      return nullptr;
    }
    return itr->second;
  }
  void Put(std::string name, T* value) { content_.insert({name, value}); }
  bool Empty() { return content_.empty(); }
  size_t Size() { return content_.size(); }

 private:
  std::unordered_map<std::string, T*> content_;
};
}  // namespace soil::util
#endif
