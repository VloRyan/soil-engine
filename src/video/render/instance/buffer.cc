
#include "video/render/instance/buffer.h"

#include "exception.h"

namespace soil::video::render::instance {
    Buffer::Buffer(buffer::Object *buffer, const size_t instanceSize) :
        perInstanceBuffer_(buffer), instanceSize_(instanceSize) {}

    Buffer::~Buffer() {
        preparedInstances_.clear();
        dirtyInstances_.clear();
    }

    buffer::Object *Buffer::GetPerInstanceBuffer() const { return perInstanceBuffer_; }

    size_t Buffer::GetInstanceSize() const { return instanceSize_; }

    size_t Buffer::GetPreparedInstancesCount() const { return preparedInstances_.size(); }

    void Buffer::Update(const glm::vec3 &viewerPos) {
        if (dirtyInstances_.empty()) {
            return;
        }

        buffer::Object *perInstanceBuffer = this->GetPerInstanceBuffer();
        if (perInstanceBuffer == nullptr) {
            return;
        }

        const auto instanceSize = this->GetInstanceSize();
        const auto bufferSizeInInstances = perInstanceBuffer->GetBufferSize() / instanceSize;
        const auto maxInstancesPerDraw = bufferSizeInInstances;

        auto *cursor = perInstanceBuffer->GetCursor();

        for (auto *instance : dirtyInstances_) {
            if (instance->GetIndex() == -1) {
                if (maxInstancesPerDraw == preparedInstances_.size()) {
                    throw Exception("Buffer is too small");
                }
                instance->SetIndex(static_cast<int>(preparedInstances_.size()));
                preparedInstances_.push_back(instance);
            }
            cursor->MoveTo(instanceSize * instance->GetIndex());
            instance->WriteData(cursor, instance->GetIndex());
        }
        dirtyInstances_.clear();
        perInstanceBuffer->Flush();
    }

    void Buffer::AddChangedInstance(Instance *instance) {
        if (instance->GetIndex() == -1) {
            // unknown
            return;
        }
        for (const auto *dirtyInstance : dirtyInstances_) {
            if (dirtyInstance == instance) {
                return;
            }
        }
        dirtyInstances_.push_back(instance);
    }

    void Buffer::AddInstance(Instance *instance) {
        if (instance->GetIndex() != -1) {
            return;
        }
        for (const auto &dirtyInstance : dirtyInstances_) {
            if (dirtyInstance == instance) {
                return;
            }
        }
        for (const auto &prepInstance : preparedInstances_) {
            if (prepInstance == instance) {
                return;
            }
        }
        dirtyInstances_.push_back(instance);
    }

    bool Buffer::RemoveInstance(Instance *instance) {
        for (auto itr = dirtyInstances_.begin(); itr != dirtyInstances_.end(); ++itr) {
            if (*itr == instance) {
                dirtyInstances_.erase(itr);
                break;
            }
        }
        for (auto i = 0; i < preparedInstances_.size(); ++i) {
            if (preparedInstances_[i] == instance) {
                if (preparedInstances_[i] != preparedInstances_.back()) {
                    preparedInstances_[i] = preparedInstances_.back();
                    preparedInstances_[i]->SetIndex(i);
                    dirtyInstances_.push_back(preparedInstances_[i]);
                }
                instance->SetIndex(-1);
                preparedInstances_.pop_back();
                return true;
            }
        }
        return false;
    }
} // namespace soil::video::render::instance
