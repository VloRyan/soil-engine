#ifndef SOIL_VIDEO_MESH_CACHE_H_
#define SOIL_VIDEO_MESH_CACHE_H_

#include <functional>
#include <string>
#include "base.h"
#include "data.h"
#include "prefab.h"

namespace soil::video::mesh {
    class Cache final {
    public:
        Cache() = default;

        ~Cache();

        Data *GetOrPut(const Prefab::Definition &definition,
                       const std::function<Data *(const Prefab::Definition &definition)> &newFunc);

        Data *Get(const Prefab::Definition &definition) const;

        Cache(Cache const &) = delete;

        void operator=(Cache const &) = delete;

    private:
        HashMap<std::string, Data *> meshes_;
    };
} // namespace soil::video::mesh

#endif // SOIL_VIDEO_MESH_CACHE_H_
