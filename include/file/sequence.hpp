
#ifndef SOIL_FILE_SEQUENCE_H
#define SOIL_FILE_SEQUENCE_H
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

namespace soil::file {
    struct Sequence {
        std::vector<int> FrameIndices{};
        int TicksPerFrame{0};

        friend bool operator==(const Sequence& lhs, const Sequence& rhs) {
            return lhs.FrameIndices == rhs.FrameIndices && lhs.TicksPerFrame == rhs.TicksPerFrame;
        }

        friend bool operator!=(const Sequence& lhs, const Sequence& rhs) {
            return !(lhs == rhs);
        }
    };

    static void to_json(json& j, const Sequence& p) {
        j = json{{"frameIndices", p.FrameIndices}, //
                 {"ticksPerFrame", json(p.TicksPerFrame)}};
    }

    static void from_json(const json& j, Sequence& p) {
        j.at("frameIndices").get_to(p.FrameIndices);
        j.at("ticksPerFrame").get_to(p.TicksPerFrame);
    }
} // namespace wild_soil::texture

#endif

