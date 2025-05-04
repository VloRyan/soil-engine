
#ifndef ENGINE_VIDEO_VIEWER_H
#define ENGINE_VIDEO_VIEWER_H
#include <glm/fwd.hpp>

#include "render/state.h"

namespace video {
    class Viewer {
    public:
        virtual ~Viewer() = default;

        [[nodiscard]] virtual glm::mat4 GetViewMatrix() const = 0;

        [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

        virtual void Render(render::State &state) = 0;
    };
}
#endif //ENGINE_VIDEO_VIEWER_H
