#include "video/render/instance/instance.h"

namespace soil::video::render::instance {
    Instance::Instance() : index_(-1), batch_(nullptr) {}

    int Instance::GetIndex() const {
        return index_;
    }

    void Instance::SetIndex(const int index) {
        index_ = index;
    }

    const Batch* Instance::GetBatch() const {
        return batch_;
    }

    void Instance::SetBatch(const Batch* batch) {
        batch_ = batch;
    }
} // namespace soil::video::render::instance
