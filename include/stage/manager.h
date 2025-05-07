#ifndef SOIL_STAGE_MANAGER_H_
#define SOIL_STAGE_MANAGER_H_

#include "input/event.h"
#include "input/manager.h"
#include "resources.h"

/*namespace soil {
    class Window;
}

namespace soil::input {
    class Manager;
}

namespace soil::stage::scene {
    class Scene;
}
*/
namespace soil::stage {
    class Stage;

    class Manager final : public input::EventHandler, public WindowEventHandler {
    public:
        Manager();

        ~Manager() override;

        int AddStage(Stage *stage);

        void Init(Window *window, input::Manager *inputManager, Resources *resources);

        void Update() const;

        void Render(video::render::State &state) const;

        void Handle(const input::Event &event) override;

        void Handle(const WindowEvent &event) override;
        [[nodiscard]] Resources &GetResources() const;

    private:
        std::vector<Stage *> stages_;
        int currentStage_;
        std::function<const void()> deregisterFun;
        Resources *resources_;
        Window *window_;
        input::Manager *inputManager_;
        int nextStageId_;
    };
} // namespace soil::stage

#endif // SOIL_STAGE_MANAGER_H_
