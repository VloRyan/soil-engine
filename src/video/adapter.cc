#include "video/adapter.h"

#include <utility>

namespace video
{
    Adapter::Adapter(const Vendor vendor, std::string model) : vendor_(vendor), model_(std::move(model)) {}

    Vendor Adapter::GetVendor() const { return vendor_; }

    std::string Adapter::GetModel() { return model_; }
} // namespace video
