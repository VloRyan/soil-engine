#include "video/texture/data.h"

#include <cstring>
#include "GL/gl3w.h"
#include <string>
#include "exception.h"

namespace video::texture {
    Data::Data(const byte *data, const glm::ivec2 size, const int format, const int type)
        : Bytes(nullptr), Size(size), Format(format), Type(type) {
        copyData(data);
    }

    Data::Data(const Data &other)
        : Bytes(nullptr), Size(other.Size), Format(other.Format), Type(other.Type) {
        copyData(other.Bytes);
    }

    Data::~Data() {
        delete[] Bytes;
    }

    void Data::copyData(const byte *data) {
        if (data == nullptr) {
            return;
        }
        if (Type != GL_UNSIGNED_BYTE) {
            throw engine::Exception("type " + std::to_string(Type) + " is not supported yet.");
        }
        const uint numPixel = Size.x * Size.y;
        constexpr uint sizeComponent = sizeof(byte);
        const uint sizePixel = getComponentsPerPixel(Format) * sizeComponent;
        const uint dataSize = numPixel * sizePixel;
        Bytes = new byte[dataSize];
        memcpy(Bytes, data, dataSize);
    }


    int Data::getComponentsPerPixel(const int format) {
        switch (format) {
            case GL_RGB:
            case GL_SRGB:
            case GL_BGR: {
                return 3;
            }
            case GL_RGBA:
            case GL_SRGB_ALPHA:
            case GL_BGRA: {
                return 4;
            }
            default:
                throw engine::Exception("unknown format: " + std::to_string(format));
        }
    }
}
