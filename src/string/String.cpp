#include "String.h"
#include <algorithm>

namespace util {
namespace string {

bool compare(const std::string &str1, const std::string &str2,
                     bool ignoreCase)
{
    if (ignoreCase)
    {
        return (0 == str1.compare(str2));
    }
    else
    {
        std::string strA = str1;
        std::string strB = str2;
        toLowerCase(strA);
        toLowerCase(strB);
        return (0 == strA.compare(strB));
    }
}

std::string& toLowerCase(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string& toUpCase(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string &stripQuotes(std::string &str)
{
    while ( str.length() > 2
            && '"' == str.at(0)
            && '"' == str.at(str.length()-1) ) {
        str = std::string(str.c_str()+1, str.length()-2);
    }
    return str;
}

std::string &trim(std::string &str, std::string s)
{
    size_t index = str.find_first_not_of(s);
    if (std::string::npos == index) {
        str.clear();
        return str;
    } else {
        str = std::string(str.c_str()+index);
    }

    index = str.find_last_not_of(s);
    str = std::string(str.c_str(), index+1);

    return str;
}

std::vector<std::string> split(const std::string &s,
                                       const std::string &seperator)
{
    std::vector<std::string> result;
    size_t i = 0;

    while(i != s.size())
    {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0)
        {
            flag = 1;
            for(size_t x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        size_t j = i;
        while(j != s.size() && flag == 0)
        {
            for(size_t x = 0; x < seperator.size(); ++x)
            {
                if(s[j] == seperator[x])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                ++j;
            }
        }
        if(i != j)
        {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

} // namespace string
} // namespace util

