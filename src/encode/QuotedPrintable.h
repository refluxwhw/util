#ifndef UTIL_QUOTEDPRINTABLE_H
#define UTIL_QUOTEDPRINTABLE_H

#include <iostream>

namespace util {
namespace encode {

std::string encodeQP(const char* data, size_t len, size_t maxLine = 76);
size_t decodeQP(const char* data, size_t len, char** outBuf);

} // namespace encode
} // namespace util

#endif // UTIL_QUOTEDPRINTABLE_H
