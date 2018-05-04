#ifndef UTILSTRING_H
#define UTILSTRING_H

#include <iostream>
#include <vector>

namespace util {
namespace string {



bool compare(const std::string& str1, const std::string& str2,
             bool ignoreCase = true);
std::string &toLowerCase(std::string& str);
std::string &toUpCase(std::string& str);
std::string& stripQuotes(std::string& str);
std::string& trim(std::string& str, std::string s = " \t");
// 可以按多个字符来分割
std::vector<std::string> split(const std::string &s,
                                      const std::string &seperator);


} // namespace string
} // namespace util



#endif // UTILSTRING_H
