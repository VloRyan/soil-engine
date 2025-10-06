#include "sound/file.h"

#include <utility>

namespace soil::sound {

File::File(std::string name, const InfoType& info)
    : info_(info), name_(std::move(name)) {}
const std::string& File::Name() const { return name_; }
const File::InfoType& File::Info() const { return info_; }
}  // namespace soil::sound
