
#ifndef CHARACTER_SHADER_H
#define CHARACTER_SHADER_H
#include <string>

#include "stage/scene/viewer/node.h"
#include "video/shader/instance_shader.h"

namespace soil_samples::gui {

    class CharacterShader final : public soil::video::shader::Shader {
    public:
        explicit CharacterShader(const std::string& path);
        void Prepare(soil::video::render::State& state) override;
        void SetViewer(soil::stage::scene::viewer::Node* viewer);

        inline static const auto NAME = std::string("Character");

    private:
        soil::stage::scene::viewer::Node* viewer_;
    };
} // namespace soil_samples::gui


#endif
