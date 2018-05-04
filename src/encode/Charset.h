#ifndef UTIL_CHARSET_H
#define UTIL_CHARSET_H

#include <stdio.h>

namespace util {
namespace charset {

size_t covert(const char *fromCharset, const char *toCharset,
              char *inBuf, size_t inLen, char*outBuf, size_t outLen);

} // namespace charset
} // namespace util

#endif // UTIL_CHARSET_H
