#include "video/model/letter.h"

namespace video::model {
bool Letter::isPrintable() const { return Id != ' '; }
} // namespace video::model
