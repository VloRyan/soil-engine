#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H
#include <stage/resources.h>
#include <stage/scene/component/mesh_component.h>

namespace soil_samples::basic {

    class Shape : public soil::stage::scene::component::MeshComponent {
    public:
        Shape(const soil::video::mesh::Data& mesh, bool isOpaque, soil::video::shader::Shader* shader);
        ~Shape() override = default;
        [[nodiscard]] virtual soil::video::texture::Texture* GetTexture() const;
        virtual void SetTexture(soil::video::texture::Texture* texture);
        [[nodiscard]] virtual glm::vec2 GetSize() const;
        virtual void SetSize(const glm::vec2& size);
        [[nodiscard]] virtual glm::vec4 GetColor() const;
        virtual void SetColor(const glm::vec4& color);
        static Shape* New(const soil::stage::Resources& resources, bool isOpaque, soil::video::shader::Shader* shader);

    protected:
        void PrepareRender() override;

    private:
        soil::video::texture::Texture* texture_;
        glm::vec2 size_;
        glm::vec4 color_;
    };
} // namespace soil_samples::basic


#endif // BASIC_SHAPE_H
