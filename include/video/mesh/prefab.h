#ifndef PREFAB_H
#define PREFAB_H
#include <string>
#include "data.h"

namespace soil::video::mesh {
    class Prefab {
    public:
        enum class Type : std::uint8_t { Line = 0, Quad, Box };

        struct Definition {
            std::string Identifier{};
            Prefab::Type Type{Type::Quad};
            [[nodiscard]] std::string TypeString() const;
            [[nodiscard]] std::string ToString() const;
        };

        static Data *New(Type type);
        Prefab() = delete;
        ~Prefab() = delete;
    };
} // namespace soil::video::mesh
#endif
