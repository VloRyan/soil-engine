#include "shape_tile.h"

#include "stage/scene/gui/rectangle.h"
#include "stage/scene/node.h"

namespace soil_samples::gui::component {
    std::unordered_map<std::string, ShapeTile::PrefabData> ShapeTile::PREFABS = {};

    ShapeTile::ShapeTile(const std::string& prefab, const bool isOpaque) :
        MeshComponent(*PREFABS[prefab].MeshData, PREFABS[prefab].Shader, isOpaque), data_(&PREFABS[prefab]),
        transform_(1.F), size_(0.F), color_(1.F), tileIndex_(-1), tileScale_(1.F), positionOffset_(0.F) {}

    void ShapeTile::InitPrefab(const std::string& name, const PrefabData& data) {
        PREFABS[name] = data;
    }

    void ShapeTile::PrepareRender(soil::video::render::State& state) {
        const auto* parentRect = dynamic_cast<soil::stage::scene::gui::Rectangle*>(GetParent());
        if (parentRect != nullptr) {
            state.SetScissorTest(true);
            state.SetScissor(parentRect->GetScissorRect());
        } else {
            state.SetScissorTest(false);
        }
        GetShader()->Use();
        GetShader()->SetUniform("uTransform", transform_);
        GetShader()->SetUniform("uSize", size_);
        GetShader()->SetUniform("uTileScale", tileScale_);
        GetShader()->SetUniform("uTexture", data_->Texture->GetSlot());
        GetShader()->SetUniform("uColor", color_);
        GetShader()->SetUniform("uTileIndex", tileIndex_);
    }

    float ShapeTile::DistanceTo(const glm::vec3& point) {
        return glm::distance(GetParent()->GetWorldPosition().z + positionOffset_.z, point.z);
    }

    void ShapeTile::UpdateTransform(const glm::mat4& transform) {
        transform_ = transform;
        transform_[3] += glm::vec4(positionOffset_, 0.0F);
    }

    int ShapeTile::GetTileIndex() const {
        return tileIndex_;
    }

    void ShapeTile::SetTileIndex(const int index) {
        if (tileIndex_ == index) {
            return;
        }
        tileIndex_ = index;
        SetDirty();
    }

    void ShapeTile::SetTileScale(const glm::vec2 scale) {
        if (tileScale_ == scale) {
            return;
        }
        tileScale_ = scale;
        SetDirty();
    }

    glm::vec2 ShapeTile::GetTileScale() const {
        return tileScale_;
    }

    glm::vec2 ShapeTile::GetSize() const {
        return size_;
    }

    void ShapeTile::SetSize(const glm::vec2& size) {
        if (size_ == size) {
            return;
        }
        size_ = size;
        SetDirty();
    }

    glm::vec4 ShapeTile::GetColor() const {
        return color_;
    }

    void ShapeTile::SetColor(const glm::vec4& color) {
        if (color_ == color) {
            return;
        }
        color_ = color;
        SetOpaque(color.a == 1.0F);
        SetDirty();
    }

    void ShapeTile::SetPositionOffset(glm::vec3 offset) {
        if (offset == positionOffset_) {
            return;
        }
        positionOffset_ = offset;
        SetDirty();
    }

    glm::vec3 ShapeTile::GetPositionOffset() const {
        return positionOffset_;
    }


} // namespace soil_samples::gui::component
