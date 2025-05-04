#ifndef ENGINE_EVENT_EVENT_H
#define ENGINE_EVENT_EVENT_H
#include <cstdint>

namespace event
{
    class Event
    {
    public:
        enum class Type : std::uint8_t
        {
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

        Type getType() const;

        void consume();

        [[nodiscard]] bool isConsumed() const;

    private:
        Type type_;
        bool consumed_;
    };
} // namespace event
#endif /* ENGINE_EVENT_EVENT_H */
