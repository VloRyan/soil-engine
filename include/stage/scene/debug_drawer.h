
#ifndef ENGINE_STAGE_SCENE_DEBUG_DRAWER_H
#define ENGINE_STAGE_SCENE_DEBUG_DRAWER_H
#include <glm/glm.hpp>


namespace stage::scene {
    class Node;

    class IDebugDrawer {
    public:
        IDebugDrawer() = default;

        IDebugDrawer(const IDebugDrawer &other) = delete;

        IDebugDrawer(IDebugDrawer &&other) noexcept = delete;

        IDebugDrawer &operator=(const IDebugDrawer &other) = delete;

        IDebugDrawer &operator=(IDebugDrawer &&other) noexcept = delete;

        virtual ~IDebugDrawer() = default;

        virtual void StartDraw() {
        };

        virtual void Line(glm::vec3 start, glm::vec3 end, glm::vec4 color) = 0;

        virtual void EndDraw() {
        };
    };
}
#endif //ENGINE_STAGE_SCENE_DEBUG_DRAWER_H
