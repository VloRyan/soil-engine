#ifndef SOIL_STAGE_MANAGER_H_
#define SOIL_STAGE_MANAGER_H_

#include "input/event.h"
#include "input/manager.h"
#include "resources.h"

namespace soil::stage {
    class Stage;

    class Manager final : public input::EventHandler, public WindowEventHandler, public IStages {
    public:
        Manager();

        ~Manager() override;

        void Init(Window* window, input::Manager* inputManager, Resources* resources);

        void RegisterStage(const std::string& name, Stage* stage);
        void SetCurrent(const std::string& name) override;
        Stage* GetCurrent() const override;
        Stage* GetStage(const std::string& name) const;
        Stage* RemoveStage(const std::string& name);

        void Update() const;

        void Render(video::render::State& state) const;

        void Handle(const input::Event& event) override;

        void Handle(const WindowEvent& event) override;
        [[nodiscard]] Resources& GetResources() const;


    private:
        std::unordered_map<std::string, Stage*> stages_;
        std::string currentStageName_;
        Stage* currentStage_;
        std::function<const void()> deregisterFun;
        Resources* resources_;
        Window* window_;
        input::Manager* inputManager_;
    };
} // namespace soil::stage

#endif
