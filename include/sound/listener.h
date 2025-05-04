#ifndef ENGINE_SOUND_LISTENER_H
#define ENGINE_SOUND_LISTENER_H
#include "glm/glm.hpp"

namespace sound {
    class Listener {
    public:
        Listener();

        virtual ~Listener() = default;

        void SetOrientation(const glm::mat2x3 &orientation);

        [[nodiscard]] glm::mat2x3 getOrientation() const;

        void setVelocity(glm::vec3 velocity);

        [[nodiscard]] glm::vec3 getVelocity() const;

        void SetPosition(glm::vec3 position);

        [[nodiscard]] glm::vec3 getPosition() const;

        void setGain(float gain);

        [[nodiscard]] float getGain() const;

    private:
        float gain_;
        glm::vec3 position_;
        glm::vec3 velocity_;
        glm::mat2x3 orientation_;
    };
}

#endif /* ENGINE_SOUND_LISTENER_H */

