#ifndef SOIL_VIDEO_MESH_CACHE_H_
#define SOIL_VIDEO_MESH_CACHE_H_

#include <functional>
#include <string>
#include "base.h"
#include "mesh.h"

namespace soil::video::mesh {
    class Cache final {
    public:
        Cache() = default;

        ~Cache();

        Mesh *GetOrPut(const Definition &definition, const std::function<Mesh *(std::string name)> &func);

        Mesh *Get(const Definition &definition) const;

        Cache(Cache const &) = delete;

        void operator=(Cache const &) = delete;

    private:
        HashMap<std::string, Mesh *> meshes_;
    };
} // namespace soil::video::mesh

#endif // SOIL_VIDEO_MESH_CACHE_H_
