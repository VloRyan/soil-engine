#ifndef VIDEO_TEXTURE_H
#define VIDEO_TEXTURE_H
#include <string>
#include <glm/glm.hpp>
#include "base.h"

namespace video::texture {
    class
            Texture {
        friend class Manager;
        friend class Loader;

    public:
        enum Format : std::uint8_t {
            RGB,
            RGBA,
            RGBA16F,
            sRGB,
            sRGBA,
            DepthComponent,
            DepthComponent16,
            DepthComponent24,
            DepthComponent32
        };

        enum Type : std::uint8_t {
            Texture2D,
            TextureArray2D,
            Cubemap,
        };

        Texture(uint id, std::string name, glm::uvec2 size, Type type = Texture2D, Format format = sRGBA);


        virtual ~Texture();

        /**
         * Returns the filename of the texture.
         * @return
         */
        std::string GetName();

        /**
         * Returns the OpenGl id.
         * @return OpenGl Id
         */
        [[nodiscard]] uint GetId() const;

        [[nodiscard]] Type GetType() const;

        [[nodiscard]] Format GetFormat() const;

        [[nodiscard]] glm::uvec2 GetSize() const;

        [[nodiscard]] uint64_t GetHandle() const;

        [[nodiscard]] uint GetBufferIndex() const;

        [[nodiscard]] bool IsResident() const;

        void SetResident(bool);

    protected:
        void free();

    private:
        uint id_;
        uint64_t handle_;
        bool isResident_;
        uint bufferIndex_;
        std::string name_;
        Type type_;
        Format format_;
        glm::uvec2 size_;
    };
}
#endif /* VIDEO_TEXTURE_H */


