
#ifndef SHAPE_TILE_SHADER_H
#define SHAPE_TILE_SHADER_H
#include <string>

#include "stage/scene/viewer/node.h"
#include "video/shader/instance_shader.h"

namespace soil_samples::gui {

    class ShapeTileShader final : public soil::video::shader::Shader {
    public:
        explicit ShapeTileShader(const std::string& path);
        void Prepare(soil::video::render::State& state) override;
        void SetViewer(soil::stage::scene::viewer::Node* viewer);

        inline static const auto NAME = std::string("ShapeTile");

    private:
        soil::stage::scene::viewer::Node* viewer_;
    };
} // namespace soil_samples::gui


#endif
