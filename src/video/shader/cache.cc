#include "video/shader/cache.h"

#include <stdexcept>

namespace soil::video::shader {
    Cache::~Cache() {
        for (const auto &entry : shaders_) {
            delete entry.second;
        }
    }

    void Cache::ForEach(const std::function<void(Shader *)> &callback) const {
        for (const auto &entry : shaders_) {
            callback(entry.second);
        }
        for (const auto &entry : instanceShaders_) {
            callback(entry.second);
        }
    }

    Shader *Cache::GetByName(const std::string &name) {
        if (const auto itr = shaders_.find(name); itr != shaders_.end()) {
            return itr->second;
        }
        return nullptr;
    }

    InstanceShader *Cache::GetByName2(const std::string &name) {
        if (const auto itr = instanceShaders_.find(name); itr != instanceShaders_.end()) {
            return itr->second;
        }
        return nullptr;
    }

    Shader *Cache::GetById(const int id) const {
        for (const auto &entry : shaders_) {
            if (entry.second->GetId() == id) {
                return entry.second;
            }
        }
        return nullptr;
    }

    void Cache::Prepare(const std::string &name, const std::string &path) {
        auto *shader = GetByName(name);
        if (shader != nullptr) {
            return;
        }
        shader = new Shader(name, path);
        std::pair pair(name, shader);
        shaders_.insert(pair);
    }

    void Cache::Prepare(const std::string &name, Shader *shader) {
        if (const auto *existing = GetByName(name); existing != nullptr) {
            throw std::runtime_error("Shader already exists");
        }
        std::pair pair(name, shader);
        shaders_.insert(pair);
    }

    void Cache::PrepareInstanceShader(const std::string &name, InstanceShader *shader) {
        if (const auto *existing = GetByName2(name); existing != nullptr) {
            throw std::runtime_error("Shader already exists");
        }
        std::pair pair(name, shader);
        instanceShaders_.insert(pair);
    }

    Shader *Cache::Create(const std::string &name, const Definition &shaderDef) {
#ifdef DEBUG
        if (shaders_.contains(name)) {
            throw engine::Exception("Shader " + name + " already cached");
        }
#endif
        auto *shader = new Shader(shaderDef);
        shader->SetName(name);
        std::pair pair(name, shader);
        shaders_.insert(pair);
        return shader;
    }
} // namespace soil::video::shader
