#ifndef ENGINE_ENGINE_VIDEO_BUFFER_CURSOR_H_
#define ENGINE_ENGINE_VIDEO_BUFFER_CURSOR_H_
#include <glm/glm.hpp>
#include "base.h"

namespace video::buffer {
    class Cursor {
    public:
        explicit Cursor(byte *data);

        Cursor(const Cursor &orig) = delete;

        virtual ~Cursor();

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
} // buffer

#endif //ENGINE_ENGINE_VIDEO_BUFFER_CURSOR_H_
