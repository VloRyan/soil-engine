#ifndef SOIL_VIDEO_TEXTURE_DATA_H
#define SOIL_VIDEO_TEXTURE_DATA_H
#include <glm/glm.hpp>
#include "base.h"

namespace soil::video::texture {
    struct Data final {
        Data(const byte *data, glm::ivec2 size, int format, int type);

        Data(const Data &other);

        ~Data();

        static int getComponentsPerPixel(int format);

        byte *Bytes;
        glm::ivec2 Size;
        int Format;
        int Type;

    private:
        void copyData(const byte *data);
    };
} // namespace soil::video::texture
#endif /* SOIL_VIDEO_TEXTURE_DATA_H */
