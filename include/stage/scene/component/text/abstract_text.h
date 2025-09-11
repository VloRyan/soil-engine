#ifndef SOIL_STAGE_SCENE_COMPONENT_TEXT_ABSTRACT_TEXT_H
#define SOIL_STAGE_SCENE_COMPONENT_TEXT_ABSTRACT_TEXT_H
#include "file/font.h"
#include "stage/scene/component/mesh_component.h"
#include "stage/scene/node.h"
#include "video/manager.h"
#include "video/model/letter.h"
#include "video/texture/texture.h"

namespace soil::stage::scene::component::text {
    struct Word {
        std::vector<const file::Font::Character*> Characters {};
#ifdef DEBUG
        std::string Text {};
#endif
        int Length {0};
        void Append(const file::Font::Character* character);
    };

    struct Line {
        std::vector<Word> Words {};
#ifdef DEBUG
        std::string Text {};
#endif
        int Length {0};
        void Append(const Word& word);
    };

    class AbstractText : public MeshComponent {
    public:
        struct PrefabData {
            video::mesh::Data* MeshData {nullptr};
            video::shader::Shader* Shader {nullptr};
            const file::Font* Font {nullptr};
            video::texture::Texture* FontTexture {nullptr};
        };

        explicit AbstractText(const std::string& prefab, const std::string& text = "");
        ~AbstractText() override = default;
        static void InitPrefab(const std::string& name, const PrefabData& data);

        [[nodiscard]] virtual std::string GetText() const;
        virtual void SetText(const std::string& text);
        [[nodiscard]] glm::vec2 GetSize() const;
        virtual void SetBorderOutline(const glm::vec2& borderOutline);
        [[nodiscard]] glm::vec2 GetBorderOutline() const;
        [[nodiscard]] glm::vec2 GetCharacterOutline() const;
        virtual void SetCharacterOutline(const glm::vec2& characterOutline);
        virtual void SetCharacterSize(float size);
        [[nodiscard]] virtual float GetCharacterSize() const;

        void SetColor(glm::vec4 color);
        [[nodiscard]] glm::vec4 GetColor() const;

        void SetBorderColor(glm::vec3 color);
        [[nodiscard]] glm::vec3 GetBorderColor() const;

        [[nodiscard]] const file::Font* GetFont() const;

        [[nodiscard]] virtual glm::vec2 GetMaxSize() const;
        virtual void SetMaxSize(const glm::vec2& maxSize);

        [[nodiscard]] virtual byte GetTextureSlot() const;

        virtual void SetPositionOffset(const glm::vec3& positionOffset);
        [[nodiscard]] virtual glm::vec3 GetPositionOffset() const;

        float DistanceTo(const glm::vec3& point) override;
        void UpdateTransform(const glm::mat4& transform) override;
        void Update() override;

        void Render(video::render::State& state) override = 0;
        const std::vector<Line>& GetLines() const;

    private:
        static std::unordered_map<std::string, PrefabData> PREFABS;
        void updateText();
        PrefabData* data_;
        glm::vec2 size_;

        glm::vec3 positionOffset_;

        glm::vec2 characterOutline_;
        glm::vec2 borderOutline_;

        std::string text_;
        float characterSize_;
        glm::vec2 maxSize_;
        glm::vec4 color_;
        glm::vec3 borderColor_;

        std::vector<Line> lines_;
    };
} // namespace soil::stage::scene::component::text

#endif
