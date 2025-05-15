#ifndef SOIL_VIDEO_RENDER_PIPELINE_H_
#define SOIL_VIDEO_RENDER_PIPELINE_H_

#include <vector>

#include "container.h"
#include "video/buffer/fb.h"
#include "video/render/step/base.h"

namespace soil::video::render {
    class Pipeline final {
    public:
        explicit Pipeline(std::string name, Container *container);

        ~Pipeline();

        void Run(State &state);

        void Clear();

        void InsertStep(step::Base *step);

        void RemoveStep(const step::Base *step);

        void Print() const;

        [[nodiscard]] bool Empty() const;

        [[nodiscard]] step::Base *GetStep(uint number);

        [[nodiscard]] step::Base *GetStep(const std::string &name) const;

        [[nodiscard]] buffer::FrameBuffer *GetOutputBuffer() const;

        void SetOutputBuffer(buffer::FrameBuffer *buffer);

        [[nodiscard]] bool IsRenderToScreen() const;

        void SetRenderToScreen(bool RenderToScreen);

        void SetName(std::string Name);

        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] step::Context &GetContext();

    protected:
        void find(std::vector<step::Base *>::iterator &itr, const step::Base *step);

    private:
        std::string name_;
        bool renderToScreen_;
        Container *container_;


        /* Stage Texture */
        buffer::FrameBuffer *multisampleSceneTextureFrameBuffer_;
        buffer::FrameBuffer *sceneTextureFrameBuffer_;

        buffer::FrameBuffer *outputBuffer_;
        std::vector<step::Base *> processingSteps_;
        step::Context context_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_PIPELINE_H_
