#ifndef VIDEO_RENDER_PROCESSING_ABSTRACT_PROCESSING_H
#define VIDEO_RENDER_PROCESSING_ABSTRACT_PROCESSING_H
#include <string>
#include "context.h"
#include "video/render/renderable.h"

namespace video::render {
 class Pipeline;

 class AbstractProcessing {
  friend class Pipeline;

 public:
  AbstractProcessing();

  explicit AbstractProcessing(std::string name);

  virtual ~AbstractProcessing();

  virtual void Process(Context &context, const std::vector<Renderable *> &renderables) = 0;

  [[nodiscard]] AbstractProcessing* GetRequiredStep() const;

  std::string GetName();

  void SetName(std::string name);

 protected:
  virtual void onAttach();

  AbstractProcessing *requiredStep_;

 private:
  std::string name_;
 };
}
#endif /* VIDEO_RENDER_PROCESSING_ABSTRACT_PROCESSING_H */

