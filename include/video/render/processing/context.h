#ifndef SOIL_VIDEO_RENDER_PROCESSING_CONTEXT_H_
#define SOIL_VIDEO_RENDER_PROCESSING_CONTEXT_H_
#include "video/render/properties.h"
#include "video/render/state.h"
#include "video/render/technique.h"

namespace soil::video::render {
    class Context {
    public:
        Context();

        [[nodiscard]] Properties *GetProperties() const;

        void SetProperties(Properties *properties);

        [[nodiscard]] Technique *GetTechnique() const;

        void SetTechnique(Technique *algorithm);

        [[nodiscard]] State &GetState() const;

        void SetState(State &state);

    private:
        Properties *properties_;
        Technique *technique_;
        State *state_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_PROCESSING_CONTEXT_H_
