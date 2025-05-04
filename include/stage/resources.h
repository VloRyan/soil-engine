
#ifndef ENGINE_STAGE_RESOURCES_H
#define ENGINE_STAGE_RESOURCES_H
#include "sound/listener.h"
#include "sound/source.h"
#include "video/mesh/mesh.h"
#include "video/shader/instance_shader.h"
#include "video/texture/manager.h"
#include "window.h"

class Resources
{
public:
    virtual ~Resources() = default;

    Resources(const Resources &other) = default;

    Resources(Resources &&other) noexcept = default;

    Resources &operator=(const Resources &other) = default;

    Resources &operator=(Resources &&other) noexcept = default;

    [[nodiscard]] virtual video::mesh::Mesh *GetMesh(const video::mesh::Definition &definition) const = 0;

    [[nodiscard]] virtual video::shader::InstanceShader *GetShader(const std::string &name) const = 0;

    [[nodiscard]] virtual sound::Source *GetSource(const std::string &name) const = 0;

    [[nodiscard]] virtual sound::Buffer *GetSoundBuffer(const std::string &name) const = 0;

    [[nodiscard]] virtual sound::Listener *GetListener() const = 0;

    [[nodiscard]] virtual engine::Window *GetWindow() const = 0;

    [[nodiscard]] virtual video::texture::Texture *GetTexture2D(const std::string &fileName,
                                                                const video::texture::Parameter &parameter) = 0;

    [[nodiscard]] virtual video::texture::Texture *GetTextureArray2D(const std::string &fileName, int tilesPerDim,
                                                                     const video::texture::Parameter &parameter) = 0;

protected:
    Resources() = default;
};


#endif // ENGINE_STAGE_RESOURCES_H
