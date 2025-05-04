#ifndef ENGINE_ENGINE_VIDEO_RENDER_PIPELINE_H_
#define ENGINE_ENGINE_VIDEO_RENDER_PIPELINE_H_

#include <set>
#include <vector>

#include "renderable.h"
#include "technique.h"
#include "video/buffer/frame_buffer.h"
#include "video/render/processing/abstract_processing.h"
#include "video/render/processing/render_step.h"


namespace video::render {
    class Pipeline {
    public:
        explicit Pipeline(std::string name);

        virtual ~Pipeline();

        virtual void Render(const std::vector<Renderable *> &renderables, State &state);

        virtual void Reset(bool multisample, glm::ivec2 resolution);

        virtual void Clear();

        void SetRenderingTechnique(TechniqueType type);

        [[nodiscard]] Technique* GetRenderingTechnique() const;

        void InsertStep(AbstractProcessing *step, AbstractProcessing *requiredStep = nullptr);

        void RemoveStep(const AbstractProcessing *step);

        void Print();

        bool Empty() const;

        AbstractProcessing* GetStep(uint number);

        AbstractProcessing* GetStep(const std::string &name);


        //FrameBuffer_Ptr getFrameBuffer(std::string name);
        //void registerFrameBuffer(std::string name, FrameBuffer_Ptr buffer);


        [[nodiscard]] buffer::FrameBuffer* GetOutputBuffer() const;

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

        /* Render Techniques */
        //ForwardRendering* m_ForwardRendering;
        //std::unique_ptr<DeferredRendering> m_DeferredRendering;

        RenderStep *renderSceneToTextureStep_;

        /* Stage Texture */
        buffer::FrameBuffer *multisampleSceneTextureFrameBuffer_;
        buffer::FrameBuffer *sceneTextureFrameBuffer_;

        buffer::FrameBuffer *outputBuffer_;
        std::vector<AbstractProcessing *> processingSteps_;
        // HashMap<std::string, FrameBuffer_Ptr> m_NamedFrameBuffer;
        Context context_;
    };
} // Render

#endif //ENGINE_ENGINE_VIDEO_RENDER_PIPELINE_H_
