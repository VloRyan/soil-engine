#include "video/texture/texture.h"

#include <utility>

#include "GL/gl3w.h"
#include "plog/Log.h"

namespace soil::video::texture {
    Texture::Texture(const uint id, std::string name, const glm::uvec2 size, const Type type, const Format format) :
        id_(id), unit_(-1), handle_(0), isResident_(false), bufferIndex_(0), name_(std::move(name)), type_(type),
        format_(format), size_(size) {}

    Texture::~Texture() {
        PLOG_DEBUG << "Deleting Texture " << std::to_string(id_) << ": " << name_;
        free();
    }

    void Texture::free() {
        if (isResident_) {
            SetResident(false);
        }
        glDeleteTextures(1, &id_);
        id_ = 0;
        handle_ = 0;
    }

    std::string Texture::GetName() {
        return name_;
    }

    uint Texture::GetId() const {
        return id_;
    }

    Texture::Type Texture::GetType() const {
        return type_;
    }

    Texture::Format Texture::GetFormat() const {
        return format_;
    }

    glm::uvec2 Texture::GetSize() const {
        return size_;
    }

    uint64_t Texture::GetHandle() const {
        return handle_;
    }

    uint Texture::GetBufferIndex() const {
        return bufferIndex_;
    }

    bool Texture::IsResident() const {
        return isResident_;
    }

    void Texture::SetResident(const bool resident) {
        if (resident) {
            if (isResident_) {
                return;
            }
            handle_ = glGetTextureHandleARB(id_);
            glMakeTextureHandleResidentARB(handle_);
        } else {
            if (!isResident_) {
                return;
            }
            glMakeTextureHandleNonResidentARB(handle_);
        }
        isResident_ = resident;
    }

    char Texture::GetSlot() const {
        return unit_;
    }
} // namespace soil::video::texture
