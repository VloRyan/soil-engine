#ifndef SOIL_VIDEO_RENDER_PIPELINE_H_
#define SOIL_VIDEO_RENDER_PIPELINE_H_

#include <vector>

#include "renderable.h"
#include "technique.h"
#include "video/buffer/fb.h"
#include "video/render/processing/abstract_processing.h"
#include "video/render/processing/render_step.h"


namespace soil::video::render {
    class Pipeline final {
    public:
        explicit Pipeline(std::string name);

        ~Pipeline();

        void Render(const std::vector<Renderable *> &renderables, State &state);

        void Reset(bool multisample);

        void Clear();

        void SetRenderingTechnique(TechniqueType type);

        [[nodiscard]] Technique *GetRenderingTechnique() const;

        void InsertStep(AbstractProcessing *step, AbstractProcessing *requiredStep = nullptr);

        void RemoveStep(const AbstractProcessing *step);

        void Print() const;

        [[nodiscard]] bool Empty() const;

        [[nodiscard]] AbstractProcessing *GetStep(uint number);

        [[nodiscard]] AbstractProcessing *GetStep(const std::string &name) const;

        [[nodiscard]] buffer::FrameBuffer *GetOutputBuffer() const;

        void SetOutputBuffer(buffer::FrameBuffer *buffer);

        [[nodiscard]] bool IsRenderToScreen() const;

        void SetRenderToScreen(bool RenderToScreen);

        void SetName(std::string Name);

        [[nodiscard]] std::string GetName() const;

    protected:
        void find(std::vector<AbstractProcessing *>::iterator &itr, const AbstractProcessing *step);

    private:
        std::string name_;
        bool renderToScreen_;

        RenderStep *renderSceneToTextureStep_;

        /* Stage Texture */
        buffer::FrameBuffer *multisampleSceneTextureFrameBuffer_;
        buffer::FrameBuffer *sceneTextureFrameBuffer_;

        buffer::FrameBuffer *outputBuffer_;
        std::vector<AbstractProcessing *> processingSteps_;
        Context context_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_PIPELINE_H_
