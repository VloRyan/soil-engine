#include "video/texture/manager.h"

#include <GL/gl3w.h>
#include <plog/Log.h>
#include <util/files.h>

#include "IL/il.h"
#include "IL/ilu.h"

namespace soil::video::texture {
    Manager::Manager() {
        initDevIL();
    }

    Manager::~Manager() {
        for (auto itr = texturesCache_.begin(); itr != texturesCache_.end();) {
            const auto entry = *itr;
            delete entry.texture;
            itr = texturesCache_.erase(itr);
        }
        texturesCache_.clear();
    }

    void Manager::initDevIL() {
        if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) {
            throw std::runtime_error("DevIL library is out of date! Please upgrade");
        }

        // Needed to initialize DevIL.
        ilInit();
        iluInit();
        // GL cannot Use palettes anyway, so convert early.
        ilEnable(IL_CONV_PAL);

        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    }

    void Manager::Update() {
        for (auto itr = texturesCache_.begin(); itr != texturesCache_.end();) {
            if (const auto entry = *itr; entry.useCount == 0) {
                delete entry.texture;
                itr = texturesCache_.erase(itr);
            } else {
                ++itr;
            }
        }
    }

    Texture* Manager::GetTexture2D(const std::string& fileName, const Parameter& parameter) {
        // Check if texture is loaded
        for (auto entry : texturesCache_) {
            const bool sameType = entry.texture->GetType() == Texture::Type::Texture2D;
            const bool sameName = !entry.texture->GetName().empty() && entry.texture->GetName() == fileName;
            if (sameType && sameName) {
                entry.useCount++;
                return entry.texture;
            }
        }
        const auto* data = loadData(fileName);
        if (data == nullptr) {
            return nullptr;
        }
        auto* texture = GenerateTexture2D(*data, fileName, parameter);
        delete data;
        const auto entry = texturesCache_.emplace_back(texture);
        return entry.texture;
    }

    Texture* Manager::GenerateTexture2D(const Data& data, const std::string& name, const Parameter& parameter) {
        logGLError("Error before loadTexture2D");

        // Open Gl stuff
        uint id = 0;
        glActiveTexture(GL_TEXTURE0);
        logGLError("Error after glActiveTexture");
        glGenTextures(1, &id);
        logGLError("Error after glGenTextures");
        glBindTexture(GL_TEXTURE_2D, id);
        const auto internalFormat = static_cast<GLint>(parameter.InternalFormat);
        // Load texture from file, and build all mipmap levels
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data.Size.x, data.Size.y, 0, data.Format, data.Type, data.Bytes);

        logGLError("Error after glTexImage2D");
        const auto wrap = static_cast<GLint>(parameter.Wrap);
        const auto minFilter = static_cast<GLint>(parameter.MinFilter);
        const auto magFilter = static_cast<GLint>(parameter.MagFilter);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

        if (isMipMapping(parameter.MinFilter)) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        return new Texture(id, name, data.Size, Texture::Type::Texture2D, parameter.Format);
    }

    Texture* Manager::GetTextureArray2D(const std::string& fileName, const int tilesPerDim,
                                        const Parameter& parameter) {
        // Check if texture is loaded
        for (auto entry : texturesCache_) {
            if (const bool sameType = entry.texture->GetType() == Texture::Type::TextureArray2D;
                sameType && !entry.texture->GetName().empty() && entry.texture->GetName() == fileName) {
                entry.useCount++;
                return entry.texture;
            }
        }
        const auto* data = loadData(fileName);
        if (data == nullptr) {
            return nullptr;
        }
        auto* texture = GenerateTextureArray2D(*data, fileName, tilesPerDim, parameter);
        delete data;
        const auto entry = texturesCache_.emplace_back(texture);
        return entry.texture;
    }

    Texture* Manager::GenerateTextureArray2D(const Data& data, const std::string& name, const int tilesPerDim,
                                             const Parameter& parameter) {
        logGLError("Error before generate");
        const auto internalFormat = static_cast<GLint>(parameter.Format);
        const auto tileSize = data.Size.x / tilesPerDim;
        const auto channels = Data::getComponentsPerPixel(data.Format);
        const auto tilesCount = tilesPerDim * tilesPerDim;
        glActiveTexture(GL_TEXTURE0);
        GLuint id = 0;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);
        glTexImage3D(GL_TEXTURE_2D_ARRAY,
                     0, // mipmap level
                     internalFormat, // gpu texel format
                     tileSize, // width
                     tileSize, // height
                     tilesCount, // depth
                     0, // border
                     data.Format, // cpu pixel format
                     data.Type, // cpu pixel coord type
                     nullptr); // pixel data
        logGLError("Error before glTexImage3D");
        std::vector<unsigned char> tile(tileSize * tileSize * channels);
        const int tileSizeX = tileSize * channels;
        const int rowLen = tilesPerDim * tileSizeX;

        for (int iy = 0; iy < tilesPerDim; ++iy) {
            for (int ix = 0; ix < tilesPerDim; ++ix) {
                unsigned char* ptr = data.Bytes + iy * rowLen * tileSize + ix * tileSizeX;
                for (int row = 0; row < tileSize; ++row) {
                    std::copy_n(ptr + row * rowLen, tileSizeX, tile.begin() + row * tileSizeX);
                }
                const int tileIndex = tilesCount - tilesPerDim - iy * tilesPerDim + ix;
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, tileIndex, tileSize, tileSize, 1, data.Format, data.Type,
                                tile.data());
                logGLError("Error before glTexSubImage3D");
            }
        }

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        logGLError("Error before glGenerateMipmap");
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(parameter.MinFilter));
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(parameter.MagFilter));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(parameter.Wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(parameter.Wrap));
        logGLError("Error before glTexParameteri");
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        return new Texture(id, name, data.Size, Texture::Type::TextureArray2D, parameter.Format);
    }

    Data* Manager::loadData(const std::string& fileName) {
        logGLError(("Error before loadData " + fileName).c_str());

        ILuint ilImgId = 0;
        const bool isWithPath = fileName.find('\\') != std::string::npos || fileName.find('/') != std::string::npos;
        std::string textureFileNamePath;
        if (isWithPath) {
            textureFileNamePath = fileName;
        } else {
            textureFileNamePath = "Texture/" + fileName;
        }
        if (!util::Files::Exists(textureFileNamePath)) {
            throw std::runtime_error("Texture file not found");
        }
        PLOG_DEBUG << "load texture " << textureFileNamePath;
        // Generate the main image name to Use.
        ilGenImages(1, &ilImgId);
        // Bind this image name.
        ilBindImage(ilImgId);

        if (ilLoadImage(textureFileNamePath.c_str()) == IL_FALSE) {
            ILenum error = ilGetError();
            while (error != IL_NO_ERROR) {
                PLOG_ERROR << iluErrorString(error) << ": " << textureFileNamePath;
                error = ilGetError();
            }
            return nullptr;
        }

        auto* const data = new Data(ilGetData(), // will copy data
                                    glm::ivec2(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT)),
                                    ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE));
        ilDeleteImages(1, &ilImgId);
        return data;
    }

    Texture* Manager::GetCubeMap(const std::string& name, const std::string& ext,
                                 const Texture::Format internalFormat) {
        const uint openGlId = loadCubeMap(name, ext, internalFormat);
        const auto entry = texturesCache_.emplace_back(new Texture(openGlId, name, glm::uvec2(0)));
        return entry.texture;
    }

    uint Manager::loadCubeMap(const std::string& name, const std::string& ext, const Texture::Format internalFormat) {
        PLOG_INFO << "load cubeMap " << name;

        std::string fileTop = "Texture/cubemap/" + name + "/" + name + "_top" + ext;
        std::string fileBottom;
        std::string fileLeft;
        std::string fileRight;
        std::string fileBack;
        std::string fileFront;

        if (util::Files::Exists(fileTop)) {
            fileBottom = "Texture/cubemap/" + name + "/" + name + "_bottom" + ext;
            fileLeft = "Texture/cubemap/" + name + "/" + name + "_left" + ext;
            fileRight = "Texture/cubemap/" + name + "/" + name + "_right" + ext;
            fileBack = "Texture/cubemap/" + name + "/" + name + "_back" + ext;
            fileFront = "Texture/cubemap/" + name + "/" + name + "_front" + ext;
        } else {
            fileTop = "Texture/cubemap/" + name + "/" + name + "_up" + ext;
            if (!util::Files::Exists(fileTop)) {
                throw std::runtime_error("CubeMap textures not found");
            }
            fileBottom = "Texture/cubemap/" + name + "/" + name + "_dn" + ext;
            fileLeft = "Texture/cubemap/" + name + "/" + name + "_lf" + ext;
            fileRight = "Texture/cubemap/" + name + "/" + name + "_rt" + ext;
            fileBack = "Texture/cubemap/" + name + "/" + name + "_bk" + ext;
            fileFront = "Texture/cubemap/" + name + "/" + name + "_ft" + ext;
        }

        glActiveTexture(GL_TEXTURE0);
        uint texId = 0;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

        loadCubeMapFace(fileRight, GL_TEXTURE_CUBE_MAP_POSITIVE_X, internalFormat);
        loadCubeMapFace(fileLeft, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, internalFormat);

        loadCubeMapFace(fileTop, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, internalFormat);
        loadCubeMapFace(fileBottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, internalFormat);

        // loadCubeMapFace(fileBack, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, internalFormat);
        // loadCubeMapFace(fileFront, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, internalFormat);
        loadCubeMapFace(fileFront, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, internalFormat);
        loadCubeMapFace(fileBack, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, internalFormat);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        logGLError("Error after cube");
        return texId;
    }

    void Manager::loadCubeMapFace(const std::string& texturePathFile, const GLenum target,
                                  const Texture::Format internalFormat) {
        ILuint ilImgId = 0;
        // Generate the main image name to Use.
        ilGenImages(1, &ilImgId);
        // Bind this image name.
        ilBindImage(ilImgId);

        if (ilLoadImage(texturePathFile.c_str()) == IL_FALSE) {
            ILenum error = ilGetError();

            while (error != IL_NO_ERROR) {
                PLOG_ERROR << texturePathFile << ": " << iluErrorString(error);
                error = ilGetError();
            }
        }

        const int width = ilGetInteger(IL_IMAGE_WIDTH);
        const int height = ilGetInteger(IL_IMAGE_HEIGHT);
        const uint format = static_cast<uint>(ilGetInteger(IL_IMAGE_FORMAT));
        const uint type = static_cast<uint>(ilGetInteger(IL_IMAGE_TYPE));

        // CubeMap texture have origin top left
        iluFlipImage();

        glTexImage2D(target, 0, static_cast<GLint>(internalFormat), width, height, 0, format, type, ilGetData());
        logGLError("Error after glTexImage2D");

        ilDeleteImages(1, &ilImgId);
    }

    void Manager::logGLError(const char* text) {
        if (const GLenum glError = glGetError(); glError != GL_NO_ERROR) {
            switch (glError) {
            case GL_INVALID_ENUM:
                PLOG_ERROR << text << ": Invalid enum";
                break;
            case GL_INVALID_VALUE:
                PLOG_ERROR << text << ": Invalid value";
                break;
            case GL_INVALID_OPERATION:
                PLOG_ERROR << text << ": Invalid operation";
                break;
            default:
                PLOG_ERROR << text << ": Unrecognised GLenum(" << std::to_string(glError) << ")";
                break;
            }
        }
    }

    bool Manager::isMipMapping(const Parameter::MinFilterType type) {
        return type == Parameter::MinFilterType::LINEAR_MIPMAP_LINEAR ||
            type == Parameter::MinFilterType::LINEAR_MIPMAP_NEAREST ||
            type == Parameter::MinFilterType::NEAREST_MIPMAP_LINEAR ||
            type == Parameter::MinFilterType::NEAREST_MIPMAP_NEAREST;
    }

    TextureCacheEntry::TextureCacheEntry(Texture* texture) : useCount(1), texture(texture) {}
} // namespace soil::video::texture
