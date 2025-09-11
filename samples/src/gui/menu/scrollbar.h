#ifndef SCROLLBAR_H
#define SCROLLBAR_H
#include "file/sprite_sheet.h"
#include "gui/plane.h"

namespace soil_samples::gui::menu {
    class Scrollbar final : public Plane {
    public:
        struct ScrollerDefinition {
            std ::string TileName{};
            Plane::Style Style{};
        };

        struct Definition {
            soil::file::SpriteSheet* SpriteSheet{nullptr};
            std ::string BackgroundTileName{};
            ScrollerDefinition Scroller{};
        };
        enum class Direction : std::int8_t {
            Up = -1,
            Down = 1,
        };

        explicit Scrollbar(const Definition& definition);
        ~Scrollbar() override = default;

        void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                           soil::input::Event::State state) override;
        [[nodiscard]] float GetValue() const;
        void SetOnValueChange(const std::function<void(float)>& onValueChange);
        void SetValue(float value);
        void UpdateSize(const glm::ivec2& parentSize) override;
        [[nodiscard]] float GetMinValue() const;
        void SetMinValue(float minValue);
        [[nodiscard]] float GetMaxValue() const;
        void SetMaxValue(float maxValue);
        [[nodiscard]] float GetIncrement() const;
        void SetIncrement(float increment);

        void Scroll(Direction direction = Direction::Down);
        void SetScissorFrom(Rectangle* rectangle) override;

    private:
        void UpdateScrollerPosition() const;
        Plane* scroller_;
        float value_;
        float minValue_;
        float maxValue_;
        float increment_;
        std::function<void(float)> onValueChange_;
    };
} // namespace soil_samples::gui::menu


#endif
