
#ifndef SHAPE_TILE_INSTANCE_SHADER_H
#define SHAPE_TILE_INSTANCE_SHADER_H
#include <string>

#include "stage/scene/viewer/node.h"
#include "video/shader/instance_shader.h"

namespace soil_samples::gui {

    class ShapeTileInstanceShader final : public soil::video::shader::Shader {
    public:
        explicit ShapeTileInstanceShader(const std::string& path);
        void Prepare(soil::video::render::State& state) override;
        void SetViewer(soil::stage::scene::viewer::Node* viewer);

        inline static const auto NAME = std::string("ShapeTileInstance");

    private:
        soil::stage::scene::viewer::Node* viewer_;
    };
} // namespace soil_samples::gui


#endif
