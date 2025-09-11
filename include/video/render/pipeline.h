#ifndef SOIL_VIDEO_RENDER_PIPELINE_H_
#define SOIL_VIDEO_RENDER_PIPELINE_H_

#include <vector>

#include "container.h"
#include "video/render/step/base.h"

namespace soil::video::render {
    class Pipeline final {
    public:
        explicit Pipeline(std::string name, Container* container);

        ~Pipeline();

        void Run(State& state);

        void Clear();

        void InsertStep(step::Base* step);

        void RemoveStep(const step::Base* step);

        void Print() const;

        [[nodiscard]] bool Empty() const;

        [[nodiscard]] step::Base* GetStep(const std::string& id) const;
        [[nodiscard]] std::vector<step::Base*>& GetSteps();

        void SetName(std::string Name);

        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] step::Context& GetContext();

        static Pipeline* NewForwardRenderingPipeline(Container* container, bool depthTest = true,
                                                     const std::string& name = "");

    protected:
        void find(std::vector<step::Base*>::iterator& itr, const step::Base* step);

    private:
        std::string name_;
        std::vector<step::Base*> steps_;
        step::Context context_;
    };
} // namespace soil::video::render

#endif
