#include "util/files.h"
#include <sys/stat.h>
#include <sstream>

// for windows mkdir
#ifdef _WIN32
#include <direct.h>

#endif
namespace util {
    bool Files::Exists(const std::string &name) {
        if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        }
        return false;
    }

    std::string Files::GetDirectory(const std::string &path) {
        const size_t lastSlash = path.find_last_of('/');
        const size_t lastBackSlash = path.find_last_of('\\');
        std::string directory;
        if (lastSlash != std::string::npos) {
            if (lastBackSlash != std::string::npos) {
                if (lastSlash > lastBackSlash) {
                    directory = path.substr(0, lastSlash + 1);
                } else {
                    directory = path.substr(0, lastBackSlash + 1);
                }
            } else {
                directory = path.substr(0, lastSlash + 1);
            }
        } else if (lastBackSlash != std::string::npos) {
            directory = path.substr(0, lastBackSlash + 1);
        }
        return directory;
    }

    bool Files::IsAbsolutePath(const std::string path) {
        if (path.length() < 3) {
            return false;
        }
        return path.at(1) == ':';
    }

    std::string Files::GetExtension(const std::string &fileName, const bool withDelimiter) {
        const std::string::size_type offset = withDelimiter ? 0U : 1U;
        const std::string::size_type lastDot = fileName.rfind('.');
        if (lastDot == std::string::npos) {
            return "";
        }
        return fileName.substr(lastDot + offset);
    }

    /**
     * Portable wrapper for mkdir. Internally used by mkdir()
     * @param[in] path the full path of the directory to Create.
     * @return zero on success, otherwise -1.
     */
    int Files::mkdir(const char *path) {
#ifdef _WIN32
  return ::_mkdir(path);
#else
#if _POSIX_C_SOURCE
        return ::mkdir(path, 0755);
#else
  return ::mkdir(path, 0755); // not sure if this works on mac
#endif
#endif
    }

    bool Files::MkDir(const std::string &path) {
        std::string current_level;
        std::string level;
        std::stringstream stream(path);
        // split path using slash as a separator
        while (std::getline(stream, level, '/')) {
            current_level += level; // append folder to the current level

            // Create current level
            if (!FolderExists(current_level) && mkdir(current_level.c_str()) != 0) {
                return false;
            }

            current_level += "/"; // don't forget to append a slash
        }

        return true;
    }

    bool Files::FolderExists(const std::string &name) {
        return IsDirectory(name);
    }

    bool Files::IsDirectory(const std::string &path) {
        struct stat stat_{};
        if (stat(path.c_str(), &stat_) == 0) {
            return (stat_.st_mode & S_IFDIR) != 0U;
        }
        return false;
    }

    bool Files::IsFile(const std::string &path) {
        struct stat stat_{};
        if (stat(path.c_str(), &stat_) == 0) {
            return (stat_.st_mode & S_IFREG) != 0U;
        }
        return false;
    }
}
