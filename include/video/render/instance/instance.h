#ifndef SOIL_VIDEO_RENDER_INSTANCE_INSTANCE_H_
#define SOIL_VIDEO_RENDER_INSTANCE_INSTANCE_H_

#include "video/buffer/cursor.h"

namespace soil::video::render {
class Batch;
}

namespace soil::video::render::instance {
class Instance {
 public:
  explicit Instance();

  virtual ~Instance() = default;

  Instance(const Instance& other) = delete;

  virtual void WriteData(buffer::Cursor* cursor) const = 0;

  [[nodiscard]] virtual int GetIndex() const;

  virtual void SetIndex(int index);

  [[nodiscard]] virtual const Batch* GetBatch() const;

  virtual void SetBatch(const Batch* batch);

 private:
  int index_;
  const Batch* batch_;
};
}  // namespace soil::video::render::instance

#endif
