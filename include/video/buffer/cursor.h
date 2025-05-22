#ifndef SOIL_VIDEO_BUFFER_CURSOR_H_
#define SOIL_VIDEO_BUFFER_CURSOR_H_
#include "base.h"
#include "glm/glm.hpp"

namespace soil::video::buffer {
    class Cursor final {
    public:
        explicit Cursor(byte *data);

        Cursor(const Cursor &orig) = delete;

        ~Cursor() = default;

        void Write(const int &data);

        void Write(const uint &data);

        void Write(const float &data);

        void Write(const glm::vec2 &data);

        void Write(const glm::vec3 &data);

        void Write(const glm::vec4 &data);

        void Write(const glm::mat4 &data);

        void Write(const void *data, size_t dataSize);

        void Move(size_t offset);

        void MoveTo(size_t offset);

        [[nodiscard]] size_t GetDataWritten() const;

    private:
        byte *pos_;
        byte *start_;
        byte *max_;
    };
} // namespace soil::video::buffer

#endif // SOIL_VIDEO_BUFFER_CURSOR_H_
