#ifndef VIDEO_RENDER_PROCESSING_CONTEXT_H_
#define VIDEO_RENDER_PROCESSING_CONTEXT_H_
#include "video/render/properties.h"
#include "video/render/state.h"
#include "video/render/technique.h"

namespace video::render {
    class Context {
    public:
        Context();

        [[nodiscard]] Properties* GetProperties() const;

        void SetProperties(Properties *properties);

        [[nodiscard]] Technique* GetTechnique() const;

        void SetTechnique(Technique *algorithm);

        [[nodiscard]] State &GetState() const;

        void SetState(State &state);

    private:
        Properties *properties_;
        Technique *technique_;
        State *state_;
    };
} // Render

#endif //VIDEO_RENDER_PROCESSING_CONTEXT_H_
