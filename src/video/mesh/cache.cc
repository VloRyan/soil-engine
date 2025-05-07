#include "video/mesh/cache.h"
#include <plog/Log.h>

namespace soil::video::mesh {
    Cache::~Cache() {
        for (const auto &entry : meshes_) {
            delete entry.second;
        }
    }

    Mesh *Cache::GetOrPut(const Definition &definition, const std::function<Mesh *(std::string name)> &func) {
        std::string cacheKey = definition.ToString();
        if (const auto itr = meshes_.find(cacheKey); itr != meshes_.end()) {
            return itr->second;
        }
        auto *const mesh = func(cacheKey);
        PLOG_DEBUG.printf("Cache mesh with key %s", cacheKey.c_str());
        meshes_.insert(std::pair(cacheKey, mesh));
        mesh->id_ = static_cast<int>(meshes_.size());
        return mesh;
    }

    Mesh *Cache::Get(const Definition &definition) const {
        const std::string cacheKey = definition.ToString();
        if (const auto itr = meshes_.find(cacheKey); itr != meshes_.end()) {
            return itr->second;
        }
        return nullptr;
    }
} // namespace soil::video::mesh
