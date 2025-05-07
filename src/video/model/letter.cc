#include "video/model/letter.h"

namespace soil::video::model {
    bool Letter::isPrintable() const { return Id != ' '; }
} // namespace soil::video::model
