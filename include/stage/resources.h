
#ifndef SOIL_STAGE_RESOURCES_H
#define SOIL_STAGE_RESOURCES_H
#include "input/manager.h"
#include "sound/manager.h"
#include "video/manager.h"
#include "video/mesh/data.h"
#include "video/shader/instance_shader.h"

namespace soil::stage {
    class Resources {
    public:
        explicit Resources(Window* window, video::Manager* videoManager, sound::Manager* soundManager,
                           input::Manager* inputManager);
        ~Resources() = default;

        Resources(const Resources& other) = delete;

        Resources(Resources&& other) noexcept = delete;

        Resources& operator=(const Resources& other) = delete;

        Resources& operator=(Resources&& other) noexcept = delete;

        [[nodiscard]] video::mesh::Data* GetMesh(const video::mesh::Prefab::Definition& definition) const;

        [[nodiscard]] video::shader::Shader* GetShader(const std::string& name) const;

        [[nodiscard]] sound::Source* GetSource(const std::string& name) const;

        [[nodiscard]] sound::Buffer* GetSoundBuffer(const std::string& name) const;

        [[nodiscard]] sound::Listener* GetListener() const;

        [[nodiscard]] Window* GetWindow() const;

        [[nodiscard]] video::texture::Texture* GetTexture2D(const std::string& fileName,
                                                            const video::texture::Parameter& parameter = {}) const;

        [[nodiscard]] video::texture::Texture* GetTextureArray2D(const std::string& fileName, int tilesPerDim,
                                                                 const video::texture::Parameter& parameter = {}) const;

        [[nodiscard]] video::render::State& GetRenderState() const;

    private:
        video::Manager* videoManager_;
        sound::Manager* soundManager_;
        input::Manager* inputManager_;
        Window* window_;
    };
} // namespace soil::stage

#endif
