#ifndef UTILBASE64_H
#define UTILBASE64_H

#include <iostream>

namespace util {
namespace encode {

std::string encodeBase64(const char* data, size_t len);
size_t decodeBase64(const std::string& src, char** outBuf);
size_t decodeBase64(const char* data, size_t len, char** outBuf);

} // namespace encode
} // namespace util


#endif // UTILBASE64_H
