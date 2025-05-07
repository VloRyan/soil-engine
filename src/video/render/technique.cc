#include "video/render/technique.h"
#include "exception.h"
#include "video/render/forward_rendering.h"

namespace soil::video::render {
    Technique *Technique::GetTechnique(const TechniqueType type) {
        static ForwardRendering forwardRendering;
        switch (type) {
        case TechniqueType::Forward:
            return &forwardRendering;
        case TechniqueType::Deferred:
        default:
            throw Exception("unknown TechniqueType");
        }
    }
} // namespace soil::video::render
