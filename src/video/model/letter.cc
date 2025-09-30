#include "video/model/letter.h"

namespace soil::video::model {
bool Letter::IsPrintable() const { return Id != ' '; }
}  // namespace soil::video::model
