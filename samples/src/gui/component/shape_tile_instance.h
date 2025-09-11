#ifndef GUI_COMPONENT_SHAPE_TILE_INSTANCE_H
#define GUI_COMPONENT_SHAPE_TILE_INSTANCE_H
#include <stage/scene/component/instance_data.h>
#include <vector>

#include "video/vertex/vertex.h"

namespace soil_samples::gui::component {
    class ShapeTileInstance : public soil::stage::scene::component::InstanceData {
    public:
        explicit ShapeTileInstance(bool isOpaque);

        ~ShapeTileInstance() override = default;

        [[nodiscard]] virtual int GetTileIndex() const;
        virtual void SetTileIndex(int index);
        [[nodiscard]] virtual glm::vec2 GetSize() const;
        virtual void SetSize(const glm::vec2& size);
        [[nodiscard]] virtual glm::vec4 GetColor() const;
        virtual void SetColor(const glm::vec4& color);
        void SetPositionOffset(glm::vec3 offset);
        [[nodiscard]] glm::vec3 GetPositionOffset() const;

        [[nodiscard]] virtual glm::vec2 GetTileScale() const;
        virtual void SetTileScale(glm::vec2 scale);

        static std::vector<soil::video::vertex::VertexAttribDescriptor> ATTRIBS;
        static inline std::string BATCH_NAME = "ShapeTile";
        void UpdateTransform(const glm::mat4& transform) override;

    protected:
        void WriteData(soil::video::buffer::Cursor* cursor) const override;

    private:
        struct Data {
            glm::mat4 Transform {1.F};
            glm::vec2 Size {0.F};
            glm::vec4 Color {1.F};
            int TileIndex {-1};
            glm::vec2 TileScale {1.F};
        };

        Data data_;
        glm::vec3 positionOffset_;
    };
} // namespace soil_samples::gui::component

#endif
