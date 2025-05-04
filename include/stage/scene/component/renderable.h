
#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "stage/event/component.h"
#include "video/mesh/mesh.h"
#include "video/render/renderable.h"
#include "video/render/instance/instance.h"
#include "video/shader/instance_shader.h"


namespace stage::scene::component {
    class Renderable : public Component {
    public:
        ~Renderable() override = default;

        [[nodiscard]] virtual video::mesh::Mesh* GetMesh() const = 0;

        [[nodiscard]] virtual video::shader::InstanceShader* GetShader() const = 0;


        [[nodiscard]] virtual bool IsOpaque() const = 0;

    protected:
        Renderable();
    };
}


#endif //RENDERABLE_H
