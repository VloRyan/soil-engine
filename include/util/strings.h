#ifndef ENGINE_ENGINE_STRINGS_STRINGS_H_
#define ENGINE_ENGINE_STRINGS_STRINGS_H_

#include <string>
#include <vector>
#include "glm/glm.hpp"

namespace util {
    class Strings {
    public:
        /**
         * @return string in format (x, y)
         */
        static std::string to_string(float value, int precision = 3);

        /**
         * @return string in format (x, y)
         */
        static std::string to_string(int value, bool surroundingParentheses = true);

        /**
         * @return string in format (x, y)
         */
        static std::string to_string(glm::vec2 value,
                                     int precision = 3,
                                     const std::string &delimiter = ", ",
                                     bool surroundingParentheses = true);

        /**
         * @return string in format (x, y, z)
         */
        static std::string to_string(glm::vec3 value,
                                     int precision = 3,
                                     const std::string &delimiter = ", ",
                                     bool surroundingParentheses = true);

        /**
         * @return string in format (x, y, z, w)
         */
        static std::string to_string(glm::vec4 value,
                                     int precision = 3,
                                     const std::string &delimiter = ", ",
                                     bool surroundingParentheses = true);

        /**
         * @return string in format "(x, y, z, w)\n" for each row
         */
        static std::string to_string(const glm::mat4 &matrix,
                                     int precision = 3,
                                     const std::string &delimiter = ", ",
                                     bool surroundingParentheses = true);

        /**
         * trim from start (in place)
         */
        static void ltrim(std::string &str);

        /**
         * trim from end (in place)
         */
        static void rtrim(std::string &str);

        /**
         * trim from both ends (in place)
         */
        static void trim(std::string &str);

        /**
         * trim from start (copying)
         */
        static std::string ltrim_copy(std::string str);

        /**
         *  trim from end (copying)
         */
        static std::string rtrim_copy(std::string str);

        /**
         *  trim from both ends (copying)
         */
        static std::string trim_copy(std::string str);

        static bool equals(const std::string &str1, const std::string &str2);

        static bool equalsIgnoreCase(const std::string &str1, const std::string &str2);

        static void toLower(std::string str);

        static void toUpper(std::string str);

        static std::vector<std::string> split(std::string text, const std::string &by);

        static bool to_bool(const std::string &str);

        static glm::vec2 to_vec2(const std::string &str, const std::string &delimiter = ",");

        static glm::vec3 to_vec3(const std::string &str, const std::string &delimiter = ",");

        static glm::vec4 to_vec4(const std::string &str, const std::string &delimiter = ",");

        static glm::uvec2 to_uvec2(const std::string &str, const std::string &delimiter = ",");

        static glm::uvec3 to_uvec3(const std::string &str, const std::string &delimiter = ",");

        static glm::uvec4 to_uvec4(const std::string &str, const std::string &delimiter = ",");

        static bool endsWith(const std::string &text, const std::string &ending);

        static bool startsWith(const std::string &text, const std::string &starting);
    };
} // util

#endif //ENGINE_ENGINE_STRINGS_STRINGS_H_
