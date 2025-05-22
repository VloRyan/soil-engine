#ifndef SOIL_VIDEO_RENDER_PIPELINE_H_
#define SOIL_VIDEO_RENDER_PIPELINE_H_

#include <vector>

#include "container.h"
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

        void SetName(std::string Name);

        [[nodiscard]] std::string GetName() const;
        [[nodiscard]] step::Context &GetContext();

    protected:
        void find(std::vector<step::Base *>::iterator &itr, const step::Base *step);

    private:
        std::string name_;
        Container *container_;
        std::vector<step::Base *> processingSteps_;
        step::Context context_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_PIPELINE_H_
