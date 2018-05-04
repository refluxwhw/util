#include "File.h"

#ifdef _WIN32
# include <io.h>
# define F_OK 0
#else /* ifdef _WIN32 */
# include <unistd.h>
#endif /* ifdef _WIN32 */

namespace util {
namespace file {

bool isFileExist(const char *path)
{
    return 0 == access(path, F_OK);
}

} // namespace file
} // namespace util
