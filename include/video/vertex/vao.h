#ifndef SOIL_VIDEO_VERTEX_VAO_H_
#define SOIL_VIDEO_VERTEX_VAO_H_
#include <string>
#include <vector>

#include "video/buffer/ebo.h"
#include "video/buffer/object.h"
#include "video/vertex/attribute_pointer.h"

namespace soil::video::vertex {
class Vao final {
 public:
  Vao();

  ~Vao();

  [[nodiscard]] uint GetId() const;

  size_t AddAttributePointer(const std::string& bufferName,
                             AttributePointer::DataType dataType,
                             int elementSize, int elementStride = 0,
                             size_t offset = 0, bool perInstance = false);

  void CreateWithEbo(const void* indices, IndexType indexType, uint indexCount);

  void Unload();

  void Bind() const;

  static void Unbind();

  [[nodiscard]] bool IsCreated() const;

  [[nodiscard]] const std::vector<AttributePointer*>& GetAttribPointer() const;

  [[nodiscard]] buffer::Ebo* GetEbo() const;

  [[nodiscard]] buffer::Object* GetBuffer(const std::string& name) const;

  buffer::Object* SetBuffer(const std::string& name, buffer::Object* vbo);

 private:
  uint id_;
  buffer::Ebo* ebo_;
  std::vector<buffer::Object*> bufferObjects_;
  std::unordered_map<std::string, size_t> bufferNamesToIndex_;
  std::vector<AttributePointer*> attribPointer_;
};
}  // namespace soil::video::vertex

#endif
