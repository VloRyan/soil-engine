#include "video/render/step/base.h"

#include <stdexcept>

namespace soil::video::render::step {

    Base::Base(std::string id, Type type, Base* requiredStep) :
        requiredStep_(requiredStep), id_(std::move(id)), type_(type) {
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

    Type Base::GetType() const {
        return type_;
    }
} // namespace soil::video::render::step
