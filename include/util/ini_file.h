
#ifndef INI_FILE_H
#define INI_FILE_H
#include <string>
#include <unordered_map>

namespace util {
    static std::unordered_map<std::string, std::string> ReadIniFile(const std::string &fileName);
}


#endif //INI_FILE_H
