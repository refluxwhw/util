#ifndef CHARFIFO_H
#define CHARFIFO_H

#include <stdio.h>
#include <pthread.h>

namespace util {

class CharFifo
{
public:
    CharFifo(int nSize);
    virtual ~CharFifo();

    bool init();

    size_t put(const char *buffer, size_t len, bool full = false);
    size_t putSpinlocked(const char *buffer, size_t len, bool full = false);
    size_t get(char *buffer, size_t len, bool full = false);
    size_t getSpinlocked(char *buffer, size_t len, bool full = false);

    inline void clean() { m_in = m_out = 0; }
    inline size_t getDataLen() const { return  m_in - m_out; }

private:
    inline size_t leftSize() { return m_mask + 1 - m_in + m_out; }
    inline unsigned long roundup_power_of_two(unsigned long val);

private:
    char  *m_buffer = nullptr;
    size_t m_mask   = 0;
    size_t m_in     = 0;
    size_t m_out    = 0;

    pthread_spinlock_t m_inLock;
    pthread_spinlock_t m_outLock;
};

} // namespace util

#endif // CHARFIFO_H
