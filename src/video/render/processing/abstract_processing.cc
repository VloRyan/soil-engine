#include "video/render/processing/abstract_processing.h"
#include <typeinfo>

namespace video::render {
    AbstractProcessing::AbstractProcessing()
        : requiredStep_(nullptr), name_(typeid(*this).name()) {
    }

    AbstractProcessing::AbstractProcessing(std::string name)
        : requiredStep_(nullptr), name_(std::move(name)) {
        if (name_.empty()) {
            name_ = typeid(*this).name();
        }
    }

    AbstractProcessing::~AbstractProcessing() {
        if (requiredStep_ != nullptr) {
            requiredStep_ = nullptr;
        }
    }

    void AbstractProcessing::onAttach() {
        // NOOP
    }

    AbstractProcessing* AbstractProcessing::GetRequiredStep() const {
        return requiredStep_;
    }

    void AbstractProcessing::SetName(std::string name) {
        name_ = std::move(name);
    }

    std::string AbstractProcessing::GetName() {
        return name_;
    }
}
