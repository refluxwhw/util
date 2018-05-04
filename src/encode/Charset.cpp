#include "Charset.h"

#include <string.h>
#include <errno.h>

#ifdef _WIN32

#else /* ifdef _WIN32 */
# include <iconv.h>
#endif /* ifdef _WIN32 */

namespace util {

size_t covert(const char *fromCharset, const char *toCharset,
              char *inBuf, size_t inLen, char *outBuf, size_t outLen)
{
#ifdef _WIN32
    /// TODO: windows 下没有可以根据编码名称进行转码的库
    return false;
#else /* ifdef _WIN32 */
    char **pin = &inBuf;
    char **pout = &outBuf;

    iconv_t cd = iconv_open(toCharset, fromCharset);
    if (cd == 0) {
        return 0;
    }
    memset(outBuf, 0, outLen);

    size_t leftToCovert = inLen;
    size_t leftToSave = outLen;
    if ((size_t) -1 == iconv(cd, pin, &leftToCovert, pout, &leftToSave)) {
        iconv_close(cd);

        switch (errno) {
            case E2BIG: // out put buffer is not big enough
                break;

            case EINVAL: // input is not completion
                break;

            case EILSEQ: // input is error
                break;

            default:
                break;
        }
        return 0;
    }
    iconv_close(cd);

    return outLen-leftToSave;
#endif /* ifdef _WIN32 */
}

} // namespace Util

