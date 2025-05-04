#ifndef VIDEO_TEXTURE_MANAGER_H
#define VIDEO_TEXTURE_MANAGER_H
#include <vector>
#include <video/texture/data.h>
#include "parameter.h"
#include "texture.h"

namespace video::texture
{
#define MAX_BINDLESS_TEXTURES 255U

    struct TextureBuffer
    {
        GLuint64 texture[MAX_BINDLESS_TEXTURES];
    };

    struct TextureCacheEntry
    {
        explicit TextureCacheEntry(Texture *texture);

        int useCount;
        Texture *texture;
    };

    class Manager
    {
    public:
        explicit Manager();

        virtual ~Manager();

        // void Init(buffer::Object *uboTextures);

        Texture *GetTexture2D(const std::string &fileName, const Parameter &parameter);

        Texture *GetTextureArray2D(const std::string &fileName, int tilesPerDim, const Parameter &parameter);

        Texture *GetCubeMap(const std::string &name, const std::string &ext = ".tga",
                            Texture::Format internalFormat = Texture::Format::sRGB);

        void Update();

        Manager(Manager const &) = delete;

        void operator=(Manager const &) = delete;

        static Texture *GenerateTexture2D(const Data &data, const std::string &name, const Parameter &parameter);

        static Texture *GenerateTextureArray2D(const Data &data, const std::string &name, int tilesPerDim,
                                               const Parameter &parameter);

    private:
        static void initDevIL();

        static uint loadCubeMap(const std::string &name, const std::string &ext = ".tga",
                                Texture::Format format = Texture::Format::sRGB);

        static void loadCubeMapFace(const std::string &texturePathFile, uint target, Texture::Format format);

        static Data *loadData(const std::string &fileName);

        static uint getGLFormat(Texture::Format format);

        static Texture::Format getTextureFormat(uint format);

        static int getGLWrapType(Parameter::WrapType type);

        static int getGLMagFilter(Parameter::MagFilterType type);

        static int getGLMinFilter(Parameter::MinFilterType type);

        static bool isMipMapping(Parameter::MinFilterType type);

        static void logGLError(const char *text);

        bool cacheDirty_;
        std::vector<TextureCacheEntry> texturesCache_;
        // buffer::Object *uboTextures_;
    };
} // namespace video::texture
#endif /* VIDEO_TEXTURE_MANAGER_H */
