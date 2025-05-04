#include "input/mouse/picker.h"

namespace input::mouse
{
    Picker::Picker() = default;

    Picker::~Picker() = default;

    glm::vec3 Picker::CalculateRay(const glm::uvec2 &mousePos, const glm::uvec2 &windowSize,
                                   const stage::scene::viewer::Node *viewer)
    {
        const auto nds = getNormalisedDeviceCoordinates(mousePos, windowSize);
        const auto clipCoords = glm::vec4(nds, -1.0F, 1.0F);
        const auto eyeCoords = toEyeCoords(clipCoords, viewer->GetProjectionMatrix());
        return toWorldCoords(eyeCoords, viewer->GetViewMatrix());
    }

    glm::vec2 Picker::getNormalisedDeviceCoordinates(const glm::uvec2 &mousePos, const glm::uvec2 &windowSize)
    {
        glm::vec2 nds;
        nds.x = (2.0F * mousePos.x) / windowSize.x - 1.0F;
        nds.y = 1.0F - (2.0F * mousePos.y) / windowSize.y;
        return nds;
    }

    glm::vec4 Picker::toEyeCoords(const glm::vec4 &clipCoords, const glm::mat4 &projMatrix)
    {
        const auto invertedProjection = glm::inverse(projMatrix);
        auto eyeCoords = invertedProjection * clipCoords;
        return {eyeCoords.x, eyeCoords.y, -1.0F, 0.0F};
    }

    glm::vec3 Picker::toWorldCoords(const glm::vec4 &eyeCoords, const glm::mat4 &viewMatrix)
    {
        const auto invertedView = glm::inverse(viewMatrix);
        const auto rayWorld = invertedView * eyeCoords;
        return normalize(rayWorld);
    }
} // namespace input::mouse
