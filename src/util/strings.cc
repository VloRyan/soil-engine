#include "util/strings.h"

#include <algorithm>
#include <cctype>
#include <glm/glm.hpp>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "base.h"

namespace soil::util {
    std::string Strings::to_string(const bool value, std::array<std::string, 2> options) {
        if (value == false) {
            return options[0];
        }
        return options[1];
    }

    std::string Strings::to_string(const float value, const int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << value;
        std::string s = stream.str();
        return s;
    }

    std::string Strings::to_string(const int value, const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << "(" << value << ")";
        } else {
            stream << value;
        }
        std::string s = stream.str();
        return s;
    }

    std::string Strings::to_string(const glm::vec2 value, const int precision, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << std::fixed << std::setprecision(precision) << "(" << value.x << delimiter << value.y << ")";
        } else {
            stream << std::fixed << std::setprecision(precision) << value.x << delimiter << value.y;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::vec3 value, const int precision, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << std::fixed << std::setprecision(precision) << "(" << value.x << delimiter << value.y << delimiter
                   << value.z << ")";
        } else {
            stream << std::fixed << std::setprecision(precision) << value.x << delimiter << value.y << delimiter
                   << value.z;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::vec4 value, const int precision, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << std::fixed << std::setprecision(precision) << "(" << value.x << delimiter << value.y << delimiter
                   << value.z << delimiter << value.w << ")";
        } else {
            stream << std::fixed << std::setprecision(precision) << value.x << delimiter << value.y << delimiter
                   << value.z << delimiter << value.w;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::ivec2 value, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << "(" << value.x << delimiter << value.y << ")";
        } else {
            stream << value.x << delimiter << value.y;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::ivec3 value, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << "(" << value.x << delimiter << value.y << delimiter << value.z << ")";
        } else {
            stream << value.x << delimiter << value.y << delimiter << value.z;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::ivec4 value, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        if (surroundingParentheses) {
            stream << "(" << value.x << delimiter << value.y << delimiter << value.z << delimiter << value.w << ")";
        } else {
            stream << value.x << delimiter << value.y << delimiter << value.z << delimiter << value.w;
        }
        return stream.str();
    }

    std::string Strings::to_string(const glm::mat4& matrix, const int precision, const std::string& delimiter,
                                   const bool surroundingParentheses) {
        std::stringstream stream;
        for (int i = 0; i < 4; ++i) {
            if (surroundingParentheses) {
                stream << std::fixed << std::setprecision(precision) << "(" << matrix[0][i] << delimiter << matrix[1][i]
                       << delimiter << matrix[2][i] << delimiter << matrix[3][i] << ")";
            } else {
                stream << std::fixed << std::setprecision(precision) << matrix[0][i] << delimiter << matrix[1][i]
                       << delimiter << matrix[2][i] << delimiter << matrix[3][i];
            }
            if (i < 3) {
                stream << "\n";
            }
        }
        return stream.str();
    }

    void Strings::ltrim(std::string& str) {
        str.erase(str.begin(), std::ranges::find_if(str, [](const int chr) { return !std::isspace(chr); }));
    }

    void Strings::rtrim(std::string& str) {
        str.erase(std::ranges::find_if(std::ranges::reverse_view(str), [](const int chr) { return !std::isspace(chr); })
                      .base(),
                  str.end());
    }

    void Strings::trim(std::string& str) {
        ltrim(str);
        rtrim(str);
    }

    std::string Strings::ltrim_copy(std::string str) {
        ltrim(str);
        return str;
    }

    std::string Strings::rtrim_copy(std::string str) {
        rtrim(str);
        return str;
    }

    std::string Strings::trim_copy(std::string str) {
        trim(str);
        return str;
    }

    bool Strings::equals(const std::string& str1, const std::string& str2) {
        return str1 == str2;
    }

    bool Strings::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
        toLower(str1);
        toLower(str2);
        return str1 == str2;
    }

    void Strings::toLower(std::string str) {
        std::ranges::transform(str, str.begin(),
                               [](const byte letter) -> byte { return static_cast<byte>(std::tolower(letter)); });
    }

    void Strings::toUpper(std::string str) {
        std::ranges::transform(str, str.begin(),
                               [](const byte letter) -> byte { return static_cast<byte>(std::toupper(letter)); });
    }

    std::vector<std::string> Strings::split(std::string text, const std::string& by) {
        std::string tmpText = std::move(text);

        size_t pos = 0U;
        std::vector<std::string> retList;
        while ((pos = tmpText.find(by)) != std::string::npos) {
            std::string token = tmpText.substr(0, pos);
            tmpText.erase(0, pos + by.length());
            retList.push_back(token);
        }
        retList.push_back(tmpText);
        return retList;
    }

    bool Strings::to_bool(const std::string& str) {
        return str == "true";
    }

    glm::vec2 Strings::to_vec2(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 2) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + "' : " + str);
        }
        glm::vec2 returnValue;
        for (glm::vec2::length_type i = 0; i < 2; i++) {
            returnValue[i] = std::stof(parts[i]);
        }
        return returnValue;
    }

    glm::vec3 Strings::to_vec3(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 3) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + delimiter +
                                     "#' : " + str);
        }
        glm::vec3 returnValue;
        for (glm::vec3::length_type i = 0; i < 3; i++) {
            returnValue[i] = std::stof(parts[i]);
        }
        return returnValue;
    }

    glm::vec4 Strings::to_vec4(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 4) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + delimiter + "#" +
                                     delimiter + "#' : " + str);
        }
        glm::vec4 returnValue;
        for (glm::vec4::length_type i = 0; i < 4; i++) {
            returnValue[i] = std::stof(parts[i]);
        }
        return returnValue;
    }

    glm::uvec2 Strings::to_uvec2(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 2) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + "' : " + str);
        }
        glm::uvec2 returnValue;
        for (glm::uvec2::length_type i = 0; i < 2; i++) {
            returnValue[i] = static_cast<uint>(std::stoi(parts[i]));
        }
        return returnValue;
    }

    glm::uvec3 Strings::to_uvec3(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 3) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + delimiter +
                                     "#' : " + str);
        }
        glm::uvec3 returnValue;
        for (glm::uvec3::length_type i = 0; i < 3; i++) {
            returnValue[i] = static_cast<uint>(std::stoi(parts[i]));
        }
        return returnValue;
    }

    glm::uvec4 Strings::to_uvec4(const std::string& str, const std::string& delimiter) {
        const std::vector<std::string> parts = split(str, delimiter);
        if (parts.size() != 4) {
            throw std::runtime_error("String format does not match format '#" + delimiter + "#" + delimiter + "#" +
                                     delimiter + "#' : " + str);
        }
        glm::uvec4 returnValue;
        for (glm::uvec4::length_type i = 0; i < 4; i++) {
            returnValue[i] = static_cast<uint>(std::stoi(parts[i]));
        }
        return returnValue;
    }

    bool Strings::startsWith(const std::string& text, const std::string& starting) {
        if (text.length() >= starting.length()) {
            return text.starts_with(starting);
        }
        return false;
    }

    bool Strings::endsWith(const std::string& text, const std::string& ending) {
        if (text.length() >= ending.length()) {
            return 0 == text.compare(text.length() - ending.length(), ending.length(), ending);
        }
        return false;
    }
} // namespace soil::util
