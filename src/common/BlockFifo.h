#ifndef BLOCKFIFO_H
#define BLOCKFIFO_H

#include <map>
#include <mutex>

#include "Fifo.hpp"

namespace util {

class BlockFifo
{
public:
    BlockFifo();

    bool init(size_t blockSize, size_t blockNum);
    void uninit();

    char* wget();
    char* wgetLocked();
    bool wput(char* b);
    bool wputLocked(char* b);
    char* rget();
    char* rgetLocked();
    bool rput(char* b);
    bool rputLocked(char* b);

private:
    unsigned long roundup_pow_of_two(unsigned long val) {
        if((val & (val-1)) == 0) return val;

        unsigned long maxulong = (unsigned long)((unsigned long)~0);
        unsigned long andv = ~(maxulong&(maxulong>>1));
        while((andv & val) == 0) andv = andv>>1;

        return andv<<1;
    }

private:
    char   *m_buffer    = nullptr;
    size_t  m_blockSize = 0;
    size_t  m_blockNum  = 0;

    Fifo<char*> m_wFifo;
    Fifo<char*> m_rFifo;

    std::map<char*, char> m_wMap;
    std::map<char*, char> m_rMap;

    std::mutex m_rLock;
    std::mutex m_wLock;
};

} // namespace util {

#endif // BLOCKFIFO_H
