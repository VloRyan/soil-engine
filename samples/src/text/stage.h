#ifndef TEXT_STAGE_H
#define TEXT_STAGE_H
#include "common/stage.h"
#include "instancing/shape_instance.h"
#include "node.h"
#include "stage/stage.h"

namespace soil_samples::text {
    class Stage final : public common::Stage {
    public:
        explicit Stage();
        ~Stage() override = default;
        void Handle(const soil::WindowEvent& event) override;
        void Update() override;
        void OnLoad() override;

    protected:
        void RegisterInputEvents(soil::input::EventMap& eventMap) override;

    private:
        void initBackground(soil::stage::scene::Scene* scene, int textureSlot);

        bool printStatistics_;
        Node* text_;
        soil::stage::scene::Node* bgNode_;
        instancing::ShapeInstance* bgShape_;
        Node* description_;
        Node* bounceText_;
        glm::vec2 bounceTextVelocity;
        float bounceTextGlowVelocity;
        int fastChangeIndex = -1;
    };
} // namespace soil_samples::text


#endif
