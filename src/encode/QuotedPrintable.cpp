#include "QuotedPrintable.h"

#include <stdio.h>
#include <string.h>

namespace util {
namespace encode  {

std::string encodeQP(const char *data, size_t len, size_t maxLine)
{
    char* buf = new char[len*3+1];

    const char* src = data;
    char* dst = buf;

    size_t lineSize = 0;       // 行长度计数
    while (src < data + len)
    {
        // ASCII 33-60, 62-126原样输出，其余的需编码
        if ((*src >= '!') && (*src <= '~') && (*src != '='))
        {
            *dst++ = (char)*src++;
            lineSize++;
        }
        else
        {
            sprintf(dst, "=%02X", *src++);
            dst += 3;
            lineSize += 3;
        }

        // 输出换行？
        if (lineSize >= maxLine - 3)
        {
            sprintf(dst, "=\r\n");
            dst += 3;
            lineSize = 0;
        }
    }

    // 输出加个结束符
    *dst = '\0';

    std::string outStr = std::string(buf);
    delete [] buf;
    buf = nullptr;

    return outStr;
}

size_t decodeQP(const char *pSrc, size_t nSrcLen, char **outBuf)
{
    size_t nDstLen = 0;        // 输出的字符计数
    size_t i = 0;

    char* pDst = new char[nSrcLen];
    *outBuf = pDst;

    char hex[8] = {0};
    while (i < nSrcLen)
    {
        if (pSrc[i] == '=') // 是编码字节
        {
            if ( pSrc[i+1] == '\r'
                 && pSrc[i+2] == '\n' ) // 软回车，跳过
            {
                i += 3;
            }
            else if (pSrc[i+1] == '\n')
            {
                i += 2;
            }
            else
            {
                memcpy(hex, pSrc+i+1, 2);
                int c = 0;
                sscanf(hex, "%x", &c);
                *pDst++ = (char)c;
                nDstLen++;

                i += 3;
            }
        }
        else if (pSrc[i] != '\r' && pSrc[i] != '\n') // 非编码字节
        {
            *pDst++ = pSrc[i++];
            nDstLen++;
        }
        else
        {
            i++;
        }
    }

    // 输出加个结束符
    *pDst = '\0';

    return nDstLen;
}

} // namespace encode
} // namespace util
