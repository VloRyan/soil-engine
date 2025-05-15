
#ifndef SOIL_STAGE_STAGE_H
#define SOIL_STAGE_STAGE_H
#include <video/buffer/fb.h>

#include "input/event.h"
#include "resources.h"
#include "scene/scene.h"
#include "video/render/quad.h"
#include "video/render/state.h"
#include "window_event.h"

namespace soil::stage {
    namespace scene {
        class Scene;
    }

    class Manager;

    class Stage : public input::EventHandler, public WindowEventHandler {
    public:
        explicit Stage(Manager *manager);

        ~Stage() override;

        virtual void Update();

        virtual void Render(video::render::State &state);

        int AddScene(scene::Scene *scene);

        void Handle(const input::Event &event) override;
        void Handle(const WindowEvent &event) override;

        [[nodiscard]] Resources &GetResources() const;

        [[nodiscard]] video::buffer::FrameBuffer *GetFrameBuffer() const;

    protected:
        void SetFrameBuffer(video::buffer::FrameBuffer *frameBuffer, video::render::Quad *quad = nullptr);

        [[nodiscard]] video::render::Quad *GetQuad() const;

    private:
        int id_;
        std::vector<scene::Scene *> scenes_;
        Resources *resources_;
        int nextSceneId_;
        video::buffer::FrameBuffer *frameBuffer_;
        video::render::Quad *quad_;
    };
} // namespace soil::stage

#endif // SOIL_STAGE_STAGE_H
