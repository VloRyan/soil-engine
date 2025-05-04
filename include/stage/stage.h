
#ifndef ENGINE_STAGE_STAGE_H
#define ENGINE_STAGE_STAGE_H
#include <video/buffer/frame_buffer.h>

#include "resources.h"
#include "window_event.h"
#include "input/event.h"
#include "video/render/quad.h"
#include "video/render/state.h"


namespace stage {
    namespace scene {
        class Scene;
    }

    class Manager;

    class Stage : public input::EventHandler,
                  public engine::WindowEventHandler {
    public:
        explicit Stage(Manager *manager);

        ~Stage() override;

        virtual void Update();

        virtual void Render(video::render::State &state);

        virtual int AddScene(scene::Scene *scene);

        //int GetSceneNextId();

        void Handle(const input::Event &event) override;

        void Handle(const engine::WindowEvent &event) override;

        [[nodiscard]] Resources &GetResources() const;

        [[nodiscard]] video::buffer::FrameBuffer* GetFrameBuffer() const;

    protected:
        void SetFrameBuffer(video::buffer::FrameBuffer *frameBuffer, video::render::Quad *quad = nullptr);

        video::render::Quad* GetQuad() const;

    private:
        int id_;
        std::vector<scene::Scene *> scenes_;
        Resources *resources_;
        int nextSceneId_;
        video::buffer::FrameBuffer *frameBuffer_;
        video::render::Quad *quad_;
    };
}

#endif //ENGINE_STAGE_STAGE_H
