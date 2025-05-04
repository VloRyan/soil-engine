#ifndef ENGINE_ENGINE_STAGE_MANAGER_H_
#define ENGINE_ENGINE_STAGE_MANAGER_H_

#include "resources.h"
#include "input/event.h"
#include "sound/manager.h"
#include "sound/source.h"
#include "video/manager.h"

namespace engine {
    class Window;
}

namespace input {
    class Manager;
}

namespace stage::scene {
    class Scene;
}

namespace stage {
    class Stage;

    class Manager : public Resources, public input::EventHandler, public engine::WindowEventHandler {
    public :
        Manager(video::Manager *videoManager, sound::Manager *soundManager, input::Manager *inputManager);

        ~Manager() override;


        int AddStage(Stage *stage);

        void Init(engine::Window *window);

        void Update() const;

        void Render(video::render::State &state) const;

        void Handle(const input::Event &event) override;

        void Handle(const engine::WindowEvent &event) override;

        [[nodiscard]] video::mesh::Mesh* GetMesh(const video::mesh::Definition &definition) const override;

        [[nodiscard]] video::shader::InstanceShader* GetShader(const std::string &name) const override;

        [[nodiscard]] sound::Source* GetSource(const std::string &name) const override;

        [[nodiscard]] sound::Buffer* GetSoundBuffer(const std::string &name) const override;

        [[nodiscard]] sound::Listener* GetListener() const override;

        [[nodiscard]] video::texture::Texture* GetTexture2D(const std::string &fileName,
                                                            const video::texture::Parameter &parameter) override;

        [[nodiscard]] video::texture::Texture* GetTextureArray2D(const std::string &fileName, int tilesPerDim,
                                                                 const video::texture::Parameter &parameter) override;

        [[nodiscard]] engine::Window* GetWindow() const override;

    private :
        std::vector<Stage *> stages_;
        int currentStage_;
        std::function<const void()> deregisterFun;
        video::Manager *videoManager_;
        sound::Manager *soundManager_;
        input::Manager *inputManager_;
        engine::Window *window_;
        int nextStageId_;
    };
} // stage

#endif //ENGINE_ENGINE_STAGE_MANAGER_H_
