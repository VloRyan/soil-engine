#ifndef SOIL_VIDEO_RENDER_STEP_SORT_H
#define SOIL_VIDEO_RENDER_STEP_SORT_H
#include "base.h"

namespace soil::video::render::step {
    class Sort : public Base {
    public:
        enum class Order : std::int8_t {
            FrontToBack = 0,
            BackToFront,
        };
        explicit Sort(const std::string& id, Order order = Order::FrontToBack, Base* requiredStep = nullptr);
        ~Sort() override = default;
        void Process(Context& context) override;
        [[nodiscard]] virtual Order GetOrder() const;

    private:
        static void sortFrontToBack(const glm::vec3& refPoint, std::vector<RenderableObject*>& renderables);
        static void sortBackToFront(const glm::vec3& refPoint, std::vector<RenderableObject*>& renderables);

        Order order_;
    };
} // namespace soil::video::render::step


#endif
