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

    class BoundingVolume : public Component, public world::volume::Volume {
    public:
        enum class ContactType {
            View = 0,
            Player = 1,
            World = 2,
        };
        inline static std::vector<ContactType> ALL_CONTACT_TYPES = {ContactType::View, ContactType::Player,
                                                                    ContactType::World};

        explicit BoundingVolume(Volume* volume, const std::vector<ContactType>& contactTypes = {});

        ~BoundingVolume() override = default;

        void UpdateTransform(const glm::mat4& matrix) override;

        [[nodiscard]] virtual bool IsContactType(ContactType type) const;

        virtual void SetContactType(ContactType type, bool value);

        [[nodiscard]] bool IsInside(const glm::vec3& min, const glm::vec3& max) const override;
        [[nodiscard]] bool IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const override;
        [[nodiscard]] bool IsInside(const glm::vec2& min, const glm::vec2& max) const override;

        [[nodiscard]] bool Contains(const glm::vec3& point) const override;
        [[nodiscard]] bool ContainsXZ(const glm::vec3& point) const override;

        [[nodiscard]] world::volume::IntersectionResult IntersectsRay(const glm::vec3& start,
                                                                      const glm::vec3& dir) const override;

        [[nodiscard]] world::volume::IntersectionResult IntersectsRayXZ(const glm::vec3& start,
                                                                        const glm::vec3& dir) const override;

        void SetPosition(const glm::vec3& position) override;
        [[nodiscard]] glm::vec3 GetPosition() const override;
        void SetParent(Node* parent) override;
        void SetContainer(world::volume::Container* container);
        [[nodiscard]] virtual world::volume::Container* GetContainer() const;
        [[nodiscard]] virtual std::vector<world::volume::Line> GenerateLines() const;

    protected:
        Volume* volume_;
        world::volume::Container* container_;
        std::bitset<3> contactTypes_;
    };
} // namespace soil::stage::scene::component


#endif
