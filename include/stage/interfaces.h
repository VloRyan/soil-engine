#ifndef SOIL_ENGINE_STAGE_INTERFACES_H
#define SOIL_ENGINE_STAGE_INTERFACES_H

namespace soil::stage {
class Stage;

class IManager {
 public:
  virtual ~IManager() = default;

  virtual void SetCurrent(Stage* stage) = 0;

  virtual void SetCurrent(const std::string& name) = 0;

  [[nodiscard]] virtual Stage* GetStage(const std::string& name) const = 0;

  [[nodiscard]] virtual Stage* RemoveStage(const std::string& name) = 0;
};
}  // namespace soil::stage
#endif
