#include "video/vertex/vao.h"
#include "GL/gl3w.h"
#include "plog/Log.h"

namespace soil::video::vertex {
    Vao::Vao() : id_(0), ebo_(nullptr) {}

    Vao::~Vao() {
        Unload();
        for (const auto *ptr : attribPointer_) {
            delete ptr;
        }
    }

    void Vao::Create(const void *indices, const uint indexSize) {
        if (IsCreated()) {
            return;
        }
        glGenVertexArrays(1, &this->id_);
        this->Bind();
        // Element buffer
        if (indexSize > 0) {
            if (indices != nullptr) {
                ebo_ = new buffer::Ebo();
            } else {
                ebo_ = new buffer::Ebo(buffer::Ebo::UsageType::Dynamic);
            }
            ebo_->SetData(indices, indexSize);
        }
        uint index = 0;
        for (const AttributePointer *pointer : GetAttribPointer()) {
            pointer->Set(index++);
        }
        Unbind();
    }

    void Vao::Unload() {
        if (this->IsCreated()) {
            if (ebo_ != nullptr) {
                delete ebo_;
                ebo_ = nullptr;
            }
            glDeleteVertexArrays(1, &this->id_);
        }
        this->id_ = 0;
    }

    size_t Vao::AddAttributePointer(buffer::Object *buffer, const AttributePointer::DataType dataType,
                                    const int elementSize, const GLsizei elementStride, const size_t offset,
                                    const bool perInstance) {
        const auto divisor = perInstance ? 1 : 0;
        auto *vPointer = new AttributePointer(buffer, dataType, elementSize, elementStride, divisor, offset);
        attribPointer_.push_back(vPointer);
        return elementSize * AttributePointer::GetSizeOfDataType(dataType) + offset;
    }

    void Vao::Bind() const {
#ifdef DEBUG
        if (this->id_ == 0) {
            PLOG_DEBUG << "Id == 0";
        }
#endif
        glBindVertexArray(this->id_);
    }

    void Vao::Unbind() { glBindVertexArray(0); }

    buffer::Ebo *Vao::GetEbo() const { return ebo_; }

    uint Vao::GetId() const { return id_; }

    const std::vector<AttributePointer *> &Vao::GetAttribPointer() const { return attribPointer_; }

    bool Vao::IsCreated() const { return id_ > 0; }
} // namespace soil::video::vertex
