#include "video/model/material.h"

namespace video::model {
Material::Material()
    : diffuse_(1.0F), specular_(0.5F), shininess_(5.0F), opacity_(1.0F),
      diffuseTextureCount_(0), normalTextureCount_(0),
      specularTextureCount_(0) {}

const std::string &Material::GetName() const { return name_; }

void Material::SetName(const std::string &name) { name_ = name; }

const glm::vec3 &Material::GetDiffuse() const { return diffuse_; }

void Material::SetDiffuse(const glm::vec3 &diffuse) { diffuse_ = diffuse; }

const glm::vec3 &Material::GetSpecular() const { return specular_; }

void Material::SetSpecular(const glm::vec3 &specular) { specular_ = specular; }

float Material::GetShininess() const { return shininess_; }

void Material::SetShininess(const float shininess) { shininess_ = shininess; }

float Material::GetOpacity() const { return opacity_; }

void Material::SetOpacity(const float opacity) { opacity_ = opacity; }

uint Material::GetDiffuseTextureCount() const { return diffuseTextureCount_; }

void Material::SetDiffuseTextureCount(const uint diffuse_texture_count) {
  diffuseTextureCount_ = diffuse_texture_count;
}

uint Material::GetNormalTextureCount() const { return normalTextureCount_; }

void Material::SetNormalTextureCount(const uint normal_texture_count) {
  normalTextureCount_ = normal_texture_count;
}

uint Material::GetSpecularTextureCount() const { return specularTextureCount_; }

void Material::SetSpecularTextureCount(const uint specular_texture_count) {
  specularTextureCount_ = specular_texture_count;
}

const std::vector<texture::Texture *> &Material::GetTextures() const {
  return textures_;
}

void Material::SetTextures(const std::vector<texture::Texture *> &textures) {
  textures_ = textures;
}
} // namespace video::model
