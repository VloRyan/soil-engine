#include "video/buffer/cursor.h"

#include <algorithm>
#include <cstring>

namespace soil::video::buffer {
Cursor::Cursor(byte *data) : pos_(data), start_(data), max_(data) {}

void Cursor::Write(const int &data) { Write(&data, sizeof(int)); }

void Cursor::Write(const uint &data) { Write(&data, sizeof(uint)); }

void Cursor::Write(const float &data) { Write(&data, sizeof(float)); }

void Cursor::Write(const glm::vec2 &data) { Write(&data, sizeof(glm::vec2)); }

void Cursor::Write(const glm::vec3 &data) { Write(&data, sizeof(glm::vec3)); }

void Cursor::Write(const glm::vec4 &data) { Write(&data, sizeof(glm::vec4)); }

void Cursor::Write(const glm::mat4 &data) { Write(&data, sizeof(glm::mat4)); }

void Cursor::Write(const void *data, const size_t dataSize) {
  memcpy(pos_, data, dataSize);
  pos_ += dataSize;
  max_ = std::max(pos_, max_);
}

void Cursor::Move(const size_t offset) { pos_ += offset; }

void Cursor::MoveTo(const size_t offset) { pos_ = start_ + offset; }

size_t Cursor::GetDataWritten() const { return max_ - start_; }
}  // namespace soil::video::buffer
