#ifndef SOIL_BASE_H_
#define SOIL_BASE_H_

#include <unordered_map>
#include <GL/glcorearb.h>

using ushort = unsigned short;
using uint = unsigned int;
using byte = unsigned char;
using gl_size_t = GLsizeiptr;

struct EnumClassHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

struct PairHash {
    template<typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &pair) const {
        return std::hash<T>()(pair.first) ^ std::hash<U>()(pair.second);
    }
};

template<typename Key>
using HashType = std::conditional_t<std::is_enum_v<Key>, EnumClassHash, std::hash<Key> >;

template<typename Key, typename T>
using HashMap = std::unordered_map<Key, T, HashType<Key> >;

template<typename Key, typename T>
using PairHashMap = std::unordered_map<Key, T, PairHash>;

#endif //SOIL_BASE_H_
