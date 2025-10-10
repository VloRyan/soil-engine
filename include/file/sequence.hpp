#ifndef SOIL_FILE_SEQUENCE_H
#define SOIL_FILE_SEQUENCE_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace soil::file {
struct Sequence {
  int From{0};
  int To{0};
  int FrameDuration{0};

  friend bool operator==(const Sequence& lhs, const Sequence& rhs) {
    return lhs.From == rhs.From && lhs.To == rhs.To &&
           lhs.FrameDuration == rhs.FrameDuration;
  }
  friend bool operator!=(const Sequence& lhs, const Sequence& rhs) {
    return !(lhs == rhs);
  }
};

static void to_json(json& j, const Sequence& p) {
  j = json{{"from", p.From},  //
           {"to", p.To},      //
           {"frameDuration", p.FrameDuration}};
}

static void from_json(const json& j, Sequence& p) {
  j.at("from").get_to(p.From);
  j.at("to").get_to(p.To);
  j.at("frameDuration").get_to(p.FrameDuration);
}
}  // namespace soil::file

#endif
