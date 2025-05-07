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

        Event(const Event &orig);

        virtual ~Event();

        [[nodiscard]] Type getType() const;

        void consume();

        [[nodiscard]] bool isConsumed() const;

    private:
        Type type_;
        bool consumed_;
    };
} // namespace soil::event
#endif /* SOIL_EVENT_EVENT_H */
