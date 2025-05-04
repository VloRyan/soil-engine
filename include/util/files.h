#ifndef UTIL_FILES_H
#define UTIL_FILES_H
#include <string>
#include "base.h"

namespace util {

class Files {
 public:
  static bool Exists(const std::string& name);
  /**
   * Retrieve the directory path of the filepath. The returned path ends with path separator.
   * @param path
   * @return
   */
  static std::string GetDirectory(const std::string& path);
  static bool IsAbsolutePath(std::string path);
  static std::string GetExtension(const std::string& fileName, bool withDelimiter = true);

  /**
   * Checks if path points to an file.
   * @param path to check
   * @return true if its an existing file
   */
  static bool IsFile(const std::string& path);
  /**
   * Checks if path points to an directory.
   * @param path to check
   * @return true if its an existing directory
   */
  static bool IsDirectory(const std::string& path);
  /**
   * Recursive, portable wrapper for mkdir.
   * @param[in] path the full path of the directory to Create.
   * @return true on success, otherwise false.
   */
  static bool MkDir(const std::string& path);

  /**
   * Checks if a folder Exists
   * @param name path to the folder to check.
   * @return true if the folder Exists, false otherwise.
   */
  static bool FolderExists(const std::string& name);
 private:
  /**
   * Portable wrapper for mkdir. Internally used by mkdir()
   * @param[in] path the full path of the directory to Create.
   * @return zero on success, otherwise -1.
   */
  static int mkdir(const char* path);
};
}
#endif /* UTIL_FILES_H */

