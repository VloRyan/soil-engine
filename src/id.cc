#include "id.h"

namespace engine {
std::atomic_uint Id::currentId;

auto Id::Next() -> uint { return ++currentId; }
} // namespace engine
