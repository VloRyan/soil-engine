#ifndef ENGINE_VIDEO_MESH_CACHE_H_
#define ENGINE_VIDEO_MESH_CACHE_H_

#include "../../base.h"
#include "mesh.h"
#include <functional>
#include <string>

namespace video::mesh {
 class Cache {
 public:
  Cache() = default;

  virtual ~Cache();

  Mesh* GetOrPut(const Definition &definition, const std::function<Mesh*(std::string name)> &func);

  Mesh* Get(const Definition &definition) const;

  Cache(Cache const &) = delete;

  void operator=(Cache const &) = delete;

 private:
  HashMap<std::string, Mesh *> meshes_;
 };
} // video::shader

#endif //ENGINE_VIDEO_MESH_CACHE_H_
