#ifndef SOIL_VIDEO_MODEL_CHARACTER_H_
#define SOIL_VIDEO_MODEL_CHARACTER_H_

#include <glm/glm.hpp>

namespace soil::video::model {
    struct Letter {
        uint Id;
        glm::vec2 TextureCoord;
        glm::vec2 MaxTextureCoord;
        glm::vec2 Offset;
        glm::vec2 Size;
        float XAdvance;

        [[nodiscard]] bool isPrintable() const;
    };
} // namespace soil::video::model

#endif // SOIL_VIDEO_MODEL_CHARACTER_H_
