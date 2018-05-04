#include "Dir.h"

#ifdef _WIN32

# include <io.h>
# include <direct.h>
# include "win_dirent.h"
# define F_OK 0
# define PATH_SEPERATOR '\\'

#define PATH_MAX MAX_PATH

typedef signed int ssize_t;

#else /* ifdef _WIN32 */

# include <dirent.h>
# include <sys/stat.h>
# include <unistd.h>
# define DIR_MASK 0755

#define PATH_SEPERATOR '/'

#endif /* ifdef _WIN32 */

namespace util {
namespace dir {

std::string getAppDir()
{
#ifdef _WIN32
    ///TODO:
    return std::string();
#else
    char path[PATH_MAX] = {0}; // current work dir
    ssize_t cnt = readlink("/proc/self/exe", path, PATH_MAX); //获取当前程序绝对路径
    if (cnt < 0 || cnt >= PATH_MAX) {
        return "";
    }

    //获取当前目录绝对路径，即去掉程序名
    for (int i = cnt; i >= 0; --i) {
        if ('/' == path[i]) {
            path[i] = '\0';
            break;
        }
    }

    std::string dir(path);
    if (dir.empty()) {
        dir = "/";
    }

    return dir;
#endif
}

bool isDirExist(const std::string &dir)
{
    if ("" == dir) {
        return true;
    }

    if (0 == access(dir.c_str(), F_OK)) {
        struct stat st;
        if (0 == stat(dir.c_str(), &st)) {
            if (S_ISDIR(st.st_mode)) {
                return true;
            }
        }
    }

    return false;
}

std::string getParentDir(const std::string &path)
{
    std::string parent;

    std::string _path = path;
    while (true)
    {
        size_t index = _path.find("//");
        if (index == std::string::npos)
        {
            break;
        }
        _path.replace(index, 2, "/");
    };

    size_t pos = _path.find_last_of(PATH_SEPERATOR);
    if (std::string::npos == pos) {
        parent = "";
    } else {
        parent = std::string(_path.c_str(), pos);
    }

    return parent;
}

bool mkDir(const std::string &dir)
{
    if (!isDirExist(dir)) {
        std::string parent = getParentDir(dir);
        if (!isDirExist(parent)) {
            if (!mkDir(parent)) {
                return false;
            }
        }
#ifdef _WIN32
        if (0 != mkdir(dir.c_str())) {
#else
        if (0 != mkdir(dir.c_str(), DIR_MASK)) {
#endif
            return false;
        }
    }

    return true;
}

bool fileList(std::vector<std::string> &files, const std::string& path,
                   bool recursive)
{
    DIR* dir = opendir(path.c_str());//打开指定目录
    if (nullptr == dir) {
        return false;
    }

    dirent* p = nullptr;//定义遍历指针
    while((p = readdir(dir)) != nullptr) {//开始逐个遍历
        if(p->d_name[0] != '.') { // 过滤隐藏文件 和 "." ".."
            std::string name = path + PATH_SEPERATOR + std::string(p->d_name);
            switch (p->d_type) {
                case DT_DIR:
                    if (recursive) {
                        if (fileList(files, name, recursive)) {
                            closedir(dir);
                            return false;
                        }
                    }
                    break;

                case DT_REG:
                case DT_LNK:
                    files.push_back(name);
                    break;

                default:
                    break;
            }
        }
    }

    closedir(dir);//关闭指定目录
    return true;
}

} // namespace dir
} // namespace util

