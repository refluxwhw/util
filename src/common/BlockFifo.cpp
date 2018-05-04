#include "BlockFifo.h"

namespace util {


BlockFifo::BlockFifo()
{

}

bool BlockFifo::init(size_t blockSize, size_t blockNum)
{
    uninit();
    m_blockSize = blockSize;
    m_blockNum = roundup_pow_of_two(blockNum);

    m_buffer = new char[m_blockNum * m_blockSize];
    if ( nullptr == m_buffer
         || !m_wFifo.init(m_blockNum)
         || !m_rFifo.init(m_blockNum) ) {
        uninit();
        return false;
    }

    for (size_t i=0; i<m_blockNum; i++) {
        m_wFifo.put(m_buffer+i*m_blockSize);
    }

    return true;
}

void BlockFifo::uninit()
{
    m_wMap.clear();
    m_rMap.clear();
    m_wFifo.uninit();
    m_rFifo.uninit();

    m_blockSize = 0;
    m_blockNum  = 0;

    if (nullptr != m_buffer) {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

char *BlockFifo::wget()
{
    char* b = nullptr;
    if (!m_wFifo.get(b)) {
        return nullptr;
    } else {
        m_wMap[b] = 0;
    }
    return b;
}

char *BlockFifo::wgetLocked()
{
    m_wLock.lock();
    char* b = wget();
    m_wLock.unlock();
    return b;
}

bool BlockFifo::wput(char *b)
{
    auto itr = m_wMap.find(b);
    if (itr == m_wMap.end()) {
        return false;
    }
    m_wMap.erase(itr);
    m_rFifo.put(b);
    return true;
}

bool BlockFifo::wputLocked(char *b)
{
    m_wLock.lock();
    bool ret = wput(b);
    m_wLock.unlock();
    return ret;
}

char *BlockFifo::rget()
{
    char* b = nullptr;
    if (!m_rFifo.get(b)) {
        return nullptr;
    } else {
        m_rMap[b] = 0;
    }
    return b;
}

char *BlockFifo::rgetLocked()
{
    m_rLock.lock();
    char* b = rget();
    m_rLock.unlock();
    return b;
}

bool BlockFifo::rput(char *b)
{
    auto itr = m_rMap.find(b);
    if (itr == m_rMap.end()) {
        return false;
    }
    m_rMap.erase(itr);
    m_wFifo.put(b);
    return true;
}

bool BlockFifo::rputLocked(char *b)
{
    m_rLock.lock();
    bool ret = rput(b);
    m_rLock.unlock();
    return ret;
}

} // namespace util
