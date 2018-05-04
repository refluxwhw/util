#include "Base64.h"

#include "_base64.h"

namespace util {
namespace encode {

static struct base64_context g_base64;
static bool g_isInit = false;

std::string encodeBase64(const char *data, size_t len)
{
    if (!g_isInit)
    {
        base64_init(&g_base64, 78);
        g_isInit = true;
    }

    int outLen = base64_get_encode_length(&g_base64, len);

    std::string str;
    str.reserve(outLen+1);

    base64_encode(&g_base64, data, len, (char*)str.c_str(), &outLen);

    return str;
}

size_t decodeBase64(const std::string &src, char **outBuf)
{
    if (!g_isInit)
    {
        base64_init(&g_base64, 78);
        g_isInit = true;
    }

    int outLen = (src.length()*3+3)/4 + 1;
    int realOutLen = 0;
    *outBuf = new char[outLen];
    base64_decode(&g_base64, src.c_str(), src.length(), *outBuf, &realOutLen);
    if (0 >= realOutLen)
    {
        realOutLen = 0;
        delete [] *outBuf;
        *outBuf = nullptr;
    }

    return realOutLen;
}

size_t decodeBase64(const char *data, size_t len, char **outBuf)
{
    if (!g_isInit)
    {
        base64_init(&g_base64, 78);
        g_isInit = true;
    }

    int outLen = (len*3+3)/4 + 1;
    int realOutLen = 0;
    *outBuf = new char[outLen];
    base64_decode(&g_base64, data, len, *outBuf, &realOutLen);
    if (0 >= realOutLen)
    {
        realOutLen = 0;
        delete [] *outBuf;
        *outBuf = nullptr;
    }

    return realOutLen;
}

} // namespace encode
} // namespace Util
