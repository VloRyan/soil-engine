#ifndef SOIL_SOUND_MANAGER
#define SOIL_SOUND_MANAGER
#include <cstdint>

#include "buffer.h"
#include "listener.h"
#include "source.h"

namespace soil::sound {
    class Manager {
    public:
        enum class DistanceModel : std::uint8_t {
            INVERSE_DISTANCE,
            INVERSE_DISTANCE_CLAMPED,
            LINEAR_DISTANCE,
            LINEAR_DISTANCE_CLAMPED,
            EXPONENT_DISTANCE,
            EXPONENT_DISTANCE_CLAMPED,
            NONE
        };
        Manager() = default;
        virtual ~Manager() = default;
        virtual void Init() = 0;
        [[nodiscard]] virtual Source *GetSource(const std::string &fileName) = 0;
        [[nodiscard]] virtual Buffer *GetBuffer(const std::string &fileName) = 0;
        [[nodiscard]] virtual Listener *GetListener() const = 0;
    };


} // namespace soil::sound

#endif // SOIL_SOUND_MANAGER
