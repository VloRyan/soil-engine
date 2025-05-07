#ifndef SOIL_INPUT_MOUSE_PICKER_H_
#define SOIL_INPUT_MOUSE_PICKER_H_

#include <glm/glm.hpp>
#include "stage/scene/viewer/node.h"

namespace soil::input::mouse {
    class Picker final {
    public:
        static glm::vec3 CalculateRay(const glm::uvec2 &mousePos, const glm::uvec2 &windowSize,
                                      const stage::scene::viewer::Node *viewer);

    private:
        Picker();

        ~Picker() = default;
        static glm::vec2 getNormalisedDeviceCoordinates(const glm::uvec2 &mousePos, const glm::uvec2 &windowSize);

        static glm::vec3 toWorldCoords(const glm::vec4 &eyeCoords, const glm::mat4 &viewMatrix);

        static glm::vec4 toEyeCoords(const glm::vec4 &clipCoords, const glm::mat4 &projMatrix);

        glm::vec3 currentRay_{};

        static constexpr int RECURSION_COUNT = 200;
        static constexpr int RAY_RANGE = 5000;
    };
} // namespace soil::input::mouse

#endif // SOIL_INPUT_MOUSE_PICKER_H_
