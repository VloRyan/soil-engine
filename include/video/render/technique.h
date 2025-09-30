#ifndef SOIL_VIDEO_RENDER_TECHNIQUE_H_
#define SOIL_VIDEO_RENDER_TECHNIQUE_H_

#include <vector>

#include "renderable.h"
#include "state.h"

namespace soil::video::render {
enum class TechniqueType : std::uint8_t {
  Forward,
  Deferred,
};

class Technique {
 public:
  virtual ~Technique() = default;

  virtual void Render(State& state,
                      const std::vector<Renderable*>& renderables) const = 0;

  static Technique* GetTechnique(TechniqueType type);
};
}  // namespace soil::video::render

#endif
