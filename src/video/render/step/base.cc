#include "video/render/step/base.h"

#include <stdexcept>

namespace soil::video::render::step {
    Base::Base(std::string id, Base* requiredStep) : requiredStep_(requiredStep), id_(std::move(id)) {
        if (id_.empty()) {
            throw std::runtime_error("id is empty");
        }
    }

    Base* Base::GetRequiredStep() const {
        return requiredStep_;
    }

    std::string Base::GetId() {
        return id_;
    }
} // namespace soil::video::render::step
