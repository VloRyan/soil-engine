
#ifndef INSTANCING_SHADER_H
#define INSTANCING_SHADER_H
#include <string>

#include "stage/scene/viewer/node.h"
#include "stage/stage.h"
#include "video/shader/shader.h"

namespace soil_samples::instancing {
    class Shader final : public soil::video::shader::Shader {
    public:
        explicit Shader(const std::string& path);
        ~Shader() override = default;
        inline static const std::string NAME = "Instancing";
        void SetViewer(soil::stage::scene::viewer::Node* viewer);

        void Prepare(soil::video::render::State& state) override;
        void SetTextures(const std::vector<soil::video::texture::Texture*>& textures);

    private:
        soil::stage::scene::viewer::Node* viewer_;
        std::vector<soil::video::texture::Texture*> textures_;
    };
} // namespace soil_samples::instancing


#endif
