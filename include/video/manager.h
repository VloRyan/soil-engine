#ifndef ENGINE_ENGINE_VIDEO_MANAGER_H_
#define ENGINE_ENGINE_VIDEO_MANAGER_H_

#include "adapter.h"
#include "viewer.h"
#include "window.h"
#include "mesh/cache.h"
#include "render/state.h"
#include "shader/cache.h"
#include "texture/manager.h"
#include "video/buffer/uniform_buffer_object.h"


#define UBO_TARGET_TEXTURES 2

namespace video {
    namespace constants {
        static constexpr auto RED = glm::vec3(1.0, 0.0, 0.0);
        static constexpr auto GREEN = glm::vec3(0.0, 1.0, 0.0);
        static constexpr auto BLUE = glm::vec3(0.0, 0.0, 1.0);
        static constexpr auto YELLOW = glm::vec3(1.0, 1.0, 0.0);
        static constexpr auto WHITE = glm::vec3(1.0F);
        static constexpr auto GREY = glm::vec3(0.5F);
        static constexpr auto DARK_GREY = glm::vec3(0.3F);
        static constexpr auto LIGHT_GREY = glm::vec3(0.8F);
        static constexpr auto BLACK = glm::vec3(0.0F);
        static constexpr auto ORANGE = glm::vec3(1.0F, 0.647F, 0.0F);
    }

    namespace render {
        class AbstractRenderer;
    }

    class Manager {
    public:
        Manager();

        virtual ~Manager();

        void Init(engine::Window *window);

        void Update();

        void BeginRender();

        void EndRender() const;

        [[nodiscard]] mesh::Mesh* GetMesh(const mesh::Definition &definition);

        shader::Shader* GetShader(const std::string &name);

        shader::InstanceShader* GetIShader(const std::string &name);

        void PrepareShader(const std::string &name, const std::string &path);

        void PrepareIShader(shader::InstanceShader *shader);

        void PrepareShader(shader::Shader *shader);

        texture::Manager &Texture();

        render::State &GetState();

        void NewUniformBufferObject(const std::string &name, gl_size_t size, int target);

    protected:
        void initState();

    private:
        engine::Window *window_;
        Adapter adapter;
        render::State state_;
        mesh::Cache meshCache_;
        int nextMeshId;
        shader::Cache shaderCache_;
        texture::Manager textureManager_;

        void renderToScreen(const std::vector<texture::Texture *> &textures);

        static void debugOutput(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const char *message,
                                const void *userParam);
    };
} // video

#endif //ENGINE_ENGINE_VIDEO_MANAGER_H_
