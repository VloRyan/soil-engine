#ifndef ENGINE_VIDEO_SHADER_CACHE_H_
#define ENGINE_VIDEO_SHADER_CACHE_H_

#include "../../base.h"
#include "instance_shader.h"
#include <functional>
#include <string>

namespace video::shader {
    class Cache final {
    public:
        Cache() = default;

        ~Cache();

        [[nodiscard]] Shader* GetByName(const std::string &name);

        [[nodiscard]] InstanceShader* GetByName2(const std::string &name);

        [[nodiscard]] Shader* GetById(int id) const;

        void Prepare(const std::string &name, const std::string &path = "");

        void Prepare(const std::string &name, Shader *shader);

        void PrepareInstanceShader(const std::string &name, InstanceShader *shader);

        [[nodiscard]] Shader* Create(const std::string &name, const Definition &shaderDef);

        void ForEach(const std::function<void(Shader *)> &callback) const;

        Cache(Cache const &) = delete;

        void operator=(Cache const &) = delete;

    private:
        HashMap<std::string, Shader *> shaders_;
        HashMap<std::string, InstanceShader *> instanceShaders_;
    };
} // video::shader

#endif //ENGINE_VIDEO_SHADER_CACHE_H_
