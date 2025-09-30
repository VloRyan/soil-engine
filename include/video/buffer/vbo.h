#ifndef SOIL_VIDEO_BUFFER_VBO_H_
#define SOIL_VIDEO_BUFFER_VBO_H_
#include "base.h"
#include "object.h"

namespace soil::video::buffer {
class Vbo final : public Object {
 public:
  /**
   * Constructs a new VertexBufferObject.
   * @param usage Specifies the expected usage pattern of the data store.
   * @param access
   */
  explicit Vbo(UsageType usage = UsageType::Static,
               AccessType access = AccessType::Draw);

  /** Default destructor */
  ~Vbo() override;

  void Flush() override;

  void Reserve(gl_size_t newBufferSize) override;

 private:
  bool directWrite_;
};
}  // namespace soil::video::buffer

#endif
