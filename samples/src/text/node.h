#ifndef TEXT_NODE_H
#define TEXT_NODE_H
#include "file/font.h"
#include "letter_component.h"
#include "stage/scene/node.h"
#include "video/manager.h"
#include "video/model/letter.h"
#include "video/texture/texture.h"

namespace soil_samples::text {
    class Node : public soil::stage::scene::Node {
    public:
        explicit Node(soil::file::Font* font, const std::string& text = "");
        ~Node() override = default;

        [[nodiscard]] virtual std::string GetText() const;
        virtual void SetText(const std::string& text);
        [[nodiscard]] glm::vec2 GetSize() const;
        virtual void SetBorderOutline(const glm::vec2& borderOutline);
        [[nodiscard]] glm::vec2 GetBorderOutline() const;
        [[nodiscard]] glm::vec2 GetLetterOutline() const;
        virtual void SetLetterOutline(const glm::vec2& letterOutline);
        virtual void SetLetterSize(float Size);
        [[nodiscard]] virtual float GetLetterSize() const;

        void SetColor(glm::vec4 color);
        [[nodiscard]] glm::vec4 GetColor() const;

        void SetBorderColor(glm::vec3 color);
        [[nodiscard]] glm::vec3 GetBorderColor() const;

        [[nodiscard]] soil::file::Font* getFont() const;

        void SetVisible(bool visible);
        [[nodiscard]] bool IsVisible() const;
        [[nodiscard]] virtual glm::vec2 GetMaxSize() const;
        virtual void SetMaxSize(const glm::vec2& max_size);

    protected:
        void UpdateDirty() override;

        struct Word {
            std::vector<const soil::video::model::Letter*> Letters;
            std::string Text;
            float Length;
            void Append(const soil::video::model::Letter* letter, float size);
        };

        struct Line {
            std::vector<Word> Words;
            std::string Text;
            float Length;
            void Append(const Word& word);
        };


    private:
        void updateText();
        void createInstance(const soil::video::model::Letter* letter, glm::vec2 offset);
        soil::file::Font* font_;
        glm::vec2 size_;

        glm::vec2 letterOutline_;
        glm::vec2 borderOutline_;

        std::string text_;
        float letterSize_;
        glm::vec2 maxSize_;
        glm::vec4 color_;
        glm::vec3 borderColor_;

        std::vector<LetterComponent*> letterInstances_;
        bool visible_;
    };
} // namespace soil_samples::text

#endif
