#ifndef SOIL_VIDEO_BUFFER_EBO_H_
#define SOIL_VIDEO_BUFFER_EBO_H_
#include "video/buffer/object.h"
#include "video/vertex/vertex.h"

namespace soil::video::buffer {
/**Element Array Buffer */
class Ebo final : public Object {
 public:
  explicit Ebo(vertex::IndexType indexType = vertex::IndexType::TYPE_USHORT,
               UsageType usage = UsageType::Static,
               AccessType access = AccessType::Draw);

  Ebo(const Ebo& orig) = delete;

  ~Ebo() override;

  [[nodiscard]] vertex::IndexType GetIndexType() const;

  void SetData(const void* data, gl_size_t dataSize) override;

  [[nodiscard]] int GetIndexCount() const;

 private:
  vertex::IndexType indexType_;
  int indexCount_;
};
}  // namespace soil::video::buffer
#endif
