#ifndef SOIL_EVENT_EVENT_H
#define SOIL_EVENT_EVENT_H
#include <cstdint>

namespace soil::event {
    class Event {
    public:
        enum class Type : std::uint8_t {
            System = 0,
            Video,
            Input,
            Sound,
            Stage,
            Window,
            Game,
        };

        explicit Event(Type type);

        Event(const Event& orig);

        virtual ~Event();

        [[nodiscard]] Type GetType() const;

        friend bool operator==(const Event& lhs, const Event& rhs) {
            return lhs.type_ == rhs.type_;
        }

        friend bool operator!=(const Event& lhs, const Event& rhs) {
            return !(lhs == rhs);
        }

    private:
        Type type_;
    };
} // namespace soil::event
#endif
