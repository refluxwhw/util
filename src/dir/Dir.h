#ifndef UTIL_DIR_H
#define UTIL_DIR_H

#include <iostream>
#include <vector>

namespace util {
namespace dir {

std::string getAppDir();
bool isDirExist(const std::string &dir);
std::string getParentDir(const std::string& path);
bool mkDir(const std::string& dir);
bool fileList(std::vector<std::string>& files, const std::string &path,
              bool recursive = false);

} // namespace dir
} // namespace util

#endif // UTIL_DIR_H
