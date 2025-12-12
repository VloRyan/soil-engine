
#ifndef SOIL_VIDEO_TEXTURE_ANIMATION_H
#define SOIL_VIDEO_TEXTURE_ANIMATION_H
#include <vector>
namespace soil::video::texture {
class Animation {
 public:
  struct Frame {
    int Index{-1};
    int Duration{-1};
    friend bool operator==(const Frame& lhs, const Frame& rhs) {
      return lhs.Index == rhs.Index && lhs.Duration == rhs.Duration;
    }
    friend bool operator!=(const Frame& lhs, const Frame& rhs) {
      return !(lhs == rhs);
    }
  };
  explicit Animation(const std::vector<Frame>*, int ticksPerSecond = 25);
  ~Animation() = default;
  const Animation::Frame& Update();
  [[nodiscard]] const Frame& CurrentFrame() const;
  void SetFrameIndex(int index);
  [[nodiscard]] const std::vector<Frame>* Frames() const;
  [[nodiscard]] float TickDuration() const;

  void SetFrames(const std::vector<Frame>* frames);
  static Frame EMPTY_FRAME;

 private:
  float tickDuration_;
  float currentDuration_;
  int currentFrameIndex_;
  const std::vector<Frame>* frames_;
};
}  // namespace soil::video::texture

#endif
