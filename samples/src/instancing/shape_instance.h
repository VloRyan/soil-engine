#ifndef INSTANCING_SHAPE_INSTANCE_H
#define INSTANCING_SHAPE_INSTANCE_H
#include <stage/scene/component/instance_data.h>
#include <vector>

#include "video/vertex/vertex.h"

namespace soil_samples::instancing {
    class ShapeInstance : public soil::stage::scene::component::InstanceData {
    public:
        struct Data {
            glm::mat4 Matrix{1.F};
            glm::vec2 Size{1.F};
            glm::vec4 Color{1.F};
            uint TextureIndex{0};
        };

        explicit ShapeInstance(bool isOpaque);

        ~ShapeInstance() override = default;

        [[nodiscard]] virtual uint GetTextureIndex() const;
        virtual void SetTextureIndex(uint index);
        [[nodiscard]] virtual glm::vec2 GetSize() const;
        virtual void SetSize(const glm::vec2& size);
        [[nodiscard]] virtual glm::vec4 GetColor() const;
        virtual void SetColor(const glm::vec4& color);

        static std::vector<soil::video::vertex::VertexAttribDescriptor> ATTRIBS;
        static inline std::string BATCH_NAME = "Shape";
        void UpdateTransform(const glm::mat4& matrix) override;

    protected:
        void WriteData(soil::video::buffer::Cursor* cursor) const override;

    private:
        Data data_;
    };
} // namespace soil_samples::instancing

#endif
