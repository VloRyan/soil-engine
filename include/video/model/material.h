#ifndef ENGINE_ENGINE_VIDEO_MODEL_MATERIAL_H_
#define ENGINE_ENGINE_VIDEO_MODEL_MATERIAL_H_

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "base.h"
#include "video/texture/texture.h"

#define MATERIAL_MAX_TEXTURES 13

namespace video::model {
    class Material {
    public:
        Material();

        [[nodiscard]] const std::string &GetName() const;

        void SetName(const std::string &name);

        [[nodiscard]] const glm::vec3 &GetDiffuse() const;

        void SetDiffuse(const glm::vec3 &diffuse);

        [[nodiscard]] const glm::vec3 &GetSpecular() const;

        void SetSpecular(const glm::vec3 &specular);

        [[nodiscard]] float GetShininess() const;

        void SetShininess(float shininess);

        [[nodiscard]] float GetOpacity() const;

        void SetOpacity(float opacity);

        [[nodiscard]] uint GetDiffuseTextureCount() const;

        void SetDiffuseTextureCount(uint diffuse_texture_count);

        [[nodiscard]] uint GetNormalTextureCount() const;

        void SetNormalTextureCount(uint normal_texture_count);

        [[nodiscard]] uint GetSpecularTextureCount() const;

        void SetSpecularTextureCount(uint specular_texture_count);

        [[nodiscard]] const std::vector<texture::Texture *> &GetTextures() const;

        void SetTextures(const std::vector<texture::Texture *> &textures);

    private:
        std::string name_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        /** Range: 0.0f - 128.0f */
        float shininess_; //
        /** Range: 0.0f - 1.0f. 0.0f means invisible*/
        float opacity_;
        uint diffuseTextureCount_;
        uint normalTextureCount_;
        uint specularTextureCount_;
        std::vector<texture::Texture *> textures_;
    };
} // model

#endif //ENGINE_ENGINE_VIDEO_MODEL_MATERIAL_H_
