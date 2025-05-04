#ifndef VIDEO_TEXTURE_DATA_H
#define VIDEO_TEXTURE_DATA_H
#include <glm/glm.hpp>
#include "base.h"

namespace video::texture {
    struct Data {
        Data(const byte *data, glm::ivec2 size, int format, int type);

        Data(const Data &other);

        virtual ~Data();

        static int getComponentsPerPixel(int format);

        byte *Bytes;
        glm::ivec2 Size;
        int Format;
        int Type;

    private:
        void copyData(const byte *data);
    };
}
#endif /* VIDEO_TEXTURE_DATA_H */
