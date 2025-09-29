#ifndef SOIL_ENGINE_WORLD_INTERSECTION_H
#define SOIL_ENGINE_WORLD_INTERSECTION_H
#include <glm/vec2.hpp>

namespace soil::world {
    class Intersection {
    public:
        struct Box2d {
            glm::vec2 Min;
            glm::vec2 Max;
        };

        struct Circle2d {
            glm::vec2 Center;
            float Radius;
        };

        Intersection() = delete;
        ~Intersection() = default;

        static bool Test(const Box2d& box, const Circle2d& circle);
        static bool Test(const Box2d& boxA, const Box2d& boxB);
        static bool Test(const Box2d& box, const glm::vec2& point);
    };
} // namespace soil::world

#endif
