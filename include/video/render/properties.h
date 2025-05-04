#ifndef ENGINE_ENGINE_VIDEO_RENDER_PROPERTIES_H_
#define ENGINE_ENGINE_VIDEO_RENDER_PROPERTIES_H_
#include <array>

#include "base.h"
#include <glm/glm.hpp>

#define MAX_CLIPPING_PLANES 2U

namespace video::render {
    class Properties {
    public:
        Properties();

        virtual ~Properties();

        [[nodiscard]] bool isClippingPlaneEnabled(int index) const;

        [[nodiscard]] bool isWireFrame() const;

        [[nodiscard]] glm::vec4 GetClippingPlane(int index) const;

        void SetClippingPlane(int index, glm::vec4 plane);

    private:
        std::array<glm::vec4, MAX_CLIPPING_PLANES> clippingPlanes_;
        uint shadowMapSize_;
        bool wireFrame_;
    };
} // Render

#endif //ENGINE_ENGINE_VIDEO_RENDER_PROPERTIES_H_
