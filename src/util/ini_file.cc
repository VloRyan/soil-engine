
#include "util/ini_file.h"

#include <fstream>

#include "util/strings.h"

namespace util {
std::unordered_map<std::string, std::string>
ReadIniFile(const std::string &fileName) {
  std::ifstream infile(fileName);
  std::string line;
  auto map = std::unordered_map<std::string, std::string>();
  while (std::getline(infile, line)) {
    Strings::trim(line);
    if (line.starts_with(";")) {
      continue;
    }
    auto pos = line.find('=');
    if (pos == std::string::npos) {
      continue;
    }
    auto key = line.substr(0, pos);
    auto value = line.substr(pos + 1);
    map.insert({key, value});
  }
  return map;
}
} // namespace util
