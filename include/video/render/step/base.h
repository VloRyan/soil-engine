#ifndef SOIL_VIDEO_RENDER_STEP_BASE_H
#define SOIL_VIDEO_RENDER_STEP_BASE_H
#include <string>
#include "context.hpp"

namespace soil::video::render::step {
    extern const std::string OPAQUE_RENDERABLES;
    extern const std::string NON_OPAQUE_RENDERABLES;

    class Base {
        friend class Pipeline;

    public:
        explicit Base(std::string id, Base *requiredStep = nullptr);

        virtual ~Base() = default;

        virtual void Process(Context &context) = 0;

        [[nodiscard]] Base *GetRequiredStep() const;

        std::string GetId();

    protected:
        Base *requiredStep_;

    private:
        std::string id_;
    };
} // namespace soil::video::render::step
#endif /* SOIL_VIDEO_RENDER_STEP_BASE_H */
