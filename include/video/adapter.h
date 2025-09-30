#ifndef SOIL_VIDEO_ADAPTER_H_
#define SOIL_VIDEO_ADAPTER_H_

#include <cstdint>
#include <string>

namespace soil::video {
enum Vendor : std::uint8_t { ATI = 0, NVIDIA = 1, INTEL = 3, OTHER = 99 };

class Adapter {
 public:
  Adapter(Vendor vendor, std::string model);

  [[nodiscard]] Vendor GetVendor() const;

  std::string GetModel();

 private:
  Vendor vendor_;
  std::string model_;
};
}  // namespace soil::video

#endif
