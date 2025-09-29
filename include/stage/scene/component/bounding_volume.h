#ifndef SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_
#define SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_

#include <bitset>
#include <glm/glm.hpp>
#include <vector>

#include "component.h"
#include "stage/scene/volume/container.h"
#include "world/volume/volume.hpp"

namespace soil::stage::scene::component {
    struct IntersectionResult {
        bool Intersecting = false;
        glm::vec3 IntersectionPoint;
    };

    class BoundingVolume final : public Component, public world::volume::Volume {
    public:
        enum class ContactType {
            /** Ghost can only be seen but does not collide*/
            Ghost = 0,
            /** StaticObject's are non-moving objects*/
            StaticObject = 1,
            /** Object's can move*/
            Object = 2,
        };

        inline static std::vector<ContactType> ALL_CONTACT_TYPES = {ContactType::Ghost, ContactType::StaticObject,
                                                                    ContactType::Object};

        explicit BoundingVolume(Volume* volume, ContactType contactType,
                                const std::vector<ContactType>& contactTypes = {});

        ~BoundingVolume() override = default;

        void UpdateTransform(const glm::mat4& matrix) override;

        [[nodiscard]] bool CanCollideWith(ContactType type) const;

        void SetCollideWith(ContactType type, bool value);

        [[nodiscard]] ContactType GetContactType() const;

        [[nodiscard]] bool IsInside(const glm::vec3& min, const glm::vec3& max) const override;

        [[nodiscard]] bool IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const override;

        [[nodiscard]] bool IsInside(const glm::vec2& min, const glm::vec2& max) const override;

        [[nodiscard]] bool Contains(const glm::vec3& point) const override;

        [[nodiscard]] bool ContainsXZ(const glm::vec3& point) const override;

        [[nodiscard]] world::volume::IntersectionResult IntersectsRay(const glm::vec3& start,
                                                                      const glm::vec3& dir) const override;

        [[nodiscard]] world::volume::IntersectionResult IntersectsRayXZ(const glm::vec3& start,
                                                                        const glm::vec3& dir) const override;

        [[nodiscard]] bool IntersectsCircle(const glm::vec2& circleCenter, float radius) const override;

        void SetPosition(const glm::vec3& position) override;

        [[nodiscard]] glm::vec3 GetPosition() const override;

        void SetParent(Node* parent) override;

        void SetContainer(world::volume::Container* container);

        [[nodiscard]] world::volume::Container* GetContainer() const;

        [[nodiscard]] std::vector<world::volume::Line> GenerateLines() const override;

    protected:
        Volume* volume_;
        world::volume::Container* container_;
        std::bitset<3> collideWithTypes_;
        ContactType contactType_;
    };
} // namespace soil::stage::scene::component


#endif
