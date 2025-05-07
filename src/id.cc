#include "id.h"

namespace soil {
std::atomic_uint Id::currentId;

auto Id::Next() -> uint { return ++currentId; }
} // namespace engine
