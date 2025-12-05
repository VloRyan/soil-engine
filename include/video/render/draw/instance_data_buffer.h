#ifndef SOIL_VIDEO_RENDER_DRAW_INSTANCE_DATA_BUFFER_H
#define SOIL_VIDEO_RENDER_DRAW_INSTANCE_DATA_BUFFER_H
#include <vector>

#include "vao_elements_instanced.h"
#include "video/buffer/object.h"
#include "video/render/data/buffer_writer.h"

namespace soil::video::render::draw {

class InstanceDataBuffer {
 public:
  InstanceDataBuffer(buffer::Object* buffer, size_t instanceSize);
  InstanceDataBuffer(const InstanceDataBuffer& other) = delete;
  InstanceDataBuffer(InstanceDataBuffer&& other) noexcept = delete;
  InstanceDataBuffer& operator=(const InstanceDataBuffer& other) = delete;
  InstanceDataBuffer& operator=(InstanceDataBuffer&& other) noexcept = delete;
  virtual ~InstanceDataBuffer();

  void Insert(VaoElementsInstanced::Data* instance);
  bool Remove(VaoElementsInstanced::Data* instance);
  void SetDirty(VaoElementsInstanced::Data* instance);

  [[nodiscard]] buffer::Object* GetPerInstanceBuffer() const;

  [[nodiscard]] size_t InstanceSize() const;
  [[nodiscard]] size_t InstancesCount() const;
  [[nodiscard]] size_t DirtyInstancesCount() const;

  virtual void Update();

  [[nodiscard]] virtual const std::vector<VaoElementsInstanced::Data*>& Data();

 protected:
  static constexpr auto UnsetId = -1;
  std::vector<VaoElementsInstanced::Data*> instances_;
  std::vector<VaoElementsInstanced::Data*> dirtyInstances_;

  buffer::Object* buffer_;
  size_t instanceSize_;
  data::BufferWriter* dataWriter_;
};
}  // namespace soil::video::render::draw

#endif
