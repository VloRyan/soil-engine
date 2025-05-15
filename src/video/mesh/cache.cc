#include "video/mesh/cache.h"

#include <plog/Log.h>
#include <ranges>

namespace soil::video::mesh {
    Cache::~Cache() {
        for (const auto &mesh : meshes_ | std::views::values) {
            delete mesh;
        }
    }

    Data *Cache::GetOrPut(const Prefab::Definition &definition,
                          const std::function<Data *(const Prefab::Definition &definition)> &newFunc) {
        std::string cacheKey = definition.ToString();
        if (const auto itr = meshes_.find(cacheKey); itr != meshes_.end()) {
            return itr->second;
        }
        auto *meshData = newFunc(definition);
        if (meshData == nullptr) {
            throw std::runtime_error("newFunc returned NULL");
        }
        PLOG_DEBUG.printf("Cache mesh with key %s", cacheKey.c_str());
        meshes_.insert({cacheKey, meshData});
        return meshData;
    }

    Data *Cache::Get(const Prefab::Definition &definition) const {
        const std::string cacheKey = definition.ToString();
        if (const auto itr = meshes_.find(cacheKey); itr != meshes_.end()) {
            return itr->second;
        }
        return nullptr;
    }
} // namespace soil::video::mesh
