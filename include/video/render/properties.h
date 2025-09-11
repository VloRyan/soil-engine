#ifndef SOIL_VIDEO_RENDER_PROPERTIES_H_
#define SOIL_VIDEO_RENDER_PROPERTIES_H_
#include <array>

#include <glm/glm.hpp>

#define MAX_CLIPPING_PLANES 2U

namespace soil::video::render {
    class Properties final {
    public:
        Properties();

        ~Properties() = default;

        [[nodiscard]] bool isClippingPlaneEnabled(int index) const;

        [[nodiscard]] bool isWireFrame() const;

        [[nodiscard]] glm::vec4 GetClippingPlane(int index) const;

        void SetClippingPlane(int index, glm::vec4 plane);

    private:
        std::array<glm::vec4, MAX_CLIPPING_PLANES> clippingPlanes_;
        uint shadowMapSize_;
        bool wireFrame_;
    };
} // namespace soil::video::render

#endif
